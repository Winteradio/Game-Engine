#include <RHI/Vulkan/VulkanContext.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

#include <RHI/RHIStates.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIUtils.h>
#include <RHI/Vulkan/VulkanDevice.h>
#include <RHI/Vulkan/VulkanSurface.h>
#include <RHI/Vulkan/VulkanCommand.h>
#include <RHI/Vulkan/VulkanMemory.h>
#include <RHI/Vulkan/VulkanResources.h>

#include <cmath>

namespace wtr
{
	VulkanContext::VulkanContext()
		: RHIContext(eRenderType::eVulkan)
		, m_device()
		, m_surface()
		, m_memory()
		, m_primary()
		, m_staging()
		, m_releaseStaging()
		, m_acquire(VK_NULL_HANDLE)
		, m_present(VK_NULL_HANDLE)
		, m_fence(VK_NULL_HANDLE)
		, m_imageIndex(UINT32_MAX)
	{
	}

	VulkanContext::~VulkanContext()
	{
		if (!m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] Failed to release the vulkan system, the device is invalid";
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		if (m_acquire)
		{
			vkDestroySemaphore(logicalDevice, m_acquire, nullptr);
		}

		if (m_present)
		{
			vkDestroySemaphore(logicalDevice, m_present, nullptr);
		}

		if (m_fence)
		{
			vkDestroyFence(logicalDevice, m_fence, nullptr);
		}

		if (m_memory)
		{
			m_memory->Release(m_device);
			m_memory.Reset();
		}

		if (m_primary)
		{
			m_primary->Release(m_device);
			m_primary.Reset();
		}

		if (m_surface)
		{
			m_surface->Release(m_device);
			m_surface.Reset();
		}

		if (m_device)
		{
			m_device->Release();
			m_device.Reset();
		}
	}

	bool VulkanContext::Init(void* nativeHandle)
	{
		constexpr size_t MAX_SECONDARY = 2;

		m_device = !m_device ? Memory::MakeRef<VulkanDevice>() : m_device;
		m_surface = !m_surface ? Memory::MakeRef<VulkanSurface>() : m_surface;
		m_memory = !m_memory ? Memory::MakeRef<VulkanMemory>() : m_memory;

		m_primary = !m_primary ? Memory::MakeRef<VulkanCommand>(true) : m_primary;

		if (!m_device->InitInstance() || !m_device->InitDebugMessenger())
		{
			return false;
		}

		if (!m_surface->InitSurface(nativeHandle, m_device))
		{
			return false;
		}

		if (!m_device->InitDevice(m_surface))
		{
			return false;
		}

		if (!m_surface->InitSwapChain(nativeHandle, m_device))
		{
			return false;
		}

		if (!m_primary->Init(m_device))
		{
			return false;
		}

		m_memory->Init(m_device);

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkSemaphoreCreateInfo semaPhoreInfo{};
		semaPhoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if (vkCreateFence(logicalDevice, &fenceInfo, nullptr, &m_fence) != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to create the fence";
			return false;
		}

		if (vkCreateSemaphore(logicalDevice, &semaPhoreInfo, nullptr, &m_acquire) != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to create the acquire semaphore";
			return false;
		}

		if (vkCreateSemaphore(logicalDevice, &semaPhoreInfo, nullptr, &m_present) != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to create the present semaphore";
			return false;
		}

		LOGINFO() << "[Vk] Succeed to initialize the vulkan system";

		return true;
	}

	void VulkanContext::FlushPending()
	{
		if (!m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		vkWaitForFences(logicalDevice, 1, &m_fence, VK_TRUE, UINT64_MAX);
		vkResetFences(logicalDevice, 1, &m_fence);

		FlushBuffer();
		FlushVertexLayout();
		FlushTexture();
		FlushSampler();
		FlushShader();
		FlushPipeline();
		FlushRenderTarget();
	}

	void VulkanContext::Clear(const RHIClearState& state)
	{
	}

	void VulkanContext::BeginContext()
	{
	}

	void VulkanContext::EndContext()
	{
		if (!m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();
		vkDeviceWaitIdle(logicalDevice);
	}

	void VulkanContext::BeginFrame()
	{
		if (!m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE ||
			!m_surface || m_surface->GetSwapChain() == VK_NULL_HANDLE ||
			!m_primary || m_primary->GetCommandBuffer() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();
		VkSwapchainKHR swapChain = m_surface->GetSwapChain();

		VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain, UINT64_MAX, m_acquire, VK_NULL_HANDLE, &m_imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			LOGWARN() << "[Vk] The swap chain image is not fitted the viewport";
			m_imageIndex = UINT32_MAX;
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			m_imageIndex = UINT32_MAX;
			return;
		}
		else if (result == VK_SUBOPTIMAL_KHR)
		{
			LOGWARN() << "[Vk] The swap chain image is not optimaled for the viewport";

			// No `return` here on purpose: unlike OUT_OF_DATE_KHR, SUBOPTIMAL_KHR still means
			// Acquire succeeded - a valid image was handed back AND m_acquire was signaled.
			// If we bailed out here, nothing would ever consume that signal (Submit never runs),
			// so next frame's Acquire would signal m_acquire again with no wait in between -
			// the same signal-signal violation m_acquire/m_present are split apart to avoid.
		}

		vkWaitForFences(logicalDevice, 1, &m_fence, VK_TRUE, UINT64_MAX);
		vkResetFences(logicalDevice, 1, &m_fence);

		m_primary->ResetPool(m_device);
		m_staging.Insert(m_staging.End(), m_releaseStaging.Begin(), m_releaseStaging.End());
		m_releaseStaging.Clear();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext = nullptr;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VkCommandBuffer commandBuffer = m_primary->GetCommandBuffer();

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
	}

	void VulkanContext::EndFrame()
	{
		if (!m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE ||
			!m_surface || m_surface->GetSwapChain() == VK_NULL_HANDLE ||
			!m_primary || m_primary->GetCommandBuffer() == VK_NULL_HANDLE)
		{
			return;
		}

		VkQueue queue = m_device->GetQueue();
		VkSwapchainKHR swapChain = m_surface->GetSwapChain();
		VkCommandBuffer commandBuffer = m_primary->GetCommandBuffer();

		vkEndCommandBuffer(commandBuffer);

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_acquire;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_present;
		
		if (vkQueueSubmit(queue, 1, &submitInfo, m_fence) != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to submit the command buffer to the vkQueue";
			return;
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_present;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapChain;
		presentInfo.pImageIndices = &m_imageIndex;
		
		if (vkQueuePresentKHR(queue, &presentInfo) != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to present the swap chaing image";
			return;
		}
	}

	void VulkanContext::Resize(const uint32_t width, const uint32_t height)
	{
		if (!m_primary || m_primary->GetCommandBuffer() == VK_NULL_HANDLE)
		{
			return;
		}

		VkCommandBuffer commandBuffer = m_primary->GetCommandBuffer();

		VkViewport viewport{};
		viewport.x = 0;
		viewport.y = height;
		viewport.width = width;
		viewport.height = -height;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	}

	void VulkanContext::Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height)
	{
		if (!m_primary || m_primary->GetCommandBuffer() == VK_NULL_HANDLE)
		{
			return;
		}

		VkCommandBuffer commandBuffer = m_primary->GetCommandBuffer();

		VkViewport viewport{};
		viewport.x = posX;
		viewport.y = height + posY;
		viewport.width = width;
		viewport.height = -height;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	}

	Memory::RefPtr<RHIBuffer> VulkanContext::CreateBuffer(const RHIBufferDesc desc)
	{
		Memory::RefPtr<RHIBuffer> refBuffer = Memory::MakeRef<VulkanBuffer>(desc);

		return refBuffer;
	}

	Memory::RefPtr<RHIVertexLayout> VulkanContext::CreateVertexLayout(const RHIVertexLayoutDesc desc)
	{
		Memory::RefPtr<RHIVertexLayout> refLayout = Memory::MakeRef<VulkanVertexLayout>(desc);

		return refLayout;
	}

	Memory::RefPtr<RHITexture> VulkanContext::CreateTexture(const RHITextureDesc desc)
	{
		Memory::RefPtr<RHITexture> refTexture = Memory::MakeRef<VulkanTexture>(desc);

		return refTexture;
	}

	Memory::RefPtr<RHISampler> VulkanContext::CreateSampler(const RHISamplerDesc desc)
	{
		Memory::RefPtr<RHISampler> refSampler = Memory::MakeRef<VulkanSampler>(desc);

		return refSampler;
	}

	Memory::RefPtr<RHIShader> VulkanContext::CreateVertexShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refVertexShader = Memory::MakeRef<VulkanVertexShader>(desc);

		return refVertexShader;
	}

	Memory::RefPtr<RHIShader> VulkanContext::CreateGeometryShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refGeometryShader = Memory::MakeRef<VulkanGeometryShader>(desc);

		return refGeometryShader;
	}

	Memory::RefPtr<RHIShader> VulkanContext::CreateHullShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refHullShader = Memory::MakeRef<VulkanHullShader>(desc);

		return refHullShader;
	}

	Memory::RefPtr<RHIShader> VulkanContext::CreateDomainShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refDomainShader = Memory::MakeRef<VulkanDomainShader>(desc);

		return refDomainShader;
	}

	Memory::RefPtr<RHIShader> VulkanContext::CreatePixelShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refPixelShader = Memory::MakeRef<VulkanPixelShader>(desc);

		return refPixelShader;
	}

	Memory::RefPtr<RHIShader> VulkanContext::CreateComputeShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refComputeShader = Memory::MakeRef<VulkanComputeShader>(desc);

		return refComputeShader;
	}

	Memory::RefPtr<RHIPipeLine> VulkanContext::CreatePipeLine(const RHIPipeLineDesc desc)
	{
		Memory::RefPtr<RHIPipeLine> refPipeLine = Memory::MakeRef<VulkanPipeLine>(desc);

		return refPipeLine;
	}

	Memory::RefPtr<RHIRenderTarget> VulkanContext::CreateRenderTarget(const RHIRenderTargetDesc desc)
	{
		Memory::RefPtr<RHIRenderTarget> refRenderTarget = Memory::MakeRef<VulkanRenderTarget>(desc);

		return refRenderTarget;
	}

	void VulkanContext::InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!buffer)
		{
			LOGERROR() << "[Vk] Failed to initialize the buffer, the buffer is invalid";
			return;
		}

		if (!AllocateBuffer(info, buffer))
		{
			LOGERROR() << "[Vk] Failed to initialize the buffer, failed to allocate the buffer";

			buffer->SetState(eResourceState::eError);
		}
		else
		{			
			UpdateBuffer(info, buffer);

			buffer->SetState(eResourceState::eReady);
		}
	}

	void VulkanContext::InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{
		if (!texture)
		{
			LOGERROR() << "[Vk] Failed to initialize the texture, the texture is invalid";
			return;
		}

		if (!AllocateTexture(info, texture))
		{
			LOGERROR() << "[Vk] Failed to initialize the texture, failed to allocate the texture";

			texture->SetState(eResourceState::eError);
		}
		else
		{
			RHITextureUpdateDesc updateInfo = info;

			UpdateTexture(updateInfo, texture);

			texture->SetState(eResourceState::eReady);
		}
	}

	void VulkanContext::InitializeVertexLayout(const RHIVertexLayoutCreateDesc info, Memory::RefPtr<RHIVertexLayout> layout)
	{
		if (!layout)
		{
			LOGERROR() << "[Vk] Failed to initialize the vertex layout, the layout is invalid";
			return;
		}
		
		VulkanVertexLayout* rhiVertexLayout = static_cast<VulkanVertexLayout*>(layout->GetRawBuffer());
		if (!rhiVertexLayout)
		{
			LOGERROR() << "[Vk] Failed to initialize the vertex layout, the raw buffer is invalid";
			return;
		}

		for (const auto& [vertexKey, vertexStream] : info.streams)
		{
			const auto& attribute = vertexStream.attribute;
			const auto& binding = vertexStream.binding;
			const uint32_t bindingIndex = vertexStream.bindingIndex;

			VkVertexInputAttributeDescription attributeDesc{};
			attributeDesc.location = attribute.location;
			attributeDesc.binding = bindingIndex;
			attributeDesc.format = GetDataFormat(attribute.componentType);
			attributeDesc.offset = attribute.offset;

			VkVertexInputBindingDescription bindingDesc{};
			bindingDesc.binding = bindingIndex;
			bindingDesc.inputRate = GetVertexDivisor(binding.divisor);
			bindingDesc.stride = binding.stride;

			rhiVertexLayout->SetAttribute(vertexKey, attributeDesc);
			rhiVertexLayout->SetBinding(vertexKey, bindingDesc);
		}

		rhiVertexLayout->SetState(eResourceState::eReady);
	}

	void VulkanContext::InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler)
	{
		if (!m_device || !m_device->IsValid())
		{
			LOGERROR() << "[Vk] Failed to initialize the sampler, the device is invalid";
			return;
		}

		if (!sampler)
		{
			LOGERROR() << "[Vk] Failed to initialize the sampler, the sampler is invalid";
			return;
		}

		VulkanSampler* rhiSampler = static_cast<VulkanSampler*>(sampler->GetRawBuffer());
		if (!rhiSampler)
		{
			LOGERROR() << "[Vk] Failed to initialize the sampler, the raw buffer is invalid";
			return;
		}

		if (rhiSampler->GetSampler() != VK_NULL_HANDLE)
		{
			LOGWARN() << "[Vk] The sampler is already created";
			return;
		}

		const VkFilter magFilter = GetFilterMode(info.magFilter);
		const VkFilter minFilter = GetFilterMode(info.minFilter);
		const VkSamplerAddressMode wrapS = GetWrapMode(info.wrapS);
		const VkSamplerAddressMode wrapT = GetWrapMode(info.wrapT);
		const VkSamplerAddressMode wrapR = GetWrapMode(info.wrapR);
		const VkBorderColor borderColor = GetWrapBorderColor(info.wrapBorderColor);
		
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = magFilter;
		samplerInfo.minFilter = minFilter;
		samplerInfo.addressModeU = wrapS;
		samplerInfo.addressModeV = wrapT;
		samplerInfo.addressModeW = wrapR;
		samplerInfo.borderColor = borderColor;

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		VkSampler vkSampler;
		VkResult result = vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &vkSampler);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to create sampler, error code: " << result;
			return;
		}

		rhiSampler->SetSampler(vkSampler);
		rhiSampler->SetState(eResourceState::eReady);
	}

	void VulkanContext::InitializeShader(const RHIShaderCreateDesc info, Memory::RefPtr<RHIShader> shader)
	{
		if (!m_device || !m_device->IsValid())
		{
			LOGERROR() << "[Vk] Failed to initialize the shader, the device is invalid";
			return;
		}

		if (!shader || !info.data)
		{
			LOGERROR() << "[Vk] Failed to initialize the shader, the shader or the source code is invalid";
			return;
		}

		VulkanShader* vkShader = static_cast<VulkanShader*>(shader->GetRawBuffer());
		if (!vkShader)
		{
			LOGERROR() << "[Vk] Failed to initialize the shader, the shader is invalid";
			return;
		}

		if (vkShader->GetShaderModule() != VK_NULL_HANDLE)
		{
			LOGWARN() << "[Vk] The shader module is already created";
			return;
		}

		const DynamicArray<uint32_t> compiledSource = RHIUtils::CompileShader(info);
		if (compiledSource.Empty())
		{
			LOGERROR() << "[Vk] Failed to compile the shader source code";
			return;
		}

		VkShaderModuleCreateInfo moduleInfo{};
		moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleInfo.codeSize = compiledSource.Size() * sizeof(decltype(compiledSource)::ValueType);
		moduleInfo.pCode = compiledSource.Data();

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		VkShaderModule shaderModule;
		VkResult result = vkCreateShaderModule(logicalDevice, &moduleInfo, nullptr, &shaderModule);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to create shader module, error code: " << result;
			return;
		}

		RHIUtils::ReflectShader(compiledSource, shader);
		if (!CheckShaderVariable(shader))
		{
			LOGERROR() << "[Vk] The shader has wrong attribute location or naming";

			vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
			vkShader->SetState(eResourceState::eError);

			return;
		}

		vkShader->SetShaderModule(shaderModule);
		vkShader->SetState(eResourceState::eReady);
	}

	void VulkanContext::InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline)
	{
		if (!pipeline || !m_device)
		{
			return;
		}

		VulkanPipeLine* vkPipeline = static_cast<VulkanPipeLine*>(pipeline->GetRawBuffer());
		if (vkPipeline == nullptr)
		{
			return;
		}

		const bool compute = info.computeShader;
	}

	void VulkanContext::InitializeRenderTarget(const RHIRenderTargetCreateDesc info, Memory::RefPtr<RHIRenderTarget> target)
	{
	}

	void VulkanContext::UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!m_device || !m_device->IsValid() || !m_memory)
		{
			LOGERROR() << "[Vk] Device or memory is not valid, failed to update buffer";
			return;
		}

		if (!buffer || info.size == 0)
		{
			LOGERROR() << "[Vk] Buffer is null or size is zero, failed to update buffer";
			return;
		}

		VulkanBuffer* rhiBuffer = static_cast<VulkanBuffer*>(buffer->GetRawBuffer());
		if (!rhiBuffer || rhiBuffer->GetBuffer() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] VulkanBuffer is null or invalid, failed to update buffer";
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();
		const VulkanAllocation& allocation = rhiBuffer->GetAllocation();

		const bool useMapBuffer = (allocation.memoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;
		if (useMapBuffer)
		{
			SetBarrier(buffer, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

			const bool useFlush = ((allocation.memoryFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0);

			uint8_t* mappedBuffer = nullptr;

			VkResult result = vkMapMemory(logicalDevice, allocation.memory, allocation.offset, info.size, 0, reinterpret_cast<void**>(&mappedBuffer));
			if (result != VK_SUCCESS || mappedBuffer == nullptr)
			{
				LOGERROR() << "[Vk] Failed to map the buffer memory, error code: " << result;
				return;
			}

			for (const auto& dataRange : info.dataRanges)
			{
				if (!dataRange.data || dataRange.data->IsEmpty())
				{
					continue;
				}

				const void* dataPointer = dataRange.data->GetPointer();
				const size_t dataSize = dataRange.data->GetSize();
				if (dataSize + dataRange.offset[0] > buffer->GetSize())
				{
					continue;
				}

				char* offsetPointer = reinterpret_cast<char*>(mappedBuffer) + dataRange.offset[0];

				std::memcpy(offsetPointer, dataPointer, dataSize);
			}

			if (useFlush)
			{
				const VkPhysicalDeviceLimits& physicalLimits = m_device->GetPhysicalLimits();
				const VkDeviceSize nonCoherentAtomSize = physicalLimits.nonCoherentAtomSize;

				const VkDeviceSize alignedSize = (allocation.offset + allocation.size + nonCoherentAtomSize - 1) & ~(nonCoherentAtomSize - 1);
				const VkDeviceSize flushOffset = allocation.offset & ~(nonCoherentAtomSize - 1);
				const VkDeviceSize flushSize = alignedSize - flushOffset;

				VkMappedMemoryRange memoryRange{};
				memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
				memoryRange.pNext = nullptr;
				memoryRange.offset = flushOffset;
				memoryRange.size = flushSize;
				memoryRange.memory = allocation.memory;

				result = vkFlushMappedMemoryRanges(logicalDevice, 1, &memoryRange);
				if (result != VK_SUCCESS)
				{
					LOGERROR() << "[Vk] Failed to flush the map buffer";
					return;
				}
			}

			vkUnmapMemory(logicalDevice, allocation.memory);
		}
		else
		{
			const RHIBufferCreateDesc stagingInfo = GetStagingDesc(info);
			Memory::RefPtr<RHIBuffer> stagingBuffer = AcquireStaging(stagingInfo);
			CopyBuffer(stagingBuffer, stagingInfo, buffer, info);
			ReleaseStaging(stagingBuffer);
		}

		buffer->SetDesc(info);
	}

	void VulkanContext::UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture)
	{
		if (!m_device || !m_device->IsValid() || !m_memory)
		{
			LOGERROR() << "[Vk] Device or memory is not valid, failed to update texture";
			return;
		}

		if (!texture || info.faces.Empty() == 0)
		{
			LOGERROR() << "[Vk] Texture is null or size is zero, failed to update texture";
			return;
		}

		VulkanTexture* rhiTexture = static_cast<VulkanTexture*>(texture->GetRawBuffer());
		if (!rhiTexture || rhiTexture->GetImage() == VK_NULL_HANDLE || rhiTexture->GetImageView() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] VulkanTexture is null or invalid, failed to update texture";
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();
		const VulkanAllocation& allocation = rhiTexture->GetAllocation();

		const bool useMapBuffer = (allocation.memoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;
		if (useMapBuffer)
		{
			// TODO
			LOGWARN() << "[Vk] The texture is using map buffer, but the implementation is not done yet";
		}
		else
		{
			const RHIBufferCreateDesc stagingInfo = GetStagingDesc(info);
			Memory::RefPtr<RHIBuffer> stagingBuffer = AcquireStaging(stagingInfo);
			CopyBuffer(stagingBuffer, stagingInfo, texture, info);
			ReleaseStaging(stagingBuffer);
		}

		texture->SetDesc(info);
	}

	void VulkanContext::UpdateVertexLayout(const RHIVertexLayoutUpdateDesc info, Memory::RefPtr<RHIVertexLayout> layout)
	{
		// TODO: Implement this function if needed

		return;
	}

	void VulkanContext::ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!m_device || !m_device->IsValid() || !m_memory)
		{
			LOGERROR() << "[Vk] Device or memory is not valid, failed to resize buffer";
			return;
		}

		if (!buffer || info.size == 0)
		{
			LOGERROR() << "[Vk] Buffer is null or size is zero, failed to resize buffer";
			return;
		}

		VulkanBuffer* rhiBuffer = static_cast<VulkanBuffer*>(buffer->GetRawBuffer());
		if (!rhiBuffer)
		{
			LOGERROR() << "[Vk] VulkanBuffer is null, failed to resize buffer";
			return;
		}

		if (rhiBuffer->GetBuffer() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] VulkanBuffer is not created, failed to resize buffer";
			return;
		}

		RHIBufferUpdateDesc updateDesc = info;

		const bool neededResize = (updateDesc.size > rhiBuffer->GetSize()) || (updateDesc.size < (rhiBuffer->GetSize() / 4));
		if (neededResize)
		{
			Memory::RefPtr<RHIBuffer> newBuffer = CreateBuffer(updateDesc);
			VulkanBuffer* rhiNewBuffer = static_cast<VulkanBuffer*>(newBuffer->GetRawBuffer());
			if (!rhiNewBuffer)
			{
				LOGERROR() << "[Vk] Failed to create the new buffer for resizing";
				return;
			}

			if (AllocateBuffer(updateDesc, newBuffer))
			{
				UpdateBuffer(updateDesc, newBuffer);

				const VkBuffer oldVkBuffer = rhiBuffer->GetBuffer();
				const VkBuffer newVkBuffer = rhiNewBuffer->GetBuffer();

				const RHIBufferDesc oldDesc = rhiBuffer->GetDesc();
				const RHIBufferDesc newDesc = rhiNewBuffer->GetDesc();

				const VulkanAllocation oldAllocation = rhiBuffer->GetAllocation();
				const VulkanAllocation newAllocation = rhiNewBuffer->GetAllocation();

				rhiBuffer->SetDesc(newDesc);
				rhiBuffer->SetBuffer(newVkBuffer);
				rhiBuffer->SetAllocation(newAllocation);

				rhiNewBuffer->SetDesc(oldDesc);
				rhiNewBuffer->SetBuffer(oldVkBuffer);
				rhiNewBuffer->SetAllocation(oldAllocation);
			}
			else
			{
				LOGERROR() << "[Vk] Failed to allocate the new buffer for resizing";
			}

			RemoveBuffer(newBuffer);
		}
		else
		{
			UpdateBuffer(updateDesc, buffer);
		}
	}

	void VulkanContext::ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{
		if (!m_device || !m_device->IsValid() || !m_memory)
		{
			LOGERROR() << "[Vk] Device or memory is not valid, failed to resize buffer";
			return;
		}

		if (!texture || info.size == 0)
		{
			LOGERROR() << "[Vk] Texture is null or size is zero, failed to resize texture";
			return;
		}

		VulkanTexture* rhiTexture = static_cast<VulkanTexture*>(texture->GetRawBuffer());
		if (!rhiTexture)
		{
			LOGERROR() << "[Vk] VulkanTexture is null, failed to resize texture";
			return;
		}

		if (rhiTexture->GetImage() == VK_NULL_HANDLE || rhiTexture->GetImageView() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] VulkanTexture is not created, failed to resize texture";
			return;
		}

		const bool neededResize = (info.width != rhiTexture->GetWidth()) || (info.height != rhiTexture->GetHeight()) || (info.depth != rhiTexture->GetDepth());
		if (neededResize)
		{
			Memory::RefPtr<RHITexture> newTexture = CreateTexture(info);
			VulkanTexture* rhiNewTexture = static_cast<VulkanTexture*>(newTexture->GetRawBuffer());
			if (!rhiNewTexture)
			{
				LOGERROR() << "[Vk] Failed to create the new texture for resizing";
				return;
			}

			if (AllocateTexture(info, newTexture))
			{
				UpdateTexture(info, newTexture);

				const VkImage oldVkImage = rhiTexture->GetImage();
				const VkImageView oldVkImageView = rhiTexture->GetImageView();
				const VkImage newVkImage = rhiNewTexture->GetImage();
				const VkImageView newVkImageView = rhiNewTexture->GetImageView();

				const RHITextureDesc oldDesc = rhiTexture->GetDesc();
				const RHITextureDesc newDesc = rhiNewTexture->GetDesc();

				const VulkanAllocation oldAllocation = rhiTexture->GetAllocation();
				const VulkanAllocation newAllocation = rhiNewTexture->GetAllocation();

				rhiTexture->SetDesc(newDesc);
				rhiTexture->SetImage(newVkImage);
				rhiTexture->SetImageView(newVkImageView);
				rhiTexture->SetAllocation(newAllocation);

				rhiNewTexture->SetDesc(oldDesc);
				rhiNewTexture->SetImage(oldVkImage);
				rhiNewTexture->SetImageView(oldVkImageView);
				rhiNewTexture->SetAllocation(oldAllocation);
			}
			else
			{
				LOGERROR() << "[Vk] Failed to allocate the new texture for resizing";
			}

			RemoveTexture(newTexture);
		}
		else
		{
			UpdateTexture(info, texture);
		}
	}

	void VulkanContext::SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding, const uint32_t offset, const uint32_t range)
	{
		if (!buffer || !m_primary || m_primary->GetCommandBuffer() == VK_NULL_HANDLE)
		{
			LOGWARN() << "[Vk] Buffer or primary command buffer is null, failed to set the buffer";
			return;
		}
		if (!buffer)
		{
			LOGWARN() << "[Vk] Buffer is null, failed to set the buffer";
			return;
		}

		const VulkanBuffer* rhiBuffer = static_cast<const VulkanBuffer*>(buffer->GetRawBuffer());
		if (!rhiBuffer || rhiBuffer->GetBuffer() == VK_NULL_HANDLE)
		{
			LOGWARN() << "[Vk] VulkanBuffer is null or invalid, failed to set the buffer";
			return;
		}


	}

	void VulkanContext::SetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding)
	{
	}

	void VulkanContext::SetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding)
	{
	}

	void VulkanContext::SetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t bidingIndex, const uint32_t offset)
	{
		if (!buffer || !m_primary || m_primary->GetCommandBuffer() == VK_NULL_HANDLE)
		{
			LOGWARN() << "[Vk] Buffer or primary command buffer is null, failed to set the vertex buffer";
			return;
		}

		const VulkanBuffer* rhiBuffer = static_cast<const VulkanBuffer*>(buffer->GetRawBuffer());
		if (!rhiBuffer || rhiBuffer->GetBuffer() == VK_NULL_HANDLE)
		{
			LOGWARN() << "[Vk] VulkanBuffer is null or invalid, failed to set the vertex buffer";
			return;
		}

		SetBarrier(buffer, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);

		const VkBuffer vkBuffer = rhiBuffer->GetBuffer();
		VkCommandBuffer commandBuffer = m_primary->GetCommandBuffer();
		vkCmdBindVertexBuffers(commandBuffer, bidingIndex, 1, &vkBuffer, nullptr);
	}

	void VulkanContext::SetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset)
	{
		if (!buffer)
		{
			LOGWARN() << "[Vk] Buffer is null, failed to set the index buffer";
			return;
		}

		const VulkanBuffer* vkBuffer = static_cast<const VulkanBuffer*>(buffer->GetRawBuffer());
		if (!vkBuffer || vkBuffer->GetBuffer() == VK_NULL_HANDLE)
		{
			LOGWARN() << "[Vk] VulkanBuffer is null or invalid, failed to set the index buffer";
			return;
		}

		if (!m_primary || m_primary->GetCommandBuffer() == VK_NULL_HANDLE)
		{
			LOGWARN() << "[Vk] Primary command buffer is invalid, failed to set the index buffer";
			return;
		}

		VkCommandBuffer commandBuffer = m_primary->GetCommandBuffer();

		const RHIBufferDesc& desc = buffer->GetDesc();
		const VkIndexType indexType = GetIndexFormat(desc.type);
		const VkDeviceSize nativeOffset = static_cast<VkDeviceSize>(offset);

		SetBarrier(buffer, VK_ACCESS_INDEX_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT);

		vkCmdBindIndexBuffer(commandBuffer, vkBuffer->GetBuffer(),	nativeOffset, indexType);
	}

	void VulkanContext::SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
	{
		// nothing
	}

	void VulkanContext::SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
	{
	}

	void VulkanContext::UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding)
	{
		// No descriptor unbind in Vulkan - a slot just holds whatever was last written.
	}

	void VulkanContext::UnsetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding)
	{
		// See UnsetBuffer.
	}

	void VulkanContext::UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding)
	{
		// See UnsetBuffer.
	}

	void VulkanContext::UnsetVertexBuffer(const uint32_t bidingIndex)
	{
		// No vertex-buffer unbind in Vulkan - a slot just holds whatever was last bound.
	}

	void VulkanContext::UnsetIndexBuffer()
	{
		// See UnsetVertexBuffer.
	}

	void VulkanContext::UnsetVertexLayout()
	{
		// VertexLayout is baked into the bound Pipeline, not a separate bind target.
	}

	void VulkanContext::UnsetPipeLine()
	{
		// No unbind - a draw can't run without some pipeline bound anyway.
	}

	void VulkanContext::BeginRenderPass(Memory::RefPtr<const RHIRenderTarget> target)
	{
	}

	void VulkanContext::EndRenderPass()
	{
	}

	void VulkanContext::ClearRenderTarget(Memory::RefPtr<const RHIRenderTarget> target, const RHIClearState state)
	{
	}

	void VulkanContext::DispatchCompute(const RHIDispatchDesc info)
	{
	}

	void VulkanContext::DrawIndexPrimitive(const RHIDrawIndexDesc info)
	{
	}

	bool VulkanContext::AllocateBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!m_device || !m_device->IsValid() || !m_memory)
		{
			LOGERROR() << "[Vk] Device or memory is not valid, failed to allocate buffer";
			return false;
		}

		if (!buffer || info.size == 0)
		{
			LOGERROR() << "[Vk] Buffer is null or size is zero, failed to allocate buffer";
			return false;
		}

		VulkanBuffer* rhiBuffer = static_cast<VulkanBuffer*>(buffer->GetRawBuffer());
		if (!rhiBuffer)
		{
			LOGERROR() << "[Vk] VulkanBuffer is null, failed to allocate buffer";
			return false;
		}

		if (rhiBuffer->GetBuffer() != VK_NULL_HANDLE)
		{
			LOGWARN() << "[Vk] Buffer already has a valid VkBuffer, skipping the allocation";
			return true;
		}

		const VkBufferUsageFlags usageFlags = GetBufferUsage(info.usage);

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;
		bufferInfo.size = info.size;
		bufferInfo.usage = usageFlags;
		bufferInfo.queueFamilyIndexCount = 0;
		bufferInfo.pQueueFamilyIndices = nullptr;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		VkBuffer vkBuffer;
		VkResult result = vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &vkBuffer);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to create the buffer, error code: " << result;
			return false;
		}

		VkMemoryRequirements requirements;
		vkGetBufferMemoryRequirements(logicalDevice, vkBuffer, &requirements);

		const VkMemoryPropertyFlags properties = GetMemoryProperty(info.access, info.usage);

		VulkanAllocation allocation = m_memory->Allocate(m_device, requirements, properties);
		if (!allocation.IsValid())
		{
			LOGERROR() << "[Vk] Failed to allocate the buffer memory";

			vkDestroyBuffer(logicalDevice, vkBuffer, nullptr);

			return false;
		}

		result = vkBindBufferMemory(logicalDevice, vkBuffer, allocation.memory, allocation.offset);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to bind the buffer memory, error code: " << result;

			vkDestroyBuffer(logicalDevice, vkBuffer, nullptr);
			m_memory->Free(m_device, allocation);

			return false;
		}

		rhiBuffer->SetBuffer(vkBuffer);
		rhiBuffer->SetAllocation(allocation);

		return true;
	}

	bool VulkanContext::AllocateTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{
		if (!m_device || !m_device->IsValid() || !m_memory)
		{
			LOGERROR() << "[Vk] Device or memory is not valid, failed to allocate texture";
			return false;
		}

		if (!texture || info.width == 0 || info.height == 0 || info.depth == 0)
		{
			LOGERROR() << "[Vk] Texture is null or dimensions are zero, failed to allocate texture";
			return false;
		}

		VulkanTexture* rhiTexture = static_cast<VulkanTexture*>(texture->GetRawBuffer());
		if (!rhiTexture)
		{
			LOGERROR() << "[Vk] VulkanTexture is null, failed to allocate texture";
			return false;
		}

		if (rhiTexture->GetImage() != VK_NULL_HANDLE || rhiTexture->GetImageView() != VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] Texture already has a valid VkImage";
			return true;
		}

		const VkImageUsageFlags imageUsage = GetImageUsage(info.usage, info.generateMips);
		const VkImageCreateFlags imageCreate = GetImageCreate(info.dimension);
		const VkImageType imageType = GetImageType(info.dimension);
		const VkFormat imageFormat = GetImageFormat(info.format);
		const VkSampleCountFlags sampleCount = GetSampleCount(info.sampleCount);

		const VkMemoryPropertyFlags properties = GetMemoryProperty(info.access, info.usage);

		const bool hasVisible = (properties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;
		const bool hasHeight = (imageType == VK_IMAGE_TYPE_2D) || (imageType == VK_IMAGE_TYPE_3D);
		const bool hasDepth = (imageType == VK_IMAGE_TYPE_3D);
		const bool hasArray = (imageType != VK_IMAGE_TYPE_3D); // TODO : Texture Array

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.flags = imageCreate;
		imageInfo.imageType = imageType;
		imageInfo.format = imageFormat;
		imageInfo.usage = imageUsage;
		imageInfo.extent.width = info.width;
		imageInfo.extent.height = hasHeight ? info.height : 1;
		imageInfo.extent.depth = hasDepth ? info.depth : 1;
		imageInfo.arrayLayers = hasArray ? info.face : 1;
		imageInfo.mipLevels = info.mipLevels;
		imageInfo.samples = static_cast<VkSampleCountFlagBits>(sampleCount);
		imageInfo.tiling = hasVisible ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = hasVisible ? VK_IMAGE_LAYOUT_PREINITIALIZED : VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		VkImage vkImage;
		VkResult result = vkCreateImage(logicalDevice, &imageInfo, nullptr, &vkImage);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to create the image, error code: " << result;
			return false;
		}

		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(logicalDevice, vkImage, &requirements);

		VulkanAllocation allocation = m_memory->Allocate(m_device, requirements, properties);
		if (!allocation.IsValid())
		{
			LOGERROR() << "[Vk] Failed to allocate the image memory";

			vkDestroyImage(logicalDevice, vkImage, nullptr);
			return false;
		}

		result = vkBindImageMemory(logicalDevice, vkImage, allocation.memory, allocation.offset);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to bind the image memory, error code: " << result;

			vkDestroyImage(logicalDevice, vkImage, nullptr);
			m_memory->Free(m_device, allocation);

			return false;
		}

		const VkImageViewType imageViewType = GetImageViewType(info.dimension);
		const VkImageAspectFlags imageAspect = GetImageAspect(info.format);

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = vkImage;
		viewInfo.viewType = imageViewType;
		viewInfo.format = imageFormat;
		viewInfo.subresourceRange.aspectMask = imageAspect;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = info.mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = imageInfo.arrayLayers;

		VkImageView vkImageView;
		result = vkCreateImageView(logicalDevice, &viewInfo, nullptr, &vkImageView);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to create the image view, error code: " << result;

			vkDestroyImage(logicalDevice, vkImage, nullptr);
			m_memory->Free(m_device, allocation);

			return false;
		}

		rhiTexture->SetImage(vkImage);
		rhiTexture->SetImageView(vkImageView);
		rhiTexture->SetAllocation(allocation);

		return true;
	}

	const RHIBufferCreateDesc VulkanContext::GetStagingDesc(const RHIBufferCreateDesc info)
	{
		RHIBufferCreateDesc stagingDesc{};
		stagingDesc.access = eDataAccess::eStream;
		stagingDesc.usage = eBufferUsage::eTransferSrc;
		stagingDesc.dataRanges.Reserve(info.dataRanges.Size());

		uint32_t offset = 0;
		for (const auto& dataRange : info.dataRanges)
		{
			if (!dataRange.data)
			{
				continue;
			}

			RHIDataRange<1> currRange = dataRange;
			currRange.offset[0] = offset;

			offset += currRange.data->GetSize();

			stagingDesc.dataRanges.PushBack(currRange);
		}

		stagingDesc.size = offset;

		return stagingDesc;
	}

	const RHIBufferCreateDesc VulkanContext::GetStagingDesc(const RHITextureCreateDesc info)
	{
		RHIBufferCreateDesc stagingDesc{};
		stagingDesc.access = eDataAccess::eStream;
		stagingDesc.usage = eBufferUsage::eTransferSrc;

		uint32_t offset = 0;
		for (const auto& face : info.faces)
		{
			for (const auto& mipmap : face.mipMaps)
			{
				if (!mipmap.data)
				{
					continue;
				}

				RHIDataRange<1> currRange;
				currRange.data = mipmap.data;
				currRange.offset[0] = offset;

				offset += currRange.data->GetSize();

				stagingDesc.dataRanges.PushBack(currRange);
			}
		}

		stagingDesc.size = offset;

		return stagingDesc;
	}

	Memory::RefPtr<RHIBuffer> VulkanContext::AcquireStaging(const RHIBufferCreateDesc info)
	{
		if (!m_device || !m_device->IsValid() || !m_memory)
		{
			LOGERROR() << "[Vk] Device or memory is not valid, failed to get staging buffer";
			return nullptr;
		}

		Memory::RefPtr<RHIBuffer> stagingBuffer = nullptr;		
		for (auto itr = m_staging.begin(); itr != m_staging.end(); itr++)
		{
			Memory::RefPtr<RHIBuffer> buffer = *itr;
			if (buffer && buffer->GetSize() >= info.size)
			{
				stagingBuffer = buffer;
				m_staging.Erase(itr);
				break;
			}
		}

		if (!stagingBuffer)
		{
			stagingBuffer = CreateBuffer(info);
			AllocateBuffer(info, stagingBuffer);
		}
		
		UpdateBuffer(info, stagingBuffer);

		return stagingBuffer;
	}

	void VulkanContext::ReleaseStaging(Memory::RefPtr<RHIBuffer> staging)
	{
		if (!staging)
		{
			LOGWARN() << "[Vk] Staging buffer is null, nothing to release";
			return;
		}

		m_releaseStaging.PushBack(staging);
	}

	void VulkanContext::CopyBuffer(Memory::RefPtr<const RHIBuffer> src, const RHIBufferCreateDesc srcInfo, Memory::RefPtr<RHIBuffer> dest, const RHIBufferCreateDesc destInfo)
	{
		if (!m_device || !m_device->IsValid() || !m_primary)
		{
			LOGERROR() << "[Vk] Device or primary command is not valid, failed to copy the buffer";
			return;
		}

		if (!src || !dest)
		{
			LOGWARN() << "[Vk] Source or destination buffer is null, failed to copy the buffer";
			return;
		}

		if (srcInfo.dataRanges.Empty() || destInfo.dataRanges.Empty() || srcInfo.dataRanges.Size() != destInfo.dataRanges.Size())
		{
			LOGWARN() << "[Vk] Source/destination data ranges are empty or mismatched in count, failed to copy the buffer";
			return;
		}

		const VulkanBuffer* rhiSrc = static_cast<const VulkanBuffer*>(src->GetRawBuffer());
		VulkanBuffer* rhiDest = static_cast<VulkanBuffer*>(dest->GetRawBuffer());
		if (!rhiSrc || rhiSrc->GetBuffer() == VK_NULL_HANDLE ||
			!rhiDest || rhiDest->GetBuffer() == VK_NULL_HANDLE)
		{
			LOGWARN() << "[Vk] VulkanBuffer is null or invalid, failed to copy the buffer";
			return;
		}

		wtr::DynamicArray<VkBufferCopy> regions(srcInfo.dataRanges.Size());
		for (size_t index = 0; index < srcInfo.dataRanges.Size(); index++)
		{
			const auto& stagingRange = srcInfo.dataRanges[index];
			const auto& bufferRange = destInfo.dataRanges[index];
			if (!stagingRange.data || !bufferRange.data)
			{
				continue;
			}

			auto& region = regions[index];
			region.size = stagingRange.data->GetSize();
			region.srcOffset = stagingRange.offset[0];
			region.dstOffset = bufferRange.offset[0];
		}

		VkBuffer vkSrc = rhiSrc->GetBuffer();
		VkBuffer vkDest = rhiDest->GetBuffer();
		VkCommandBuffer commandBuffer = m_primary->GetCommandBuffer();

		SetBarrier(src, VK_ACCESS_TRANSFER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
		SetBarrier(dest, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

		vkCmdCopyBuffer(commandBuffer, vkSrc, vkDest, regions.Size(), regions.Data());
	}

	void VulkanContext::CopyBuffer(Memory::RefPtr<const RHIBuffer> src, const RHIBufferCreateDesc srcInfo, Memory::RefPtr<RHITexture> dest, const RHITextureCreateDesc destInfo)
	{
		if (!m_device || !m_device->IsValid() || !m_primary)
		{
			LOGERROR() << "[Vk] Device or primary command is not valid, failed to copy the texture";
			return;
		}

		if (!src || !dest)
		{
			LOGWARN() << "[Vk] Source or destination buffer is null, failed to copy the texture";
			return;
		}

		if (srcInfo.dataRanges.Empty() || destInfo.faces.Empty())
		{
			LOGWARN() << "[Vk] Source data ranges or destination faces are empty, failed to copy the texture";
			return;
		}

		const VulkanBuffer* rhiSrc = static_cast<const VulkanBuffer*>(src->GetRawBuffer());
		VulkanTexture* rhiDest = static_cast<VulkanTexture*>(dest->GetRawBuffer());
		if (!rhiSrc || rhiSrc->GetBuffer() == VK_NULL_HANDLE ||
			!rhiDest || rhiDest->GetImage() == VK_NULL_HANDLE || rhiDest->GetImageView() == VK_NULL_HANDLE)
		{
			LOGWARN() << "[Vk] VulkanBuffer or VulkanTexture is null or invalid, failed to copy the texture";
			return;
		}

		const VkImageAspectFlags aspectMask = GetImageAspect(destInfo.format);

		wtr::DynamicArray<VkBufferImageCopy> regions;
		size_t rangeIndex = 0;
		for (const auto& face : destInfo.faces)
		{
			for (const auto& mipmap : face.mipMaps)
			{
				if (!mipmap.data)
				{
					continue;
				}

				if (rangeIndex >= srcInfo.dataRanges.Size())
				{
					LOGWARN() << "[Vk] Source data ranges ran out before all destination mipmaps were matched, failed to copy the texture";
					return;
				}

				const auto& stagingRange = srcInfo.dataRanges[rangeIndex];
				rangeIndex++;

				if (!stagingRange.data)
				{
					continue;
				}

				const uint32_t mipWidth = (destInfo.width >> mipmap.level) > 0 ? (destInfo.width >> mipmap.level) : 1;
				const uint32_t mipHeight = (destInfo.height >> mipmap.level) > 0 ? (destInfo.height >> mipmap.level) : 1;
				const uint32_t mipDepth = (destInfo.depth >> mipmap.level) > 0 ? (destInfo.depth >> mipmap.level) : 1;

				VkBufferImageCopy region{};
				region.bufferOffset = stagingRange.offset[0];
				region.bufferRowLength = 0;
				region.bufferImageHeight = 0;
				region.imageSubresource.aspectMask = aspectMask;
				region.imageSubresource.mipLevel = mipmap.level;
				region.imageSubresource.baseArrayLayer = face.index;
				region.imageSubresource.layerCount = 1;
				region.imageOffset = { 0, 0, 0 };
				region.imageExtent = { mipWidth, mipHeight, mipDepth };

				regions.PushBack(region);
			}
		}

		if (regions.Empty())
		{
			LOGWARN() << "[Vk] No valid regions to copy, failed to copy the texture";
			return;
		}

		VkCommandBuffer commandBuffer = m_primary->GetCommandBuffer();

		SetBarrier(src, VK_ACCESS_TRANSFER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
		SetBarrier(dest, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		vkCmdCopyBufferToImage(commandBuffer, rhiSrc->GetBuffer(), rhiDest->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, regions.Size(), regions.Data());
	}

	void VulkanContext::SetBarrier(Memory::RefPtr<const RHIBuffer> buffer, const VkAccessFlags dstAccess, const VkPipelineStageFlags dstStage)
	{
		if (!m_primary || m_primary->GetCommandBuffer() == VK_NULL_HANDLE || !buffer)
		{
			return;
		}

		const VulkanBuffer* rhiBuffer = static_cast<const VulkanBuffer*>(buffer->GetRawBuffer());
		if (!rhiBuffer || rhiBuffer->GetBuffer() == VK_NULL_HANDLE)
		{
			return;
		}

		const VkAccessFlags srcAccess = rhiBuffer->GetAccessFlag();
		const VkPipelineStageFlags srcStage = rhiBuffer->GetStageFlag();
		if (srcAccess == dstAccess && srcStage == dstStage)
		{
			return;
		}

		VkBufferMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		barrier.buffer = rhiBuffer->GetBuffer();
		barrier.srcAccessMask = srcAccess;
		barrier.dstAccessMask = dstAccess;
		barrier.size = VK_WHOLE_SIZE;
		barrier.offset = 0;
		barrier.pNext = nullptr;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		VkImageMemoryBarrier imageBarrier{};

		VkCommandBuffer commandBuffer = m_primary->GetCommandBuffer();
		vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 1, &barrier, 0, nullptr);

		rhiBuffer->SetAccessFlag(dstAccess);
		rhiBuffer->SetStageFlag(dstStage);
	}

	void VulkanContext::SetBarrier(Memory::RefPtr<const RHITexture> texture, const VkAccessFlags dstAccess, const VkPipelineStageFlags dstStage, const VkImageLayout newLayout)
	{
		if (!texture)
		{
			return;
		}

		const VulkanTexture* rhiTexture = static_cast<const VulkanTexture*>(texture->GetRawBuffer());
		if (!rhiTexture || rhiTexture->GetImage() == VK_NULL_HANDLE || rhiTexture->GetImageView() == VK_NULL_HANDLE)
		{
			return;
		}

		const VkAccessFlags srcAccess = rhiTexture->GetAccessFlag();
		const VkPipelineStageFlags srcStage = rhiTexture->GetStageFlag();
		const VkImageLayout oldLayout = rhiTexture->GetImageLayout();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = rhiTexture->GetImage();
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.srcAccessMask = srcAccess;
		barrier.dstAccessMask = dstAccess;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		VkCommandBuffer commandBuffer = m_primary->GetCommandBuffer();
		vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		rhiTexture->SetAccessFlag(dstAccess);
		rhiTexture->SetStageFlag(dstStage);
		rhiTexture->SetImageLayout(newLayout);
	}

	VkAccessFlags VulkanContext::GetAccessFlags(const eBindingType type, const eBindingUsage usage) const
	{
		VkAccessFlags accessFlags = 0x00;

		if (eBindingUsage::eRead == usage)
		{
			if (eBindingType::eUniform == type ||
				eBindingType::eStorage == type ||
				eBindingType::eSampler == type)
			{
				accessFlags |= VK_ACCESS_UNIFORM_READ_BIT;
			}
		}

		if (eBindingUsage::eWrite == usage)
		{
			if (eBindingType::eStorage == type || 
				eBindingType::eSampler == type)
			{
				accessFlags |= VK_ACCESS_SHADER_WRITE_BIT;
			}
		}

		return accessFlags;
	}

	VkPipelineStageFlags VulkanContext::GetStageFlags(const eBindingType type, const eBindingUsage usage) const
	{
		VkPipelineStageFlags stageFlags = 0x00;
		if (eBindingUsage::eRead == usage)
		{
			if (eBindingType::eUniform == type ||
				eBindingType::eStorage == type ||
				eBindingType::eSampler == type)
			{
				stageFlags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | 
					VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT |
					VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT |
					VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT |
					VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | 
					VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
			}
		}
		if (eBindingUsage::eWrite == usage)
		{
			if (eBindingType::eStorage == type ||
				eBindingType::eSampler == type)
			{
				stageFlags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
			}
		}
		return stageFlags;
	}

	VkBufferUsageFlags VulkanContext::GetBufferUsage(const eBufferUsage usage) const
	{
		VkBufferUsageFlags usageFlags = 0x00;
		if (eBufferUsage::eNone != (usage & eBufferUsage::eTransferSrc))
		{
			usageFlags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}

		if (eBufferUsage::eNone != (usage & eBufferUsage::eTransferDst))
		{
			usageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		}

		if (eBufferUsage::eNone != (usage & eBufferUsage::eVertex))
		{
			usageFlags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}

		if (eBufferUsage::eNone != (usage & eBufferUsage::eIndex))
		{
			usageFlags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}

		if (eBufferUsage::eNone != (usage & eBufferUsage::eUniform))
		{
			usageFlags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}

		if (eBufferUsage::eNone != (usage & eBufferUsage::eStorage))
		{
			usageFlags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}

		if (eBufferUsage::eNone != (usage & eBufferUsage::eIndirect))
		{
			usageFlags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		}

		return usageFlags;
	}

	VkMemoryPropertyFlags VulkanContext::GetMemoryProperty(const eDataAccess access, const eBufferUsage usage) const
	{
		// 바인딩 비트(Vertex/Index/Uniform/Storage/Indirect)가 하나도 없고 전송 비트만 있는 버퍼는
		// 파이프라인이 직접 읽지 않는 순수 중계용 버퍼 - 즉 staging / readback 이다.
		// 방향(Src/Dst)에 따라 CPU 쪽 캐시 정책을 다르게 잡아준다.
		constexpr eBufferUsage bindingMask = eBufferUsage::eVertex | eBufferUsage::eIndex
			| eBufferUsage::eUniform | eBufferUsage::eStorage | eBufferUsage::eIndirect;

		const bool isBound = eBufferUsage::eNone != (usage & bindingMask);
		if (!isBound)
		{
			// staging : CPU 가 쓰고 GPU 가 읽어감 -> write-combine 이 유리하므로 COHERENT
			if (eBufferUsage::eNone != (usage & eBufferUsage::eTransferSrc))
			{
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			}

			// readback : GPU 가 쓰고 CPU 가 읽어감 -> 캐시된 읽기가 유리하므로 CACHED
			if (eBufferUsage::eNone != (usage & eBufferUsage::eTransferDst))
			{
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
			}
		}

		if (eDataAccess::eDynamic == access || eDataAccess::eStream == access)
		{
			return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}

		return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	}

	VkMemoryPropertyFlags VulkanContext::GetMemoryProperty(const eDataAccess access, const eTextureUsage usage) const
	{
		constexpr eTextureUsage bindingMask = eTextureUsage::eSampled | eTextureUsage::eRenderTarget
			| eTextureUsage::eDepthStencil | eTextureUsage::ePresent;

		const bool isBound = eTextureUsage::eNone != (usage & bindingMask);
		if (!isBound)
		{
			if (eTextureUsage::eNone != (usage & eTextureUsage::eTransferSrc))
			{
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			}

			if (eTextureUsage::eNone != (usage & eTextureUsage::eTransferDst))
			{
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
			}
		}

		// 샘플링/렌더타겟으로 쓰이는 이미지는 갱신 빈도와 무관하게 OPTIMAL 타일링이어야 하므로
		// 항상 DEVICE_LOCAL 로 두고, 갱신은 staging 버퍼 경유 복사로 처리한다.
		return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	}

	VkFormat VulkanContext::GetDataFormat(const eDataType data) const
	{
		if (eDataType::eByte == data)
		{
			return VK_FORMAT_R8_SINT;
		}
		else if (eDataType::eUByte == data)
		{
			return VK_FORMAT_R8_UINT;
		}
		else if (eDataType::eShort == data)
		{
			return VK_FORMAT_R16_SINT;
		}
		else if (eDataType::eUShort == data)
		{
			return VK_FORMAT_R16_UINT;
		}
		else if (eDataType::eInt == data)
		{
			return VK_FORMAT_R32_SINT;
		}
		else if (eDataType::eUInt == data)
		{
			return VK_FORMAT_R32_UINT;
		}
		else if (eDataType::eFloat == data)
		{
			return VK_FORMAT_R32_SFLOAT;
		}
		else if (eDataType::eDouble == data)
		{
			return VK_FORMAT_R64_SFLOAT;
		}
		else
		{
			return VK_FORMAT_UNDEFINED;
		}
	}

	VkFormat VulkanContext::GetVertexFormat(const eDataType data, const uint32_t count, const bool normalized) const
	{
		const VkFormat baseFormat = GetDataFormat(data);
		if (VK_FORMAT_UNDEFINED == baseFormat || count < 1 || count > 4)
		{
			return VK_FORMAT_UNDEFINED;
		}

		const uint32_t index = count - 1;

		if (VK_FORMAT_R8_UINT == baseFormat)
		{
			static constexpr VkFormat table[4][2] =
			{
				{ VK_FORMAT_R8_UINT,       VK_FORMAT_R8_UNORM },
				{ VK_FORMAT_R8G8_UINT,     VK_FORMAT_R8G8_UNORM },
				{ VK_FORMAT_R8G8B8_UINT,   VK_FORMAT_R8G8B8_UNORM },
				{ VK_FORMAT_R8G8B8A8_UINT, VK_FORMAT_R8G8B8A8_UNORM },
			};

			return table[index][normalized ? 1 : 0];
		}
		else if (VK_FORMAT_R8_SINT == baseFormat)
		{
			static constexpr VkFormat table[4][2] =
			{
				{ VK_FORMAT_R8_SINT,       VK_FORMAT_R8_SNORM },
				{ VK_FORMAT_R8G8_SINT,     VK_FORMAT_R8G8_SNORM },
				{ VK_FORMAT_R8G8B8_SINT,   VK_FORMAT_R8G8B8_SNORM },
				{ VK_FORMAT_R8G8B8A8_SINT, VK_FORMAT_R8G8B8A8_SNORM },
			};

			return table[index][normalized ? 1 : 0];
		}
		else if (VK_FORMAT_R16_UINT == baseFormat)
		{
			static constexpr VkFormat table[4][2] =
			{
				{ VK_FORMAT_R16_UINT,          VK_FORMAT_R16_UNORM },
				{ VK_FORMAT_R16G16_UINT,       VK_FORMAT_R16G16_UNORM },
				{ VK_FORMAT_R16G16B16_UINT,    VK_FORMAT_R16G16B16_UNORM },
				{ VK_FORMAT_R16G16B16A16_UINT, VK_FORMAT_R16G16B16A16_UNORM },
			};

			return table[index][normalized ? 1 : 0];
		}
		else if (VK_FORMAT_R16_SINT == baseFormat)
		{
			static constexpr VkFormat table[4][2] =
			{
				{ VK_FORMAT_R16_SINT,          VK_FORMAT_R16_SNORM },
				{ VK_FORMAT_R16G16_SINT,       VK_FORMAT_R16G16_SNORM },
				{ VK_FORMAT_R16G16B16_SINT,    VK_FORMAT_R16G16B16_SNORM },
				{ VK_FORMAT_R16G16B16A16_SINT, VK_FORMAT_R16G16B16A16_SNORM },
			};

			return table[index][normalized ? 1 : 0];
		}
		else if (VK_FORMAT_R32_UINT == baseFormat)
		{
			// Vulkan has no 32-bit normalized integer format - UNORM/SNORM stop at 16-bit.
			// A normalized request here is a caller mistake, not something to silently coerce.
			if (normalized)
			{
				return VK_FORMAT_UNDEFINED;
			}

			static constexpr VkFormat table[4] =
			{
				VK_FORMAT_R32_UINT, 
				VK_FORMAT_R32G32_UINT, 
				VK_FORMAT_R32G32B32_UINT, 
				VK_FORMAT_R32G32B32A32_UINT,
			};

			return table[index];
		}
		else if (VK_FORMAT_R32_SINT == baseFormat)
		{
			if (normalized)
			{
				return VK_FORMAT_UNDEFINED;
			}

			static constexpr VkFormat table[4] =
			{
				VK_FORMAT_R32_SINT, 
				VK_FORMAT_R32G32_SINT, 
				VK_FORMAT_R32G32B32_SINT, 
				VK_FORMAT_R32G32B32A32_SINT,
			};

			return table[index];
		}
		else if (VK_FORMAT_R32_SFLOAT == baseFormat)
		{
			// normalized is meaningless for a float source - always ignored.
			static constexpr VkFormat table[4] =
			{
				VK_FORMAT_R32_SFLOAT, 
				VK_FORMAT_R32G32_SFLOAT, 
				VK_FORMAT_R32G32B32_SFLOAT, 
				VK_FORMAT_R32G32B32A32_SFLOAT,
			};

			return table[index];
		}
		else if (VK_FORMAT_R64_SFLOAT == baseFormat)
		{
			// Same as R32_SFLOAT - no normalized variant exists, and normalized is meaningless anyway.
			static constexpr VkFormat table[4] =
			{
				VK_FORMAT_R64_SFLOAT, 
				VK_FORMAT_R64G64_SFLOAT, 
				VK_FORMAT_R64G64B64_SFLOAT, 
				VK_FORMAT_R64G64B64A64_SFLOAT,
			};

			return table[index];
		}
		else
		{
			return VK_FORMAT_UNDEFINED;
		}
	}

	VkVertexInputRate VulkanContext::GetVertexDivisor(const eDataDivisor divisor) const
	{
		if (eDataDivisor::ePerVertex == divisor)
		{
			return VK_VERTEX_INPUT_RATE_VERTEX;
		}
		else if (eDataDivisor::ePerInstance == divisor)
		{
			return VK_VERTEX_INPUT_RATE_INSTANCE;
		}
		else
		{
			return VK_VERTEX_INPUT_RATE_MAX_ENUM;
		}
	}

	VkIndexType VulkanContext::GetIndexFormat(const eDataType data) const
	{
		if (eDataType::eUShort == data)
		{
			return VK_INDEX_TYPE_UINT16;
		}
		else if (eDataType::eUInt == data)
		{
			return VK_INDEX_TYPE_UINT32;
		}
		else
		{
			return VK_INDEX_TYPE_NONE_KHR;
		}
	}

	VkImageAspectFlags VulkanContext::GetImageAspect(const ePixelFormat pixel) const
	{
		if (ePixelFormat::eD32 == pixel)
		{
			return VK_IMAGE_ASPECT_DEPTH_BIT;
		}
		else if (ePixelFormat::eD24_S8 == pixel)
		{
			return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		else
		{
			return VK_IMAGE_ASPECT_COLOR_BIT;
		}
	}

	VkImageAspectFlags VulkanContext::GetImageAspect(const eAttachment attach) const
	{
		if (eAttachment::eColor == attach)
		{
			return VK_IMAGE_ASPECT_COLOR_BIT;
		}
		else if (eAttachment::eDepth == attach)
		{
			return VK_IMAGE_ASPECT_DEPTH_BIT;
		}
		else if (eAttachment::eStencil == attach)
		{
			return VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		else if (eAttachment::eDepthStencil == attach)
		{
			return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		else
		{
			return 0;
		}
	}

	VkImageUsageFlags VulkanContext::GetImageUsage(const eTextureUsage usage, const bool mipmaps) const
	{
		VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		if ((usage & eTextureUsage::eSampled) != eTextureUsage::eNone)
		{
			usageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT;
		}

		if ((usage & eTextureUsage::eRenderTarget) != eTextureUsage::eNone)
		{
			usageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}

		if ((usage & eTextureUsage::eDepthStencil) != eTextureUsage::eNone)
		{
			usageFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		}

		if (mipmaps)
		{
			usageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		return usageFlags;
	}

	VkImageCreateFlags VulkanContext::GetImageCreate(const eTextureDimension type) const
	{
		if (type == eTextureDimension::eTextureCube || type == eTextureDimension::eTextureCubeArray)
		{
			return VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		}

		return VK_IMAGE_CREATE_FLAG_BITS_MAX_ENUM;
	}

	VkImageViewType VulkanContext::GetImageViewType(const eTextureDimension texture) const
	{
		if (eTextureDimension::eTexture1D == texture)
		{
			return VK_IMAGE_VIEW_TYPE_1D;
		}
		else if (eTextureDimension::eTexture2D == texture)
		{
			return VK_IMAGE_VIEW_TYPE_2D;
		}
		else if (eTextureDimension::eTexture3D == texture)
		{
			return VK_IMAGE_VIEW_TYPE_3D;
		}
		else if (eTextureDimension::eTextureCube == texture)
		{
			return VK_IMAGE_VIEW_TYPE_CUBE;
		}
		else if (eTextureDimension::eTexture1DArray == texture)
		{
			return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
		}
		else if (eTextureDimension::eTexture2DArray == texture)
		{
			return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
		}
		else if (eTextureDimension::eTextureCubeArray == texture)
		{
			return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
		}
		else if (eTextureDimension::eTextureMultisample == texture)
		{
			// Vulkan has no dedicated multisample view type - sample count lives on
			// VkImageCreateInfo::samples instead, the view type is still plain 2D/2D_ARRAY.
			return VK_IMAGE_VIEW_TYPE_2D;
		}
		else if (eTextureDimension::eTextureMultisampleArray == texture)
		{
			return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
		}
		else
		{
			return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
		}
	}

	VkImageType VulkanContext::GetImageType(const eTextureDimension texture) const
	{
		if (eTextureDimension::eTexture1D == texture || eTextureDimension::eTexture1DArray == texture)
		{
			return VK_IMAGE_TYPE_1D;
		}
		else if (eTextureDimension::eTexture2D == texture ||
			eTextureDimension::eTextureCube == texture ||
			eTextureDimension::eTexture2DArray == texture ||
			eTextureDimension::eTextureCubeArray == texture ||
			eTextureDimension::eTextureMultisample == texture ||
			eTextureDimension::eTextureMultisampleArray == texture)
		{
			return VK_IMAGE_TYPE_2D;
		}
		else if (eTextureDimension::eTexture3D == texture)
		{
			return VK_IMAGE_TYPE_3D;
		}
		else
		{
			return VK_IMAGE_TYPE_MAX_ENUM;
		}
	}

	VkFormat VulkanContext::GetImageFormat(const ePixelFormat pixel) const
	{
		if (ePixelFormat::eR8_UNorm == pixel)
		{
			return VK_FORMAT_R8_UNORM;
		}
		else if (ePixelFormat::eR8G8_UNorm == pixel)
		{
			return VK_FORMAT_R8G8_UNORM;
		}
		else if (ePixelFormat::eR8G8B8_UNorm == pixel)
		{
			return VK_FORMAT_R8G8B8_UNORM;
		}
		else if (ePixelFormat::eR8G8B8A8_UNorm == pixel)
		{
			return VK_FORMAT_R8G8B8A8_UNORM;
		}
		else if (ePixelFormat::eR16_UNorm == pixel)
		{
			return VK_FORMAT_R16_UNORM;
		}
		else if (ePixelFormat::eR16G16_UNorm == pixel)
		{
			return VK_FORMAT_R16G16_UNORM;
		}
		else if (ePixelFormat::eR16G16B16_UNorm == pixel)
		{
			return VK_FORMAT_R16G16B16_UNORM;
		}
		else if (ePixelFormat::eR16G16B16A16_UNorm == pixel)
		{
			return VK_FORMAT_R16G16B16A16_UNORM;
		}
		else if (ePixelFormat::eR8G8B8A8_sRGB == pixel)
		{
			return VK_FORMAT_R8G8B8A8_SRGB;
		}
		else if (ePixelFormat::eR8_UInt == pixel)
		{
			return VK_FORMAT_R8_UINT;
		}
		else if (ePixelFormat::eR8G8_UInt == pixel)
		{
			return VK_FORMAT_R8G8_UINT;
		}
		else if (ePixelFormat::eR8G8B8_UInt == pixel)
		{
			return VK_FORMAT_R8G8B8_UINT;
		}
		else if (ePixelFormat::eR8G8B8A8_UInt == pixel)
		{
			return VK_FORMAT_R8G8B8A8_UINT;
		}
		else if (ePixelFormat::eR16_UInt == pixel)
		{
			return VK_FORMAT_R16_UINT;
		}
		else if (ePixelFormat::eR16G16_UInt == pixel)
		{
			return VK_FORMAT_R16G16_UINT;
		}
		else if (ePixelFormat::eR16G16B16_UInt == pixel)
		{
			return VK_FORMAT_R16G16B16_UINT;
		}
		else if (ePixelFormat::eR16G16B16A16_UInt == pixel)
		{
			return VK_FORMAT_R16G16B16A16_UINT;
		}
		else if (ePixelFormat::eR32_UInt == pixel)
		{
			return VK_FORMAT_R32_UINT;
		}
		else if (ePixelFormat::eR32G32_UInt == pixel)
		{
			return VK_FORMAT_R32G32_UINT;
		}
		else if (ePixelFormat::eR32G32B32_UInt == pixel)
		{
			return VK_FORMAT_R32G32B32_UINT;
		}
		else if (ePixelFormat::eR32G32B32A32_UInt == pixel)
		{
			return VK_FORMAT_R32G32B32A32_UINT;
		}
		else if (ePixelFormat::eR16_Float == pixel)
		{
			return VK_FORMAT_R16_SFLOAT;
		}
		else if (ePixelFormat::eR16G16_Float == pixel)
		{
			return VK_FORMAT_R16G16_SFLOAT;
		}
		else if (ePixelFormat::eR16G16B16_Float == pixel)
		{
			return VK_FORMAT_R16G16B16_SFLOAT;
		}
		else if (ePixelFormat::eR16G16B16A16_Float == pixel)
		{
			return VK_FORMAT_R16G16B16A16_SFLOAT;
		}
		else if (ePixelFormat::eR32_Float == pixel)
		{
			return VK_FORMAT_R32_SFLOAT;
		}
		else if (ePixelFormat::eR32G32_Float == pixel)
		{
			return VK_FORMAT_R32G32_SFLOAT;
		}
		else if (ePixelFormat::eR32G32B32_Float == pixel)
		{
			return VK_FORMAT_R32G32B32_SFLOAT;
		}
		else if (ePixelFormat::eR32G32B32A32_Float == pixel)
		{
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		}
		else if (ePixelFormat::eD24_S8 == pixel)
		{
			return VK_FORMAT_D24_UNORM_S8_UINT;
		}
		else if (ePixelFormat::eD32 == pixel)
		{
			return VK_FORMAT_D32_SFLOAT;
		}
		else
		{
			return VK_FORMAT_UNDEFINED;
		}
	}

	VkSampleCountFlags VulkanContext::GetSampleCount(const uint32_t count) const
	{
		if (count == 1)
		{
			return VK_SAMPLE_COUNT_1_BIT;
		}
		else if (count == 2)
		{
			return VK_SAMPLE_COUNT_2_BIT;
		}
		else if (count == 4)
		{
			return VK_SAMPLE_COUNT_4_BIT;
		}
		else if (count == 8)
		{
			return VK_SAMPLE_COUNT_8_BIT;
		}
		else if (count == 16)
		{
			return VK_SAMPLE_COUNT_16_BIT;
		}
		else if (count == 32)
		{
			return VK_SAMPLE_COUNT_32_BIT;
		}
		else if (count == 64)
		{
			return VK_SAMPLE_COUNT_64_BIT;
		}
		else
		{
			return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
		}
	}

	VkFilter VulkanContext::GetFilterMode(const eFilterMode filter) const
	{
		if (eFilterMode::eNearest == filter)
		{
			return VK_FILTER_NEAREST;
		}
		else if (eFilterMode::eLinear == filter)
		{
			return VK_FILTER_LINEAR;
		}
		else
		{
			return VK_FILTER_NEAREST;
		}
	}

	VkSamplerAddressMode VulkanContext::GetWrapMode(const eWrapMode wrap) const
	{
		if (eWrapMode::eRepeat == wrap)
		{
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
		else if (eWrapMode::eMirroredRepeat == wrap)
		{
			return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		}
		else if (eWrapMode::eClampToEdge == wrap)
		{
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		}
		else if (eWrapMode::eClampToBorder == wrap)
		{
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		}
		else
		{
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
	}

	VkBorderColor VulkanContext::GetWrapBorderColor(const eWrapBorderColor wrapBorderColor) const
	{
		if (eWrapBorderColor::eTransparentBlack == wrapBorderColor)
		{
			return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		}
		else if (eWrapBorderColor::eOpaqueBlack == wrapBorderColor)
		{
			return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
		}
		else if (eWrapBorderColor::eOpaqueWhite == wrapBorderColor)
		{
			return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		}
		else
		{
			return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		}
	}

	VkCullModeFlags VulkanContext::GetCullFace(const eCullFace face) const
	{
		if (eCullFace::eFront == face)
		{
			return VK_CULL_MODE_FRONT_BIT;
		}
		else if (eCullFace::eBack == face)
		{
			return VK_CULL_MODE_BACK_BIT;
		}
		else if (eCullFace::eFront_Back == face)
		{
			return VK_CULL_MODE_FRONT_AND_BACK;
		}
		else
		{
			return VK_CULL_MODE_NONE;
		}
	}

	VkFrontFace VulkanContext::GetFrontFace(const eFrontFace face) const
	{
		if (eFrontFace::eCCW == face)
		{
			return VK_FRONT_FACE_COUNTER_CLOCKWISE;
		}
		else if (eFrontFace::eCW == face)
		{
			return VK_FRONT_FACE_CLOCKWISE;
		}
		else
		{
			return VK_FRONT_FACE_COUNTER_CLOCKWISE;
		}
	}

	VkPrimitiveTopology VulkanContext::GetDrawMode(const eDrawMode mode) const
	{
		if (eDrawMode::ePointList == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		}
		else if (eDrawMode::eLineList == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		}
		else if (eDrawMode::eLineStrip == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		}
		else if (eDrawMode::eTriangleList == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
		else if (eDrawMode::eTriangleStrip == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		}
		else if (eDrawMode::eTriangleFan == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
		}
		else
		{
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}

	VkPolygonMode VulkanContext::GetPrimitiveMode(const ePrimitiveMode mode) const
	{
		if (ePrimitiveMode::ePoint == mode)
		{
			return VK_POLYGON_MODE_POINT;
		}
		else if (ePrimitiveMode::eLine == mode)
		{
			return VK_POLYGON_MODE_LINE;
		}
		else if (ePrimitiveMode::eFill == mode)
		{
			return VK_POLYGON_MODE_FILL;
		}
		else
		{
			return VK_POLYGON_MODE_FILL;
		}
	}

	VkCompareOp VulkanContext::GetCompareFunc(const eCompareFunc func) const
	{
		if (eCompareFunc::eNever == func)
		{
			return VK_COMPARE_OP_NEVER;
		}
		else if (eCompareFunc::eEqual == func)
		{
			return VK_COMPARE_OP_EQUAL;
		}
		else if (eCompareFunc::eNotEqual == func)
		{
			return VK_COMPARE_OP_NOT_EQUAL;
		}
		else if (eCompareFunc::eLess == func)
		{
			return VK_COMPARE_OP_LESS;
		}
		else if (eCompareFunc::eLessEqual == func)
		{
			return VK_COMPARE_OP_LESS_OR_EQUAL;
		}
		else if (eCompareFunc::eGreater == func)
		{
			return VK_COMPARE_OP_GREATER;
		}
		else if (eCompareFunc::eGreaterEqual == func)
		{
			return VK_COMPARE_OP_GREATER_OR_EQUAL;
		}
		else if (eCompareFunc::eAlways == func)
		{
			return VK_COMPARE_OP_ALWAYS;
		}
		else
		{
			return VK_COMPARE_OP_ALWAYS;
		}
	}

	VkStencilOp VulkanContext::GetStencilOp(const eStencilOp op) const
	{
		if (eStencilOp::eKeep == op)
		{
			return VK_STENCIL_OP_KEEP;
		}
		else if (eStencilOp::eZero == op)
		{
			return VK_STENCIL_OP_ZERO;
		}
		else if (eStencilOp::eReplace == op)
		{
			return VK_STENCIL_OP_REPLACE;
		}
		else if (eStencilOp::eInc_Clamp == op)
		{
			return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
		}
		else if (eStencilOp::eDec_Clamp == op)
		{
			return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		}
		else if (eStencilOp::eInvert == op)
		{
			return VK_STENCIL_OP_INVERT;
		}
		else if (eStencilOp::eInc_Wrap == op)
		{
			return VK_STENCIL_OP_INCREMENT_AND_WRAP;
		}
		else if (eStencilOp::eDec_Wrap == op)
		{
			return VK_STENCIL_OP_DECREMENT_AND_WRAP;
		}
		else
		{
			return VK_STENCIL_OP_KEEP;
		}
	}

	VkBlendFactor VulkanContext::GetBlendFunc(const eBlendFunc func) const
	{
		if (eBlendFunc::eZero == func)
		{
			return VK_BLEND_FACTOR_ZERO;
		}
		else if (eBlendFunc::eOne == func)
		{
			return VK_BLEND_FACTOR_ONE;
		}
		else if (eBlendFunc::eSrc_Color == func)
		{
			return VK_BLEND_FACTOR_SRC_COLOR;
		}
		else if (eBlendFunc::eOne_Minus_Src_Color == func)
		{
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		}
		else if (eBlendFunc::eDst_Color == func)
		{
			return VK_BLEND_FACTOR_DST_COLOR;
		}
		else if (eBlendFunc::eOne_Minus_Dst_Color == func)
		{
			return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		}
		else if (eBlendFunc::eSrc_Alpha == func)
		{
			return VK_BLEND_FACTOR_SRC_ALPHA;
		}
		else if (eBlendFunc::eOne_Minus_Src_Alpha == func)
		{
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		}
		else if (eBlendFunc::eDst_Alpha == func)
		{
			return VK_BLEND_FACTOR_DST_ALPHA;
		}
		else if (eBlendFunc::eOne_Minus_Dst_Alpha == func)
		{
			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		}
		else
		{
			return VK_BLEND_FACTOR_ONE;
		}
	}

	VkBlendOp VulkanContext::GetBlendOp(const eBlendOp op) const
	{
		if (eBlendOp::eAdd == op)
		{
			return VK_BLEND_OP_ADD;
		}
		else if (eBlendOp::eSub == op)
		{
			return VK_BLEND_OP_SUBTRACT;
		}
		else if (eBlendOp::eReverse_Sub == op)
		{
			return VK_BLEND_OP_REVERSE_SUBTRACT;
		}
		else if (eBlendOp::eMin == op)
		{
			return VK_BLEND_OP_MIN;
		}
		else if (eBlendOp::eMax == op)
		{
			return VK_BLEND_OP_MAX;
		}
		else
		{
			return VK_BLEND_OP_ADD;
		}
	}

	VkShaderStageFlagBits VulkanContext::GetShaderType(const eShaderType type) const
	{
		// NOTE: GLContext::GetShaderType() maps eHull -> GL_TESS_EVALUATION_SHADER and never
		// handles eDomain at all. That looks backwards (Hull/TCS runs pre-tessellator, Domain/TES
		// runs post-tessellator) and is worth a second look on the GL side - this function maps
		// both correctly rather than mirroring that.
		if (eShaderType::eVertex == type)
		{
			return VK_SHADER_STAGE_VERTEX_BIT;
		}
		else if (eShaderType::eGeometry == type)
		{
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		}
		else if (eShaderType::eHull == type)
		{
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		}
		else if (eShaderType::eDomain == type)
		{
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		}
		else if (eShaderType::ePixel == type)
		{
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		}
		else if (eShaderType::eCompute == type)
		{
			return VK_SHADER_STAGE_COMPUTE_BIT;
		}
		else
		{
			return static_cast<VkShaderStageFlagBits>(0);
		}
	}

	void VulkanContext::FlushBuffer()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		auto itr = m_pendingBuffers.begin();
		while (itr != m_pendingBuffers.end())
		{
			auto& buffer = *itr;
			if (!buffer)
			{
				itr = m_pendingBuffers.Erase(itr);
				continue;
			}

			auto* refData = buffer.GetRefData();
			if (refData->GetRefCount() > 1)
			{
				itr++;
				continue;
			}

			VulkanBuffer* vkBuffer = static_cast<VulkanBuffer*>(buffer->GetRawBuffer());
			if (vkBuffer)
			{
				vkDestroyBuffer(logicalDevice, vkBuffer->GetBuffer(), nullptr);
				vkDestroyBufferView(logicalDevice, vkBuffer->GetBufferView(), nullptr);

				const VulkanAllocation& allocation = vkBuffer->GetAllocation();

				m_memory->Free(m_device, allocation);

				vkBuffer->SetAllocation(VulkanAllocation::Null);
				vkBuffer->SetState(eResourceState::eNone);
			}

			itr = m_pendingBuffers.Erase(itr);
		}
	}

	void VulkanContext::FlushVertexLayout()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		auto itr = m_pendingVertexLayouts.begin();
		while (itr != m_pendingVertexLayouts.end())
		{
			auto& layout = *itr;
			if (!layout)
			{
				itr = m_pendingVertexLayouts.Erase(itr);
				continue;
			}

			auto* refData = layout.GetRefData();
			if (refData->GetRefCount() > 1)
			{
				itr++;
				continue;
			}

			VulkanVertexLayout* vkLayout = static_cast<VulkanVertexLayout*>(layout->GetRawBuffer());
			if (vkLayout)
			{
				vkLayout->SetState(eResourceState::eNone);
			}

			itr = m_pendingVertexLayouts.Erase(itr);
		}
	}

	void VulkanContext::FlushTexture()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		auto itr = m_pendingTextures.begin();
		while (itr != m_pendingTextures.end())
		{
			auto& texture = *itr;
			if (!texture)
			{
				itr = m_pendingTextures.Erase(itr);
				continue;
			}

			auto* refData = texture.GetRefData();
			if (refData->GetRefCount() > 1)
			{
				itr++;
				continue;
			}

			VulkanTexture* vkTexture = static_cast<VulkanTexture*>(texture->GetRawBuffer());
			if (vkTexture)
			{
				vkDestroyImage(logicalDevice, vkTexture->GetImage(), nullptr);
				vkDestroyImageView(logicalDevice, vkTexture->GetImageView(), nullptr);

				const VulkanAllocation& allocation = vkTexture->GetAllocation();
				m_memory->Free(m_device, allocation);

				vkTexture->SetAllocation(VulkanAllocation::Null);
				vkTexture->SetState(eResourceState::eNone);
			}

			itr = m_pendingTextures.Erase(itr);
		}
	}

	void VulkanContext::FlushSampler()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		// TODO: Flush the sampler resources
	}

	void VulkanContext::FlushShader()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		// TODO: Flush the shader resources
	}

	void VulkanContext::FlushPipeline()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		// TODO: Flush the pipeline resources
	}

	void VulkanContext::FlushRenderTarget()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		// TODO: Flush the render target resources
	}
}
