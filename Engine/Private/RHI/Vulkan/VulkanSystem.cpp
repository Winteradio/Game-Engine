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
		, m_secondary()
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

		if (m_secondary)
		{
			m_secondary->Release(m_device);
			m_secondary.Reset();
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
		m_secondary = !m_secondary ? Memory::MakeRef<VulkanCommand>(false) : m_secondary;

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

		if (!m_primary->Init(m_device) || !m_secondary->Init(m_device))
		{
			return false;
		}

		if (!m_memory->Init(m_device))
		{
			return false;
		}

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
		if (!m_secondary || m_secondary->GetCommandBuffer() == VK_NULL_HANDLE)
		{
			return;
		}

		VkCommandBuffer commandBuffer = m_secondary->GetCommandBuffer();

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
		if (!m_secondary || m_secondary->GetCommandBuffer() == VK_NULL_HANDLE)
		{
			return;
		}

		VkCommandBuffer commandBuffer = m_secondary->GetCommandBuffer();

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

	void VulkanSystem::DispatchCompute(const RHIDispatchDesc info)
	{
	}

	void VulkanSystem::DrawIndexPrimitive(const RHIDrawIndexDesc info)
	{
	}

	bool VulkanSystem::IsValid() const
	{
		return !m_device || !m_surface || !m_memory || !m_primary || !m_secondary;
	}
};
