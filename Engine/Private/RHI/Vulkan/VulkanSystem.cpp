#include <RHI/Vulkan/VulkanSystem.h>

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
	VulkanSystem::VulkanSystem()
		: RHISystem(eRenderType::eVulkan)
		, m_device()
		, m_surface()
		, m_memory()
		, m_primary()
		, m_acquire(VK_NULL_HANDLE)
		, m_present(VK_NULL_HANDLE)
		, m_fence(VK_NULL_HANDLE)
		, m_imageIndex(UINT32_MAX)
	{
	}

	VulkanSystem::~VulkanSystem()
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

	bool VulkanSystem::Init(void* nativeHandle)
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

		if (!m_surface->InitSwapChain(nativeHandle, *this, m_device))
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

	void VulkanSystem::FlushPending()
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
		FlushRenderPass();
	}

	void VulkanSystem::FlushBuffer()
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

	void VulkanSystem::FlushVertexLayout()
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
				//vkDestroyPipelineLayout(logicalDevice, vkLayout->GetPipelineLayout(), nullptr);

				vkLayout->SetState(eResourceState::eNone);
			}
			itr = m_pendingVertexLayouts.Erase(itr);
		}
	}

	void VulkanSystem::FlushTexture()
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

	void VulkanSystem::FlushSampler()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();
	}

	void VulkanSystem::FlushShader()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();
	}

	void VulkanSystem::FlushPipeline()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();
	}

	void VulkanSystem::FlushRenderTarget()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();
	}
	
	void VulkanSystem::FlushRenderPass()
	{
		if (!m_memory || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		auto itr = m_pendingRenderPasses.begin();
		while (itr != m_pendingRenderPasses.end())
		{
			auto& renderPass = *itr;
			if (!renderPass)
			{
				itr = m_pendingRenderPasses.Erase(itr);
				continue;
			}


		}
	}

	void VulkanSystem::Clear(const RHIClearState& state)
	{
	}

	void VulkanSystem::BeginContext()
	{
	}

	void VulkanSystem::EndContext()
	{
		if (!m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();
		vkDeviceWaitIdle(logicalDevice);
	}

	void VulkanSystem::BeginFrame()
	{
		if (!m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE ||
			!m_surface || m_surface->GetSwapChain() == VK_NULL_HANDLE ||
			!m_primary || m_primary->GetCommandBuffer() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();
		VkSwapchainKHR swapChain = m_surface->GetSwapChain();
		VkCommandBuffer commandBuffer = m_primary->GetCommandBuffer();

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

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext = nullptr;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
	}

	void VulkanSystem::EndFrame()
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

	void VulkanSystem::Resize(const uint32_t width, const uint32_t height)
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

	void VulkanSystem::Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height)
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

	Memory::RefPtr<RHIBuffer> VulkanSystem::CreateBuffer(const RHIBufferDesc desc)
	{
		Memory::RefPtr<RHIBuffer> refBuffer = Memory::MakeRef<VulkanBuffer>(desc);

		return refBuffer;
	}

	Memory::RefPtr<RHIVertexLayout> VulkanSystem::CreateVertexLayout(const RHIVertexLayoutDesc desc)
	{
		Memory::RefPtr<RHIVertexLayout> refLayout = Memory::MakeRef<VulkanVertexLayout>(desc);

		return refLayout;
	}

	Memory::RefPtr<RHITexture> VulkanSystem::CreateTexture(const RHITextureDesc desc)
	{
		Memory::RefPtr<RHITexture> refTexture = Memory::MakeRef<VulkanTexture>(desc);

		return refTexture;
	}

	Memory::RefPtr<RHISampler> VulkanSystem::CreateSampler(const RHISamplerDesc desc)
	{
		Memory::RefPtr<RHISampler> refSampler = Memory::MakeRef<VulkanSampler>(desc);

		return refSampler;
	}

	Memory::RefPtr<RHIShader> VulkanSystem::CreateVertexShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refVertexShader = Memory::MakeRef<VulkanVertexShader>(desc);

		return refVertexShader;
	}

	Memory::RefPtr<RHIShader> VulkanSystem::CreateGeometryShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refGeometryShader = Memory::MakeRef<VulkanGeometryShader>(desc);

		return refGeometryShader;
	}

	Memory::RefPtr<RHIShader> VulkanSystem::CreateHullShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refHullShader = Memory::MakeRef<VulkanHullShader>(desc);

		return refHullShader;
	}

	Memory::RefPtr<RHIShader> VulkanSystem::CreateDomainShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refDomainShader = Memory::MakeRef<VulkanDomainShader>(desc);

		return refDomainShader;
	}

	Memory::RefPtr<RHIShader> VulkanSystem::CreatePixelShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refPixelShader = Memory::MakeRef<VulkanPixelShader>(desc);

		return refPixelShader;
	}

	Memory::RefPtr<RHIShader> VulkanSystem::CreateComputeShader(const RHIShaderDesc desc)
	{
		Memory::RefPtr<RHIShader> refComputeShader = Memory::MakeRef<VulkanComputeShader>(desc);

		return refComputeShader;
	}

	Memory::RefPtr<RHIPipeLine> VulkanSystem::CreatePipeLine(const RHIPipeLineDesc desc)
	{
		Memory::RefPtr<RHIPipeLine> refPipeLine = Memory::MakeRef<VulkanPipeLine>(desc);

		return refPipeLine;
	}

	Memory::RefPtr<RHIRenderTarget> VulkanSystem::CreateRenderTarget(const RHIRenderTargetDesc desc)
	{
		Memory::RefPtr<RHIRenderTarget> refRenderTarget = Memory::MakeRef<VulkanRenderTarget>(desc);

		return refRenderTarget;
	}

	Memory::RefPtr<RHIRenderPass> VulkanSystem::CreateRenderPass(const RHIRenderPassDesc desc)
	{
		Memory::RefPtr<RHIRenderPass> refRenderPass = Memory::MakeRef<VulkanRenderPass>(desc);

		return refRenderPass;
	}

	void VulkanSystem::InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!buffer || !m_device || !m_device->GetLogicalDevice() || !m_memory)
		{
			return;
		}

		// eBufferType     -> primary binding role bit, i.e. which pipeline stage/fixed-function
		//                    unit is allowed to read (or write, for storage buffers) this buffer
		//                    during a draw/dispatch:
		//   eVertex   -> VERTEX_BUFFER_BIT      : read by the vertex-input-assembly stage
		//   eIndex    -> INDEX_BUFFER_BIT       : read by the index-fetch stage
		//   eConst    -> UNIFORM_BUFFER_BIT     : read by shader stages via a uniform binding
		//   eStorage  -> STORAGE_BUFFER_BIT     : read/written by shader stages via SSBO binding
		//   eIndirect -> INDIRECT_BUFFER_BIT    : read by the command processor for draw/dispatch args
		//   eTexel    -> UNIFORM/STORAGE_TEXEL_BUFFER_BIT : read via a texel buffer view
		//   eNone     -> no role bit at all - this buffer is never bound for a draw/dispatch,
		//                only used by the copy/transfer engine (see eBufferTransfer below)
		//
		// eBufferTransfer -> additive bit for the COPY engine specifically (vkCmdCopyBuffer and
		//                    friends), completely independent of the role bit above - binding a
		//                    buffer as VERTEX_BUFFER_BIT does NOT also make it eligible as a copy
		//                    argument, each participation has to be declared separately:
		//   eSource      -> TRANSFER_SRC_BIT : buffer may be passed as srcBuffer to a copy command
		//                    (the transfer engine reads FROM this buffer's memory)
		//   eDestination -> TRANSFER_DST_BIT : buffer may be passed as dstBuffer to a copy command
		//                    (the transfer engine writes INTO this buffer's memory)
		//   both may be combined (e.g. a scratch/intermediate buffer copied into and back out of)
		//
		// Combined: VkBufferUsageFlags = GetBufferType(bufferType) | GetBufferTransfer(bufferTransfer)
		//
		// | eBufferType | eBufferTransfer | Result                                | Typical case                                    |
		// |-------------|------------------|----------------------------------------|---------------------------------------------------|
		// | eNone       | eSource          | TRANSFER_SRC_BIT                      | staging (upload) buffer - copy engine reads it     |
		// | eNone       | eDestination     | TRANSFER_DST_BIT                      | readback buffer - copy engine writes it            |
		// | eVertex     | eNone            | VERTEX_BUFFER_BIT                     | eStatic - never touched again after upload         |
		// | eVertex     | eDestination     | VERTEX_BUFFER_BIT | TRANSFER_DST_BIT | eDynamic / eStream(large) - re-staged via copy     |
		// | eVertex     | eSource          | VERTEX_BUFFER_BIT | TRANSFER_SRC_BIT | debug/tooling readback of this buffer's own data   |
		// | eIndex/eConst/eStorage/eIndirect/eTexel | eNone/eDestination/eSource | matching role bit [| TRANSFER_*_BIT] | same pattern as eVertex above |

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		VulkanBuffer* vkBuffer = static_cast<VulkanBuffer*>(buffer->GetRawBuffer());
		if (!vkBuffer)
		{
			return;
		}

		VkBufferUsageFlags usageFlags = GetBufferType(info.bufferType) | GetBufferTransfer(info.bufferTransfer);

		// VkBufferCreateInfo
		// shading Mode
		//	1. VK_SHADING_MODE_EXCLUSIVE : Only one queue family at a time can access the buffer.
		//  2. VK_SHADING_MODE_CONCURRENT : Multiple queue families can access the buffer without explicit ownership transfers.
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;
		bufferInfo.size = info.size;
		bufferInfo.usage = usageFlags;
		bufferInfo.queueFamilyIndexCount = 0;
		bufferInfo.pQueueFamilyIndices = nullptr;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer bufferInstance;
		VkResult result = vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &bufferInstance);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to create the buffer, error code: " << result;
			return;
		}

		VkMemoryRequirements requiremenets;
		vkGetBufferMemoryRequirements(logicalDevice, bufferInstance, &requiremenets);

		VkMemoryPropertyFlags properties = GetBufferProperty(info.accessType, info.bufferTransfer);
		
		VulkanAllocation allocation = m_memory->Allocate(m_device, requiremenets, properties);
		bool allocated = allocation.IsValid();
		if (!allocated)
		{
			properties = GetBufferProperty(eDataAccess::eStaging, eBufferTransfer::eSource);

			allocation = m_memory->Allocate(m_device, requiremenets, properties);

			allocated = allocation.IsValid();
		}

		if (!allocated)
		{
			LOGERROR() << "[Vk] Failed to allocate the buffer memory";

			vkDestroyBuffer(logicalDevice, bufferInstance, nullptr);

			vkBuffer->SetAllocation(VulkanAllocation::Null);
			vkBuffer->SetBuffer(VK_NULL_HANDLE);
			vkBuffer->SetState(eResourceState::eError);

			return;
		}

		result = vkBindBufferMemory(logicalDevice, bufferInstance, allocation.memory, allocation.offset);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to bind the buffer memory, error code: " << result;

			vkDestroyBuffer(logicalDevice, bufferInstance, nullptr);

			m_memory->Free(m_device, allocation);

			vkBuffer->SetAllocation(VulkanAllocation::Null);
			vkBuffer->SetBuffer(VK_NULL_HANDLE);
			vkBuffer->SetState(eResourceState::eError);
			
			return;
		}

		vkBuffer->SetAllocation(allocation);
		vkBuffer->SetBuffer(bufferInstance);
		vkBuffer->SetState(eResourceState::eReady);

		// TODO - Texel Buffer View
	}

	void VulkanSystem::InitializeVertexLayout(const RHIVertexLayoutCreateDesc info, Memory::RefPtr<RHIVertexLayout> layout)
	{
	}

	void VulkanSystem::InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{
	}

	void VulkanSystem::InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler)
	{
	}

	void VulkanSystem::InitializeShader(const RHIShaderCreateDesc info, Memory::RefPtr<RHIShader> shader)
	{
		if (!shader || !info.data || !m_device)
		{
			return;
		}

		VulkanShader* vkShader = static_cast<VulkanShader*>(shader->GetRawBuffer());
		if (!vkShader)
		{
			return;
		}

		const DynamicArray<uint32_t> compiledSource = RHIUtils::CompileShader(info);
		if (compiledSource.Empty())
		{
			return;
		}

		VkShaderModuleCreateInfo moduleInfo{};
		moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleInfo.codeSize = compiledSource.Size() * sizeof(decltype(compiledSource)::ValueType);
		moduleInfo.pCode = compiledSource.Data();

		VkShaderModule shaderModule;
		VkResult result = vkCreateShaderModule(m_device->GetLogicalDevice(), &moduleInfo, nullptr, &shaderModule);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to create shader module, error code: " << result;
			return;
		}

		RHIUtils::ReflectShader(compiledSource, shader);
		if (!CheckShaderVariable(shader))
		{
			LOGERROR() << "[Vk] The shader has wrong attribute location or naming";

			vkDestroyShaderModule(m_device->GetLogicalDevice(), shaderModule, nullptr);
			vkShader->SetState(eResourceState::eError);

			return;
		}

		vkShader->SetShaderModule(shaderModule);
		vkShader->SetState(eResourceState::eReady);
	}

	void VulkanSystem::InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline)
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

	void VulkanSystem::InitializeRenderTarget(const RHIRenderTargetCreateDesc info, Memory::RefPtr<RHIRenderTarget> target)
	{
	}

	void VulkanSystem::InitializeRenderPass(const RHIRenderPassCreateDesc info, Memory::RefPtr<RHIRenderPass> pass)
	{
	}

	void VulkanSystem::UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (!buffer || info.dataRanges.Empty() || !m_device || m_device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		if (info.size > buffer->GetSize())
		{
			LOGERROR() << "[Vk] Buffer size is smaller than the data size, resizing the buffer";
			return;
		}

		VkDevice logicalDevice = m_device->GetLogicalDevice();

		VulkanBuffer* vkBuffer = static_cast<VulkanBuffer*>(buffer->GetRawBuffer());
		if (!vkBuffer || vkBuffer->GetBuffer() == VK_NULL_HANDLE)
		{
			return;
		}

		VkBuffer bufferInstance = vkBuffer->GetBuffer();
		if (bufferInstance == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] Buffer instance is invalid";
			return;
		}

		const VulkanAllocation& allocation = vkBuffer->GetAllocation();

		const bool useMapBuffer = (info.mapAccess != eMapAccess::eNone) && ((allocation.memoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0);
		if (useMapBuffer)
		{
			const bool useFlush = ((allocation.memoryFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0);

			uint8_t* mappedBuffer = nullptr;

			VkResult result = vkMapMemory(logicalDevice, allocation.memory, allocation.offset, 
				info.size, VK_MEMORY_MAP_PLACED_BIT_EXT, reinterpret_cast<void**>(&mappedBuffer));

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
				if (dataSize + dataRange.offset > buffer->GetSize())
				{
					continue;
				}

				char* offsetPointer = reinterpret_cast<char*>(mappedBuffer) + dataRange.offset;

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
				}
			}

			vkUnmapMemory(logicalDevice, allocation.memory);
		}
		else
		{

		}
	}

	void VulkanSystem::UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture)
	{
	}

	void VulkanSystem::UpdateVertexLayout(const RHIVertexLayoutUpdateDesc info, Memory::RefPtr<RHIVertexLayout> layout)
	{
	}

	void VulkanSystem::ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
	}

	void VulkanSystem::ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{
	}

	void VulkanSystem::SetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const RHIConstDesc info, const std::string& slotName)
	{
	}

	void VulkanSystem::SetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const std::string& slotName)
	{
	}

	void VulkanSystem::SetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const std::string& slotName)
	{
	}

	void VulkanSystem::SetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const std::string& slotName)
	{
	}

	void VulkanSystem::UnsetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const std::string& slotName)
	{
	}

	void VulkanSystem::UnsetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const std::string& slotName)
	{
	}

	void VulkanSystem::UnsetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const std::string& slotName)
	{
	}

	void VulkanSystem::UnsetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const std::string& slotName)
	{
	}

	void VulkanSystem::SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
	{
	}

	void VulkanSystem::SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
	{
	}

	void VulkanSystem::SetRenderTarget(Memory::RefPtr<const RHIRenderTarget> target)
	{
	}

	void VulkanSystem::SetRenderPass(Memory::RefPtr<const RHIRenderPass> pass)
	{
	}

	void VulkanSystem::ClearRenderTarget(Memory::RefPtr<const RHIRenderTarget> target, const RHIClearState& state)
	{
	}

	void VulkanSystem::UnsetVertexLayout()
	{
	}

	void VulkanSystem::UnsetPipeLine()
	{
	}

	void VulkanSystem::UnsetRenderTarget()
	{
	}

	void VulkanSystem::UnsetRenderPass()
	{
	}

	void VulkanSystem::DispatchCompute(const RHIDispatchDesc info)
	{
	}

	void VulkanSystem::DrawIndexPrimitive(const RHIDrawIndexDesc info)
	{
	}

	VkBufferUsageFlags VulkanSystem::GetBufferType(const eBufferType bufferType) const
	{
		VkBufferUsageFlags usageFlags = 0x00;
		if (eBufferType::eVertex == bufferType)
		{
			usageFlags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}
		else if (eBufferType::eIndex == bufferType)
		{
			usageFlags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}
		else if (eBufferType::eConst == bufferType)
		{
			usageFlags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}
		else if (eBufferType::eStorage == bufferType)
		{
			usageFlags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}
		else if (eBufferType::eIndirect == bufferType)
		{
			usageFlags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		}
		else if (eBufferType::eTexel == bufferType)
		{
			// A texel buffer can back either a uniform texel view or a storage texel view -
			// eBufferType alone doesn't say which, so both usage bits are set and the actual
			// VkBufferView's usage is decided at view-creation time.
			usageFlags |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
		}
		else
		{
			// nothing
		}

		return usageFlags;
	}

	VkBufferUsageFlags VulkanSystem::GetBufferTransfer(const eBufferTransfer bufferTransfer) const
	{
		VkBufferUsageFlags usageFlags = 0x00;
		if (eBufferTransfer::eSource == bufferTransfer)
		{
			usageFlags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}
		
		if (eBufferTransfer::eDestination == bufferTransfer)
		{
			usageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		}

		return usageFlags;
	}

	VkMemoryPropertyFlags VulkanSystem::GetBufferProperty(const eDataAccess dataAccess,const eBufferTransfer bufferTransfer) const
	{
		if (eDataAccess::eStatic == dataAccess)
		{
			return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}
		else if (eDataAccess::eDynamic == dataAccess || eDataAccess::eStream == dataAccess)
		{
			return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}
		else if (eDataAccess::eStaging == dataAccess)
		{
			if (eBufferTransfer::eSource == bufferTransfer)
			{
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			}
			else if (eBufferTransfer::eDestination == bufferTransfer)
			{
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
			}
			else
			{
				// nothing
			}
		}
		else
		{
			// nothing
		}

		return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	}

	VkFormat VulkanSystem::GetDataType(const eDataType data) const
	{
		// Single-component scalar format - the building block a vertex attribute's real
		// VkFormat (e.g. VK_FORMAT_R32G32B32_SFLOAT) is chosen from once component count is
		// also known, mirroring how GLSystem::GetDataType() returns a bare GL_FLOAT/GL_UNSIGNED_BYTE
		// token rather than a fully-formed vertex attribute descriptor.
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

	VkIndexType VulkanSystem::GetIndexType(const eDataType data) const
	{
		// Unlike GL's glDrawElements (which accepts the same type token for any integer width),
		// vkCmdBindIndexBuffer only accepts these two widths - there is no 8/32-bit VkIndexType
		// without the separate VK_EXT_index_type_uint8 extension.
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

	VkImageViewType VulkanSystem::GetTextureType(const eTextureType texture) const
	{
		if (eTextureType::eTexture1D == texture)
		{
			return VK_IMAGE_VIEW_TYPE_1D;
		}
		else if (eTextureType::eTexture2D == texture)
		{
			return VK_IMAGE_VIEW_TYPE_2D;
		}
		else if (eTextureType::eTexture3D == texture)
		{
			return VK_IMAGE_VIEW_TYPE_3D;
		}
		else if (eTextureType::eTextureCube == texture)
		{
			return VK_IMAGE_VIEW_TYPE_CUBE;
		}
		else if (eTextureType::eTexture1DArray == texture)
		{
			return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
		}
		else if (eTextureType::eTexture2DArray == texture)
		{
			return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
		}
		else if (eTextureType::eTextureCubeArray == texture)
		{
			return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
		}
		else if (eTextureType::eTextureMultisample == texture)
		{
			// Vulkan has no dedicated multisample view type - sample count lives on
			// VkImageCreateInfo::samples instead, the view type is still plain 2D/2D_ARRAY.
			return VK_IMAGE_VIEW_TYPE_2D;
		}
		else if (eTextureType::eTextureMultisampleArray == texture)
		{
			return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
		}
		else
		{
			return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
		}
	}

	VkImageType VulkanSystem::GetTextureDimension(const eTextureType texture) const
	{
		// GL's GetTextureDimension() returns 1/2/3 to pick InitializeTexture1D/2D/3D. VkImageType
		// is the same underlying "how many storage dimensions does this image have" concept,
		// just expressed as an enum instead of an int - a cube map's storage is still 2D
		// (6 layers), and array-ness doesn't add a storage dimension either.
		if (eTextureType::eTexture1D == texture || eTextureType::eTexture1DArray == texture)
		{
			return VK_IMAGE_TYPE_1D;
		}
		else if (eTextureType::eTexture2D == texture ||
			eTextureType::eTextureCube == texture ||
			eTextureType::eTexture2DArray == texture ||
			eTextureType::eTextureCubeArray == texture ||
			eTextureType::eTextureMultisample == texture ||
			eTextureType::eTextureMultisampleArray == texture)
		{
			return VK_IMAGE_TYPE_2D;
		}
		else if (eTextureType::eTexture3D == texture)
		{
			return VK_IMAGE_TYPE_3D;
		}
		else
		{
			return VK_IMAGE_TYPE_MAX_ENUM;
		}
	}

	VkFormat VulkanSystem::GetPixelFormat(const ePixelFormat pixel) const
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

	VkFilter VulkanSystem::GetFilterMode(const eFilterMode filter) const
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

	VkSamplerAddressMode VulkanSystem::GetWrapMode(const eWrapMode wrap) const
	{
		if (eWrapMode::eRepeat == wrap)
		{
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
		else if (eWrapMode::eMirroedRepeat == wrap)
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

	VkCullModeFlags VulkanSystem::GetCullFace(const eCullFace face) const
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

	VkFrontFace VulkanSystem::GetFrontFace(const eFrontFace face) const
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

	VkPrimitiveTopology VulkanSystem::GetDrawMode(const eDrawMode mode) const
	{
		if (eDrawMode::ePoints == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		}
		else if (eDrawMode::eLines == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		}
		else if (eDrawMode::eLine_Strip == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		}
		else if (eDrawMode::eLine_Loop == mode)
		{
			// Vulkan has no line-loop topology at all - closest approximation is a line strip;
			// closing the loop (last vertex back to first) has to be done by duplicating the
			// first vertex at the end of the vertex/index stream instead.
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		}
		else if (eDrawMode::eTriangles == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
		else if (eDrawMode::eTriangle_Strip == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		}
		else if (eDrawMode::eTriangle_Fan == mode)
		{
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
		}
		else
		{
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}

	VkPolygonMode VulkanSystem::GetPrimitiveMode(const ePrimitiveMode mode) const
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

	VkCompareOp VulkanSystem::GetCompareFunc(const eCompareFunc func) const
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
		else if (eCompareFunc::eGreatorEqual == func)
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

	VkStencilOp VulkanSystem::GetStencilOp(const eStencilOp op) const
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

	VkBlendFactor VulkanSystem::GetBlendFunc(const eBlendFunc func) const
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

	VkBlendOp VulkanSystem::GetBlendOp(const eBlendOp op) const
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

	VkShaderStageFlagBits VulkanSystem::GetShaderType(const eShaderType type) const
	{
		// NOTE: GLSystem::GetShaderType() maps eHull -> GL_TESS_EVALUATION_SHADER and never
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

	eCompareFunc VulkanSystem::GetCompareFunc(const VkCompareOp func) const
	{
		if (VK_COMPARE_OP_NEVER == func)
		{
			return eCompareFunc::eNever;
		}
		else if (VK_COMPARE_OP_EQUAL == func)
		{
			return eCompareFunc::eEqual;
		}
		else if (VK_COMPARE_OP_NOT_EQUAL == func)
		{
			return eCompareFunc::eNotEqual;
		}
		else if (VK_COMPARE_OP_LESS == func)
		{
			return eCompareFunc::eLess;
		}
		else if (VK_COMPARE_OP_LESS_OR_EQUAL == func)
		{
			return eCompareFunc::eLessEqual;
		}
		else if (VK_COMPARE_OP_GREATER == func)
		{
			return eCompareFunc::eGreater;
		}
		else if (VK_COMPARE_OP_GREATER_OR_EQUAL == func)
		{
			return eCompareFunc::eGreatorEqual;
		}
		else if (VK_COMPARE_OP_ALWAYS == func)
		{
			return eCompareFunc::eAlways;
		}
		else
		{
			return eCompareFunc::eNone;
		}
	}

	eStencilOp VulkanSystem::GetStencilOp(const VkStencilOp op) const
	{
		if (VK_STENCIL_OP_KEEP == op)
		{
			return eStencilOp::eKeep;
		}
		else if (VK_STENCIL_OP_ZERO == op)
		{
			return eStencilOp::eZero;
		}
		else if (VK_STENCIL_OP_REPLACE == op)
		{
			return eStencilOp::eReplace;
		}
		else if (VK_STENCIL_OP_INCREMENT_AND_CLAMP == op)
		{
			return eStencilOp::eInc_Clamp;
		}
		else if (VK_STENCIL_OP_DECREMENT_AND_CLAMP == op)
		{
			return eStencilOp::eDec_Clamp;
		}
		else if (VK_STENCIL_OP_INVERT == op)
		{
			return eStencilOp::eInvert;
		}
		else if (VK_STENCIL_OP_INCREMENT_AND_WRAP == op)
		{
			return eStencilOp::eInc_Wrap;
		}
		else if (VK_STENCIL_OP_DECREMENT_AND_WRAP == op)
		{
			return eStencilOp::eDec_Wrap;
		}
		else
		{
			return eStencilOp::eNone;
		}
	}

	eBlendFunc VulkanSystem::GetBlendFunc(const VkBlendFactor func) const
	{
		if (VK_BLEND_FACTOR_ZERO == func)
		{
			return eBlendFunc::eZero;
		}
		else if (VK_BLEND_FACTOR_ONE == func)
		{
			return eBlendFunc::eOne;
		}
		else if (VK_BLEND_FACTOR_SRC_COLOR == func)
		{
			return eBlendFunc::eSrc_Color;
		}
		else if (VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR == func)
		{
			return eBlendFunc::eOne_Minus_Src_Color;
		}
		else if (VK_BLEND_FACTOR_DST_COLOR == func)
		{
			return eBlendFunc::eDst_Color;
		}
		else if (VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR == func)
		{
			return eBlendFunc::eOne_Minus_Dst_Color;
		}
		else if (VK_BLEND_FACTOR_SRC_ALPHA == func)
		{
			return eBlendFunc::eSrc_Alpha;
		}
		else if (VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA == func)
		{
			return eBlendFunc::eOne_Minus_Src_Alpha;
		}
		else if (VK_BLEND_FACTOR_DST_ALPHA == func)
		{
			return eBlendFunc::eDst_Alpha;
		}
		else if (VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA == func)
		{
			return eBlendFunc::eOne_Minus_Dst_Alpha;
		}
		else
		{
			return eBlendFunc::eNone;
		}
	}

	eBlendOp VulkanSystem::GetBlendOp(const VkBlendOp op) const
	{
		if (VK_BLEND_OP_ADD == op)
		{
			return eBlendOp::eAdd;
		}
		else if (VK_BLEND_OP_SUBTRACT == op)
		{
			return eBlendOp::eSub;
		}
		else if (VK_BLEND_OP_REVERSE_SUBTRACT == op)
		{
			return eBlendOp::eReverse_Sub;
		}
		else if (VK_BLEND_OP_MIN == op)
		{
			return eBlendOp::eMin;
		}
		else if (VK_BLEND_OP_MAX == op)
		{
			return eBlendOp::eMax;
		}
		else
		{
			return eBlendOp::eNone;
		}
	}

	eCullFace VulkanSystem::GetCullFace(const VkCullModeFlags face) const
	{
		if (VK_CULL_MODE_FRONT_BIT == face)
		{
			return eCullFace::eFront;
		}
		else if (VK_CULL_MODE_BACK_BIT == face)
		{
			return eCullFace::eBack;
		}
		else if (VK_CULL_MODE_FRONT_AND_BACK == face)
		{
			return eCullFace::eFront_Back;
		}
		else
		{
			return eCullFace::eNone;
		}
	}

	eFrontFace VulkanSystem::GetFrontFace(const VkFrontFace face) const
	{
		if (VK_FRONT_FACE_COUNTER_CLOCKWISE == face)
		{
			return eFrontFace::eCCW;
		}
		else if (VK_FRONT_FACE_CLOCKWISE == face)
		{
			return eFrontFace::eCW;
		}
		else
		{
			return eFrontFace::eNone;
		}
	}

	ePrimitiveMode VulkanSystem::GetPrimitiveMode(const VkPolygonMode mode) const
	{
		if (VK_POLYGON_MODE_POINT == mode)
		{
			return ePrimitiveMode::ePoint;
		}
		else if (VK_POLYGON_MODE_LINE == mode)
		{
			return ePrimitiveMode::eLine;
		}
		else if (VK_POLYGON_MODE_FILL == mode)
		{
			return ePrimitiveMode::eFill;
		}
		else
		{
			return ePrimitiveMode::eNone;
		}
	}

	eTextureType VulkanSystem::GetTextureType(const VkImageViewType texture) const
	{
		if (VK_IMAGE_VIEW_TYPE_1D == texture)
		{
			return eTextureType::eTexture1D;
		}
		else if (VK_IMAGE_VIEW_TYPE_2D == texture)
		{
			// Ambiguous: the forward direction also sends eTextureMultisample here (Vulkan has no
			// separate multisample view type, sample count lives on VkImageCreateInfo::samples
			// instead). Resolves to the non-multisampled variant since that's the common case.
			return eTextureType::eTexture2D;
		}
		else if (VK_IMAGE_VIEW_TYPE_3D == texture)
		{
			return eTextureType::eTexture3D;
		}
		else if (VK_IMAGE_VIEW_TYPE_CUBE == texture)
		{
			return eTextureType::eTextureCube;
		}
		else if (VK_IMAGE_VIEW_TYPE_1D_ARRAY == texture)
		{
			return eTextureType::eTexture1DArray;
		}
		else if (VK_IMAGE_VIEW_TYPE_2D_ARRAY == texture)
		{
			// Same ambiguity as above - also covers eTextureMultisampleArray.
			return eTextureType::eTexture2DArray;
		}
		else if (VK_IMAGE_VIEW_TYPE_CUBE_ARRAY == texture)
		{
			return eTextureType::eTextureCubeArray;
		}
		else
		{
			return eTextureType::eNone;
		}
	}

	ePixelFormat VulkanSystem::GetPixelFormat(const VkFormat pixel) const
	{
		if (VK_FORMAT_R8_UNORM == pixel)
		{
			return ePixelFormat::eR8_UNorm;
		}
		else if (VK_FORMAT_R8G8_UNORM == pixel)
		{
			return ePixelFormat::eR8G8_UNorm;
		}
		else if (VK_FORMAT_R8G8B8_UNORM == pixel)
		{
			return ePixelFormat::eR8G8B8_UNorm;
		}
		else if (VK_FORMAT_R8G8B8A8_UNORM == pixel)
		{
			return ePixelFormat::eR8G8B8A8_UNorm;
		}
		else if (VK_FORMAT_R16_UNORM == pixel)
		{
			return ePixelFormat::eR16_UNorm;
		}
		else if (VK_FORMAT_R16G16_UNORM == pixel)
		{
			return ePixelFormat::eR16G16_UNorm;
		}
		else if (VK_FORMAT_R16G16B16_UNORM == pixel)
		{
			return ePixelFormat::eR16G16B16_UNorm;
		}
		else if (VK_FORMAT_R16G16B16A16_UNORM == pixel)
		{
			return ePixelFormat::eR16G16B16A16_UNorm;
		}
		else if (VK_FORMAT_R8G8B8A8_SRGB == pixel)
		{
			return ePixelFormat::eR8G8B8A8_sRGB;
		}
		else if (VK_FORMAT_R8_UINT == pixel)
		{
			return ePixelFormat::eR8_UInt;
		}
		else if (VK_FORMAT_R8G8_UINT == pixel)
		{
			return ePixelFormat::eR8G8_UInt;
		}
		else if (VK_FORMAT_R8G8B8_UINT == pixel)
		{
			return ePixelFormat::eR8G8B8_UInt;
		}
		else if (VK_FORMAT_R8G8B8A8_UINT == pixel)
		{
			return ePixelFormat::eR8G8B8A8_UInt;
		}
		else if (VK_FORMAT_R16_UINT == pixel)
		{
			return ePixelFormat::eR16_UInt;
		}
		else if (VK_FORMAT_R16G16_UINT == pixel)
		{
			return ePixelFormat::eR16G16_UInt;
		}
		else if (VK_FORMAT_R16G16B16_UINT == pixel)
		{
			return ePixelFormat::eR16G16B16_UInt;
		}
		else if (VK_FORMAT_R16G16B16A16_UINT == pixel)
		{
			return ePixelFormat::eR16G16B16A16_UInt;
		}
		else if (VK_FORMAT_R32_UINT == pixel)
		{
			return ePixelFormat::eR32_UInt;
		}
		else if (VK_FORMAT_R32G32_UINT == pixel)
		{
			return ePixelFormat::eR32G32_UInt;
		}
		else if (VK_FORMAT_R32G32B32_UINT == pixel)
		{
			return ePixelFormat::eR32G32B32_UInt;
		}
		else if (VK_FORMAT_R32G32B32A32_UINT == pixel)
		{
			return ePixelFormat::eR32G32B32A32_UInt;
		}
		else if (VK_FORMAT_R16_SFLOAT == pixel)
		{
			return ePixelFormat::eR16_Float;
		}
		else if (VK_FORMAT_R16G16_SFLOAT == pixel)
		{
			return ePixelFormat::eR16G16_Float;
		}
		else if (VK_FORMAT_R16G16B16_SFLOAT == pixel)
		{
			return ePixelFormat::eR16G16B16_Float;
		}
		else if (VK_FORMAT_R16G16B16A16_SFLOAT == pixel)
		{
			return ePixelFormat::eR16G16B16A16_Float;
		}
		else if (VK_FORMAT_R32_SFLOAT == pixel)
		{
			return ePixelFormat::eR32_Float;
		}
		else if (VK_FORMAT_R32G32_SFLOAT == pixel)
		{
			return ePixelFormat::eR32G32_Float;
		}
		else if (VK_FORMAT_R32G32B32_SFLOAT == pixel)
		{
			return ePixelFormat::eR32G32B32_Float;
		}
		else if (VK_FORMAT_R32G32B32A32_SFLOAT == pixel)
		{
			return ePixelFormat::eR32G32B32A32_Float;
		}
		else if (VK_FORMAT_D24_UNORM_S8_UINT == pixel)
		{
			return ePixelFormat::eD24_S8;
		}
		else if (VK_FORMAT_D32_SFLOAT == pixel)
		{
			return ePixelFormat::eD32;
		}
		else
		{
			return ePixelFormat::eNone;
		}
	}

	eFilterMode VulkanSystem::GetFilterMode(const VkFilter filter) const
	{
		if (VK_FILTER_NEAREST == filter)
		{
			return eFilterMode::eNearest;
		}
		else if (VK_FILTER_LINEAR == filter)
		{
			return eFilterMode::eLinear;
		}
		else
		{
			return eFilterMode::eNone;
		}
	}

	eWrapMode VulkanSystem::GetWrapMode(const VkSamplerAddressMode wrap) const
	{
		if (VK_SAMPLER_ADDRESS_MODE_REPEAT == wrap)
		{
			return eWrapMode::eRepeat;
		}
		else if (VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT == wrap)
		{
			return eWrapMode::eMirroedRepeat;
		}
		else if (VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE == wrap)
		{
			return eWrapMode::eClampToEdge;
		}
		else if (VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER == wrap)
		{
			return eWrapMode::eClampToBorder;
		}
		else
		{
			// Covers VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE too - no eWrapMode equivalent,
			// same as the forward direction never producing it.
			return eWrapMode::eNone;
		}
	}

	eWrapBorderColor VulkanSystem::GetWrapBorderColor(const VkBorderColor wrapBorderColor) const
	{
		if (VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK == wrapBorderColor)
		{
			return eWrapBorderColor::eTransparentBlack;
		}
		else if (VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK == wrapBorderColor)
		{
			return eWrapBorderColor::eOpaqueBlack;
		}
		else if (VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE == wrapBorderColor)
		{
			return eWrapBorderColor::eOpaqueWhite;
		}
		else
		{
			return eWrapBorderColor::eNone;
		}
	}

	eAttachment VulkanSystem::GetImageAspect(const VkImageAspectFlags aspect) const
	{
		if ((VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT) == aspect)
		{
			return eAttachment::eDepthStencil;
		}
		else if (VK_IMAGE_ASPECT_DEPTH_BIT == aspect)
		{
			return eAttachment::eDepth;
		}
		else if (VK_IMAGE_ASPECT_STENCIL_BIT == aspect)
		{
			return eAttachment::eStencil;
		}
		else if (VK_IMAGE_ASPECT_COLOR_BIT == aspect)
		{
			return eAttachment::eColor;
		}
		else
		{
			return eAttachment::eNone;
		}
	}

	VkBorderColor VulkanSystem::GetWrapBorderColor(const eWrapBorderColor wrapBorderColor) const
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

	VkImageAspectFlags VulkanSystem::GetImageAspect(const eAttachment attach) const
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
}
