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
		Memory::RefPtr<RHIBufferView> GetRingBuffer(const size_t size, const size_t align);
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
		virtual Memory::RefPtr<RHIBufferView> CreateBufferView(Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual Memory::RefPtr<RHITextureView> CreateTextureView(Memory::RefPtr<RHITexture> texture) = 0;

		virtual bool ReleaseVertexLayout(Memory::RefPtr<RHIVertexLayout> layout) = 0;
		virtual bool ReleaseBuffer(Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual bool ReleaseTexture(Memory::RefPtr<RHITexture> texture) = 0;
		virtual bool ReleaseSampler(Memory::RefPtr<RHISampler> sampler) = 0;
		virtual bool ReleaseShader(Memory::RefPtr<RHIShader> shader) = 0;
		virtual bool ReleasePipeLine(Memory::RefPtr<RHIPipeLine> pipeline) = 0;
		virtual bool ReleaseRenderPass(Memory::RefPtr<RHIRenderPass> pass) = 0;

		virtual bool ReleaseBufferView(Memory::RefPtr<RHIBufferView> bufferView);
		virtual bool ReleaseTextureView(Memory::RefPtr<RHITextureView> textureView);

		virtual void InitializeVertexLayout(Memory::RefPtr<RHIVertexLayout> layout, const RHIVertexLayoutCreateDesc& info) = 0;
		virtual void InitializeBuffer(Memory::RefPtr<RHIBuffer> buffer, const RHIBufferCreateDesc& info) = 0;
		virtual void InitializeTexture(Memory::RefPtr<RHITexture> texture, const RHITextureCreateDesc& info) = 0;
		virtual void InitializeSampler(Memory::RefPtr<RHISampler> sampler, const RHISamplerCreateDesc& info) = 0;
		virtual void InitializeShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc& info) = 0;
		virtual void InitializePipeLine(Memory::RefPtr<RHIPipeLine> pipeline, const	RHIPipeLineCreateDesc& info) = 0;
		virtual void InitializeRenderPass(Memory::RefPtr<RHIRenderPass> pass, const RHIRenderPassCreateDesc& info) = 0;
		virtual void InitializeBufferView(Memory::RefPtr<RHIBufferView> bufferView, Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual void InitializeTextureView(Memory::RefPtr<RHITextureView> textureView, Memory::RefPtr<RHITexture> texture) = 0;

		virtual void UpdateBuffer(Memory::RefPtr<RHIBufferView> bufferView, const RHIBufferUpdateDesc& info) = 0;
		virtual void UpdateTexture(Memory::RefPtr<RHITextureView> textureView, const RHITextureUpdateDesc& info) = 0;

		virtual void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout) = 0;
		virtual void SetVertexBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const uint32_t binding) = 0;
		virtual void SetIndexBuffer(Memory::RefPtr<const RHIBufferView> bufferView) = 0;
		virtual void SetBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const uint32_t binding) = 0;
		virtual void SetTexture(Memory::RefPtr<const RHITextureView> textureView, const uint32_t binding) = 0;
		virtual void SetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t binding) = 0;
		virtual void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline) = 0;
		virtual void SetRenderPass(Memory::RefPtr<const RHIRenderPass> pass) = 0;

		virtual void UnsetVertexLayout() = 0;
		virtual void UnsetVertexBuffer(const uint32_t binding) = 0;
		virtual void UnsetIndexBuffer() = 0;
		virtual void UnsetBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const uint32_t binding) = 0;
		virtual void UnsetTexture(Memory::RefPtr<const RHITextureView> textureView, const uint32_t binding) = 0;
		virtual void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t binding) = 0;
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