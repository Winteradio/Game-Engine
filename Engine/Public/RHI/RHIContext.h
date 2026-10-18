#ifndef __WTR_RHICONTEXT_H__
#define __WTR_RHICONTEXT_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <RHI/RHIRingBuffer.h>

namespace wtr
{
	class RHIVertexLayout;
	class RHIBuffer;
	class RHITexture;
	class RHISampler;
	class RHIShader;
	class RHIPipeLine;
	class RHIRenderPass;

	struct RHIVertexLayoutDesc;
	struct RHIConstDesc;
	struct RHIBufferDesc;
	struct RHITextureDesc;
	struct RHISamplerDesc;
	struct RHIShaderDesc;
	struct RHIPipeLineDesc;
	struct RHIRenderPassDesc;

	struct RHIVertexLayoutCreateDesc;
	struct RHIBufferCreateDesc;
	struct RHITextureCreateDesc;
	struct RHISamplerCreateDesc;
	struct RHIShaderCreateDesc;
	struct RHIPipeLineCreateDesc;
	struct RHIRenderPassCreateDesc;

	struct RHIVertexLayoutUpdateDesc;
	struct RHIBufferUpdateDesc;
	struct RHITextureUpdateDesc;

	struct RHIDispatchDesc;
	struct RHIDrawDirectDesc;
	struct RHIDrawIndirectDesc;

	struct RHITransitionDesc;

	struct RHIBufferView;
	struct RHITextureView;
};

namespace wtr
{
	class RHIContext
	{
	public :
		RHIContext();
		virtual ~RHIContext() = default;

	public :
		RHIBufferView GetRingBuffer(const size_t size, const size_t align);
		void ResetRingBuffer(const size_t frame);

	public :
		virtual bool Init(void* nativeHandle) = 0;
		virtual bool InitResources(const size_t frameDiff);

	public :
		virtual void BeginContext() = 0;
		virtual void EndContext() = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void Resize(const uint32_t width, const uint32_t height) = 0;
		virtual void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height) = 0;

		virtual Memory::RefPtr<RHIVertexLayout> CreateVertexLayout(const RHIVertexLayoutDesc& desc) = 0;
		virtual Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferDesc& desc) = 0;
		virtual Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc& desc) = 0;
		virtual Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerDesc& desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateVertexShader(const RHIShaderDesc& desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateGeometryShader(const RHIShaderDesc& desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateHullShader(const RHIShaderDesc& desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateDomainShader(const RHIShaderDesc& desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderDesc& desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderDesc& desc) = 0;
		virtual Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc& desc) = 0;
		virtual Memory::RefPtr<RHIRenderPass> CreateRenderPass(const RHIRenderPassDesc& desc) = 0;

		virtual bool ReleaseVertexLayout(Memory::RefPtr<RHIVertexLayout> layout) = 0;
		virtual bool ReleaseBuffer(Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual bool ReleaseTexture(Memory::RefPtr<RHITexture> texture) = 0;
		virtual bool ReleaseSampler(Memory::RefPtr<RHISampler> sampler) = 0;
		virtual bool ReleaseShader(Memory::RefPtr<RHIShader> shader) = 0;
		virtual bool ReleasePipeLine(Memory::RefPtr<RHIPipeLine> pipeline) = 0;
		virtual bool ReleaseRenderPass(Memory::RefPtr<RHIRenderPass> pass) = 0;

		virtual void InitializeVertexLayout(const RHIVertexLayoutCreateDesc& info, Memory::RefPtr<RHIVertexLayout> layout) = 0;
		virtual void InitializeBuffer(const RHIBufferCreateDesc& info, Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual void InitializeTexture(const RHITextureCreateDesc& info, Memory::RefPtr<RHITexture> texture) = 0;
		virtual void InitializeSampler(const RHISamplerCreateDesc& info, Memory::RefPtr<RHISampler> sampler) = 0;
		virtual void InitializeShader(const RHIShaderCreateDesc& info, Memory::RefPtr<RHIShader> shader) = 0;
		virtual void InitializePipeLine(const RHIPipeLineCreateDesc& info, Memory::RefPtr<RHIPipeLine> pipeline) = 0;
		virtual void InitializeRenderPass(const RHIRenderPassCreateDesc& info, Memory::RefPtr<RHIRenderPass> pass) = 0;

		virtual void UpdateVertexLayout(const RHIVertexLayoutUpdateDesc& info, Memory::RefPtr<RHIVertexLayout> layout) = 0;
		virtual void UpdateBuffer(const RHIBufferUpdateDesc& info, Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual void UpdateTexture(const RHITextureUpdateDesc& info, Memory::RefPtr<RHITexture> texture) = 0;

		virtual void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout) = 0;
		virtual void SetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t binding, const uint32_t offset) = 0;
		virtual void SetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset) = 0;
		virtual void SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding, const uint32_t offset, const uint32_t range) = 0;
		virtual void SetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding) = 0;
		virtual void SetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding) = 0;
		virtual void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline) = 0;
		virtual void SetRenderPass(Memory::RefPtr<const RHIRenderPass> pass) = 0;

		virtual void UnsetVertexLayout() = 0;
		virtual void UnsetVertexBuffer(const uint32_t binding) = 0;
		virtual void UnsetIndexBuffer() = 0;
		virtual void UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding) = 0;
		virtual void UnsetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding) = 0;
		virtual void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding) = 0;
		virtual void UnsetPipeLine() = 0;
		virtual void UnsetRenderPass() = 0;

		virtual void Transition(const RHITransitionDesc& info) = 0;

		virtual void DispatchCompute(const RHIDispatchDesc& info) = 0;
		virtual void DrawDirect(const RHIDrawDirectDesc& info) = 0;
		virtual void DrawIndirect(const RHIDrawIndirectDesc& info) = 0;

	private :
		RHIRingBuffer m_ringBuffer;
	};
};

#endif // __WTR_RHICONTEXT_H__