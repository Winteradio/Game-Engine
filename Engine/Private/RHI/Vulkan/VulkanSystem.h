#ifndef __WTR_VULKANSYSTEM_H__
#define __WTR_VULKANSYSTEM_H__

#include <RHI/RHISystem.h>
#include <RHI/RHIStates.h>

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
		void Flush() override;
		void MakeCurrent() override;
		void ReleaseCurrent() override;
		void Present() override;
		void Resize(const uint32_t width, const uint32_t height) override;
		void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height) override;

		void SetColorState(const RHIColorState& state);
		void SetDepthState(const RHIDepthState& state);
		void SetStencilState(const RHIStencilState& state);
		void SetBlendState(const RHIBlendState& state);
		void SetRasterizerState(const RHIRasterizerState& state);

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
		// The shared owner of a fixed set of VulkanCommand instances 
		//		- VkInstance/VkPhysicalDevice/VkDevice/VkQueue are process-wide singletons 
		//		- only command recording needs per-thread separation
		// 
		//		- 1 Primary   : owned/recorded by the RHI Thread. 
		//						Begins/ends the rendering scope and submits to the graphics queue (vkQueueSubmit -> vkQueuePresentKHR).
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
		//		-> vkCmdEndRendering 
		//		-> vkQueueSubmit 
		//		-> Present.
		Memory::RefPtr<VulkanCommand> m_primary;
		DynamicArray<Memory::RefPtr<VulkanCommand>> m_secondaries;
	};
};

#endif // __WTR_VULKANSYSTEM_H__