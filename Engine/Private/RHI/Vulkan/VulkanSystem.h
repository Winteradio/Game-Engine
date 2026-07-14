#ifndef __WTR_VULKANSYSTEM_H__
#define __WTR_VULKANSYSTEM_H__

#include <RHI/RHISystem.h>
#include <RHI/RHIStates.h>

#include <vulkan/vulkan.h>

namespace wtr
{
	class VulkanDevice;
	class VulkanSurface;
	class VulkanCommand;
	class VulkanMemory;
};

namespace wtr
{
	class VulkanSystem : public RHISystem
	{
	public:
		VulkanSystem();
		~VulkanSystem();

	public:
		bool Init(void* nativeHandle) override;
		void FlushPending() override;

	public:
		void Clear(const RHIClearState& state) override;
		
		void BeginContext() override;
		void EndContext() override;
		void BeginFrame() override;
		void EndFrame() override;

		void Resize(const uint32_t width, const uint32_t height) override;
		void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height) override;

		Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferDesc desc) override;
		Memory::RefPtr<RHIVertexLayout> CreateVertexLayout(const RHIVertexLayoutDesc desc) override;
		Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc desc) override;
		Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerDesc desc) override;
		Memory::RefPtr<RHIShader> CreateVertexShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIShader> CreateGeometryShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIShader> CreateHullShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIShader> CreateDomainShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc desc) override;
		Memory::RefPtr<RHIRenderTarget> CreateRenderTarget(const RHIRenderTargetDesc desc) override;
		Memory::RefPtr<RHIRenderPass> CreateRenderPass(const RHIRenderPassDesc desc) override;

		void InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void InitializeVertexLayout(const RHIVertexLayoutCreateDesc info, Memory::RefPtr<RHIVertexLayout> layout) override;
		void InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) override;
		void InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler) override;
		void InitializeShader(const RHIShaderCreateDesc info, Memory::RefPtr<RHIShader> shader) override;
		void InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline) override;
		void InitializeRenderTarget(const RHIRenderTargetCreateDesc info, Memory::RefPtr<RHIRenderTarget> target) override;
		void InitializeRenderPass(const RHIRenderPassCreateDesc info, Memory::RefPtr<RHIRenderPass> pass) override;

		void UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture) override;
		void UpdateVertexLayout(const RHIVertexLayoutUpdateDesc info, Memory::RefPtr<RHIVertexLayout> layout) override;

		void ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) override;

		void SetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const RHIConstDesc info, const std::string& slotName) override;
		void SetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const std::string& slotName) override;
		void SetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const std::string& slotName) override;
		void SetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const std::string& slotName) override;

		void UnsetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const std::string& slotName) override;
		void UnsetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const std::string& slotName) override;
		void UnsetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const std::string& slotName) override;
		void UnsetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const std::string& slotName) override;

		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout) override;
		void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline) override;
		void SetRenderTarget(Memory::RefPtr<const RHIRenderTarget> target) override;

		void ClearRenderTarget(Memory::RefPtr<const RHIRenderTarget> target, const RHIClearState& state) override;

		void UnsetVertexLayout() override;
		void UnsetPipeLine() override;
		void UnsetRenderTarget() override;

		void DispatchCompute(const RHIDispatchDesc info) override;
		void DrawIndexPrimitive(const RHIDrawIndexDesc info) override;

	private :
		bool IsValid() const;

	private:
		Memory::RefPtr<VulkanDevice> m_device;
		Memory::RefPtr<VulkanSurface> m_surface;
		Memory::RefPtr<VulkanMemory> m_memory;

		// Vulkan Command Architecture (planned)
		// The Vulkan System is the single
		// 		- it must NOT be duplicated per thread
		// 
		// The shared owner of a fixed set of VulkanCommand instances 
		//		- VkInstance/VkPhysicalDevice/VkDevice/VkQueue are process-wide singletons 
		//		- only command recording needs per-thread separation
		// 
		//		- 1 Primary   : owned/recorded by the RHI Thread. 
		//						Begins/ends the rendering scope and submits to the graphics queue 
		//						(vkQueueSubmit -> vkQueuePresentKHR).
		// 
		//		- N Secondary : one per RHI Worker Thread, fixed count decided at Init time 
		//						(pool creation is relatively expensive - not recreated per frame).
		// 
		// Worker threads never own a VulkanSystem/VulkanDevice 
		//		- they only receive a handle to their assigned Secondary VulkanCommand from this VulkanSystem.
		//
		// Per-frame flow : 
		//		-> Workers record their assigned draw items in parallel into their own Secondary
		//		-> RHI Thread CPU-joins all workers (not a Vulkan sync primitive) 
		//		-> reassembles the Secondary handles in the intended draw order (NOT finish order) 
		//		-> vkCmdExecuteCommands into the Primary
		//		-> vkCmdEndRenderPass
		//		-> vkQueueSubmit
		//		-> Present.

		Memory::RefPtr<VulkanCommand> m_primary;
		Memory::RefPtr<VulkanCommand> m_secondary;

		// Frame Synchronization Objects (2 Semaphore + 1 Fence)
		// 
		// m_acquire -	signaled by WSI (vkAcquireNextImageKHR) once the swapchain image is free to write into. 
		//				Waited on by vkQueueSubmit, at the COLOR_ATTACHMENT_OUTPUT stage, 
		//				before rendering into that image begins.
		// 
		// m_present -	signaled by the GPU (vkQueueSubmit) once rendering into that image completes. 
		//				Waited on by vkQueuePresentKHR before the image is shown.
		//
		//           -	Kept as 2 separate VkSemaphore objects, not 1: a binary semaphore holds
		//              only 1 bit (signaled/unsignaled) and its signal/wait events must strictly
		//              alternate (S,W,S,W,...)
		// 
		//			 -	two signals with no wait between them is UB.
		//              The CPU cannot guarantee that ordering across the Acquire/Submit/Present loop 
		//				(this frame's Present-wait and next frame's Acquire-signal are two independent async 
		//				requests to WSI with no enforced ordering between them),
		//              so funneling both directions of dependency through one semaphore risks
		//              exactly that violation once the loop repeats. 
		//				Two objects means each only ever carries one role's traffic, so they can never collide.
		//
		// m_fence   -	the only one of the three the CPU can directly wait on (vkWaitForFences) 
		//			 -	binary semaphores have no host-observable wait function.
		//           -	Attached to vkQueueSubmit, NOT vkAcquireNextImageKHR. 
		//				It answers "is it safe to call m_primary->ResetPool() yet", 
		//				which depends on whether the GPU finished reading the exact commandBuffer that was submitted
		// 
		//			 -	Acquire never touches that memory at all, 
		//				so a fence there would answer a question nobody is asking.
		// 
		//           -	Created with VK_FENCE_CREATE_SIGNALED_BIT: 
		//				on the very first frame there is no prior Submit to signal it, 
		//				so vkWaitForFences would hang forever on frame 0 without this.
		//
		// ---------------------------------------------------------------------------------------
		// Semaphore Signal/Wait Relationship (CPU / WSI / GPU)
		// ---------------------------------------------------------------------------------------
		//
		// CPU::BeginFrame()
		//   vkAcquireNextImageKHR
		//		-> registers with WSI: "signal m_acquire once this image is free"
		//
		//		WSI (async, own schedule - not the CPU call above)
		//			-> waits for the previous consumer (the display) to finish reading the image
		//			-> signal m_acquire
		//
		// CPU::EndFrame()
		//   vkQueueSubmit
		//		-> wait:   m_acquire   (GPU checks this before the COLOR_ATTACHMENT_OUTPUT stage)
		//		-> signal: m_present   (GPU signals this once rendering completes)
		//
		//		GPU (async, own schedule - not the CPU call above)
		//			-> waits for m_acquire before writing to the image
		//			-> renders
		//			-> signal m_present
		//
		//   vkQueuePresentKHR
		//		-> wait: m_present   (WSI checks this before handing the image to the display)
		//
		// None of the 3 CPU calls above ever block the CPU itself 
		//	-	each just registers a wait/signal condition that WSI or the GPU Command Processor checks later, 
		//		on its own schedule. 
		//		That is exactly why 2 separate semaphores are required: the CPU races through 
		//		Acquire -> Submit -> Present -> Acquire(next frame) without pausing, 
		//		so the next frame's Acquire-signal can physically arrive 
		//		before this frame's Present-wait has been consumed. 
		//		One shared semaphore would risk 2 signals with no wait between them (UB for a binary semaphore)
		// 
		//	-	2 objects means each carries only one direction of dependency and can never collide with the other.
		VkSemaphore m_acquire;
		VkSemaphore m_present;
		VkFence		m_fence;

		uint32_t m_imageIndex;
	};
};

#endif // __WTR_VULKANSYSTEM_H__