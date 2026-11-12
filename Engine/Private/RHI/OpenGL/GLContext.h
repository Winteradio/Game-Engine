#ifndef __WTR_GLCONTEXT_H__
#define __WTR_GLCONTEXT_H__

#include <RHI/RHIContext.h>
#include <RHI/RHIStates.h>
#include <RHI/OpenGL/WGL/WGLContext.h>

namespace wtr
{
	class GLContext : public RHIContext
	{
	public :
		GLContext();
		~GLContext();

	public :
		bool Init(void* nativeHandle) override;

	public :
		void BeginContext() override;
		void EndContext() override;
		void BeginFrame() override;
		void EndFrame() override;

		void Resize(const uint32_t width, const uint32_t height) override;
		void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height) override;

		Memory::RefPtr<RHIVertexLayout> CreateVertexLayout(const RHIVertexLayoutDesc& desc) override;
		Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferDesc& desc) override;
		Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc& desc) override;
		Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerDesc& desc) override;
		Memory::RefPtr<RHIShader> CreateVertexShader(const RHIShaderDesc& desc) override;
		Memory::RefPtr<RHIShader> CreateGeometryShader(const RHIShaderDesc& desc) override;
		Memory::RefPtr<RHIShader> CreateHullShader(const RHIShaderDesc& desc) override;
		Memory::RefPtr<RHIShader> CreateDomainShader(const RHIShaderDesc& desc) override;
		Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderDesc& desc) override;
		Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderDesc& desc) override;
		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc& desc) override;
		Memory::RefPtr<RHIRenderPass> CreateRenderPass(const RHIRenderPassDesc& desc) override;
		Memory::RefPtr<RHIBufferView> CreateBufferView(Memory::RefPtr<RHIBuffer> buffer) override;
		Memory::RefPtr<RHITextureView> CreateTextureView(Memory::RefPtr<RHITexture> texture) override;

		bool ReleaseVertexLayout(Memory::RefPtr<RHIVertexLayout> layout) override;
		bool ReleaseBuffer(Memory::RefPtr<RHIBuffer> buffer) override;
		bool ReleaseTexture(Memory::RefPtr<RHITexture> texture) override;
		bool ReleaseSampler(Memory::RefPtr<RHISampler> sampler) override;
		bool ReleaseShader(Memory::RefPtr<RHIShader> shader) override;
		bool ReleasePipeLine(Memory::RefPtr<RHIPipeLine> pipeline) override;
		bool ReleaseRenderPass(Memory::RefPtr<RHIRenderPass> pass) override;

		void InitializeVertexLayout(Memory::RefPtr<RHIVertexLayout> layout, const RHIVertexLayoutCreateDesc& info) override;
		void InitializeBuffer(Memory::RefPtr<RHIBuffer> buffer, const RHIBufferCreateDesc& info) override;
		void InitializeTexture(Memory::RefPtr<RHITexture> texture, const RHITextureCreateDesc& info) override;
		void InitializeSampler(Memory::RefPtr<RHISampler> sampler, const RHISamplerCreateDesc& info) override;
		void InitializeShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc& info) override;
		void InitializePipeLine(Memory::RefPtr<RHIPipeLine> pipeline, const RHIPipeLineCreateDesc& info) override;
		void InitializeRenderPass(Memory::RefPtr<RHIRenderPass> pass, const RHIRenderPassCreateDesc& info) override;
		void InitializeBufferView(Memory::RefPtr<RHIBufferView> bufferView, Memory::RefPtr<RHIBuffer> buffer) override;
		void InitializeTextureView(Memory::RefPtr<RHITextureView> textureView, Memory::RefPtr<RHITexture> texture) override;

		void UpdateBuffer(Memory::RefPtr<RHIBufferView> bufferView, const RHIBufferUpdateDesc& info) override;
		void UpdateTexture(Memory::RefPtr<RHITextureView> texture, const RHITextureUpdateDesc& info) override;

		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout) override;
		void SetVertexBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const uint32_t binding) override;
		void SetIndexBuffer(Memory::RefPtr<const RHIBufferView> bufferView) override;
		void SetBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const uint32_t binding) override;
		void SetTexture(Memory::RefPtr<const RHITextureView> textureView, const uint32_t binding) override;
		void SetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t binding) override;
		void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline) override;
		void SetRenderPass(Memory::RefPtr<const RHIRenderPass> pass) override;

		void UnsetVertexLayout() override;
		void UnsetVertexBuffer(const uint32_t binding) override;
		void UnsetIndexBuffer() override;
		void UnsetBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const uint32_t binding) override;
		void UnsetTexture(Memory::RefPtr<const RHITextureView> textureView, const uint32_t binding) override;
		void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t binding) override;
		void UnsetPipeLine() override;
		void UnsetRenderPass() override;

		void Transition(const RHITransitionDesc& info) override;

		void DispatchCompute(const RHIDispatchDesc& info) override;
		void DrawDirect(const RHIDrawDirectDesc& info) override;
		void DrawIndirect(const RHIDrawIndirectDesc& info) override;

	public :
		uint32_t GetBindingType(const eBindingType type) const;

		uint32_t GetDataUsage(const eDataUsage usage) const;
		uint32_t GetDataState(const eDataState state) const;
		uint32_t GetDataStage(const eDataStage stage) const;
		uint32_t GetDataType(const eDataType type) const;
		uint32_t GetDataDivisor(const eDataDivisor divisor) const;

		uint32_t GetBufferUsage(const eBufferUsage usage) const;

		uint32_t GetTextureFormat(const eTextureFormat format) const;
		uint32_t GetTextureSwizzle(const eTextureSwizzle swizzle) const;
		uint32_t GetTextureType(const eTextureType type) const;
		uint32_t GetTextureDimension(const eTextureType type) const;
		uint32_t GetTextureDataType(const eTextureFormat format) const;
		uint32_t GetTextureDataFormat(const eTextureFormat format) const;

		uint32_t GetSamplerFilter(const eSamplerFilter filter) const;
		uint32_t GetSamplerWrap(const eSamplerWrap wrap) const;
		fvec4 GetSamplerBorderColor(const eSamplerBorderColor borderColor) const;

		uint32_t GetCullFace(const eCullFace face) const;
		uint32_t GetFrontFace(const eFrontFace face) const;

		uint32_t GetDrawMode(const eDrawMode mode) const;
		uint32_t GetPrimitiveMode(const ePrimitiveMode mode) const;

		uint32_t GetCompareFunc(const eCompareFunc func) const;

		uint32_t GetStencilOp(const eStencilOp op) const;

		uint32_t GetBlendFunc(const eBlendFunc func) const;
		uint32_t GetBlendOp(const eBlendOp op) const;

		uint32_t GetShaderType(const eShaderType type) const;

		uint32_t GetColorAttachment(const uint32_t slot) const;
		uint32_t GetDepthStencilAttachment(const eAttachmentType attach) const;

		uint32_t GetBufferBarrier(const eBufferUsage usage) const;
		uint32_t GetTextureBarrier(const eTextureUsage usage) const;

		bool IsInteger(const eDataType type) const;
		bool IsInteger(const eTextureFormat format) const;

		eCompareFunc GetCompareFunc(const uint32_t func) const;
		eStencilOp GetStencilOp(const uint32_t op) const;
		eBlendFunc GetBlendFunc(const uint32_t func) const;
		eBlendOp GetBlendOp(const uint32_t op) const;
		eCullFace GetCullFace(const uint32_t face) const;
		eFrontFace GetFrontFace(const uint32_t face) const;
		ePrimitiveMode GetPrimitiveMode(const uint32_t mode) const;

	private :
		void InitializeState();

		bool InitializeFrameBuffer(Memory::RefPtr<const RHITexture>, const eAttachmentType attach, const uint32_t slot = 0);

		uint32_t InitializeTexture1D(const RHITextureCreateDesc& info);
		uint32_t InitializeTexture2D(const RHITextureCreateDesc& info);
		uint32_t InitializeTexture3D(const RHITextureCreateDesc& info);

		bool UpdateTexture1D(const RHITextureUpdateDesc& info, const uint32_t textureID);
		bool UpdateTexture2D(const RHITextureUpdateDesc& info, const uint32_t textureID);
		bool UpdateTexture3D(const RHITextureUpdateDesc& info, const uint32_t textureID);

		bool ResizeTexture1D(const RHITextureCreateDesc& info, const uint32_t textureID);
		bool ResizeTexture2D(const RHITextureCreateDesc& info, const uint32_t textureID);
		bool ResizeTexture3D(const RHITextureCreateDesc& info, const uint32_t textureID);

		void SetColorState(const RHIColorState& state);
		void SetDepthState(const RHIDepthState& state);
		void SetStencilState(const RHIStencilState& state);
		void SetBlendState(const RHIBlendState& state);
		void SetRasterizerState(const RHIRasterizerState& state);

	private:
		WGLContext m_context;
	};
};

#endif // __WTR_GLCONTEXT_H__