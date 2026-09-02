#ifndef __WTR_GLSYSTEM_H__
#define __WTR_GLSYSTEM_H__

#include <RHI/RHISystem.h>
#include <RHI/RHIStates.h>
#include <RHI/OpenGL/WGL/WGLContext.h>

namespace wtr
{
	class GLSystem : public RHISystem
	{
	public :
		GLSystem();
		~GLSystem();

	public :
		bool Init(void* nativeHandle) override;
		void FlushPending() override;

	public :
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

		void InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void InitializeVertexLayout(const RHIVertexLayoutCreateDesc info, Memory::RefPtr<RHIVertexLayout> layout) override;
		void InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) override;
		void InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler) override;
		void InitializeShader(const RHIShaderCreateDesc info, Memory::RefPtr<RHIShader> shader) override;
		void InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline) override;
		void InitializeRenderTarget(const RHIRenderTargetCreateDesc info, Memory::RefPtr<RHIRenderTarget> target) override;

		void UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture) override;
		void UpdateVertexLayout(const RHIVertexLayoutUpdateDesc info, Memory::RefPtr<RHIVertexLayout> layout) override;

		void ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) override;

		void SetConstant(const RHIConstDesc info, const int32_t binding);
		void SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding);
		void SetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding);
		void SetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding);

		void SetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t bidingIndex, const uint32_t offset);
		void SetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset);

		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout);
		void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline);

		void UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding);
		void UnsetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding);
		void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding);

		void UnsetVertexBuffer(const uint32_t bidingIndex);
		void UnsetIndexBuffer();

		void UnsetVertexLayout();
		void UnsetPipeLine();

		void BeginRenderPass(Memory::RefPtr<const RHIRenderTarget> target);
		void EndRenderPass();
		void ClearRenderTarget(Memory::RefPtr<const RHIRenderTarget> target, const RHIClearState state);

		void DispatchCompute(const RHIDispatchDesc info) override;
		void DrawIndexPrimitive(const RHIDrawIndexDesc info) override;

	public :
		uint32_t GetBufferType(const eBufferType buffer) const;
		uint32_t GetDataAccess(const eDataAccess access) const;
		uint32_t GetDataType(const eDataType data) const;
		uint32_t GetDataDivisor(const eDataDivisor divisor) const;
		uint32_t GetMapAccess(const eMapAccess access) const;

		uint32_t GetTextureType(const eTextureType texture) const;
		uint32_t GetTextureDimension(const eTextureType texture) const;
		uint32_t GetPixelFormat(const ePixelFormat pixel) const;
		uint32_t GetBaseFormat(const ePixelFormat pixel) const;
		uint32_t GetPixelDataType(const ePixelFormat pixel) const;

		uint32_t GetFilterMode(const eFilterMode filter) const;
		uint32_t GetWrapMode(const eWrapMode wrap) const;

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
		uint32_t GetDepthStencilAttachment(const eAttachment attach) const;

		eCompareFunc GetCompareFunc(const uint32_t func) const;
		eStencilOp GetStencilOp(const uint32_t op) const;
		eBlendFunc GetBlendFunc(const uint32_t func) const;
		eBlendOp GetBlendOp(const uint32_t op) const;
		eCullFace GetCullFace(const uint32_t face) const;
		eFrontFace GetFrontFace(const uint32_t face) const;
		ePrimitiveMode GetPrimitiveMode(const uint32_t mode) const;

		fvec4 GetWrapBorderColor(const eWrapBorderColor wrapBorderColor) const;

		bool IsSampler(const int32_t type) const;

	private :
		void InitializeState();

		bool InitializeFrameBuffer(Memory::RefPtr<const RHITexture>, const eAttachment attach, const uint32_t slot = 0);

		bool InitializeTexture1D(const RHITextureCreateDesc info, const uint32_t textureID);
		bool InitializeTexture2D(const RHITextureCreateDesc info, const uint32_t textureID);
		bool InitializeTexture3D(const RHITextureCreateDesc info, const uint32_t textureID);

		void SetColorState(const RHIColorState& state);
		void SetDepthState(const RHIDepthState& state);
		void SetStencilState(const RHIStencilState& state);
		void SetBlendState(const RHIBlendState& state);
		void SetRasterizerState(const RHIRasterizerState& state);

		bool UpdateTexture1D(const RHITextureUpdateDesc info, const uint32_t textureID);
		bool UpdateTexture2D(const RHITextureUpdateDesc info, const uint32_t textureID);
		bool UpdateTexture3D(const RHITextureUpdateDesc info, const uint32_t textureID);

		bool ResizeTexture1D(const RHITextureCreateDesc info, const uint32_t textureID);
		bool ResizeTexture2D(const RHITextureCreateDesc info, const uint32_t textureID);
		bool ResizeTexture3D(const RHITextureCreateDesc info, const uint32_t textureID);

		void FlushBuffer();
		void FlushVertexLayout();
		void FlushTexture();
		void FlushShader();
		void FlushPipeLine();
		void FlushSampler();
		void FlushRenderTarget();

	private:
		WGLContext m_context;

		RHIClearState m_clearState;
		RHIColorState m_colorState;
		RHIDepthState m_depthState;
		RHIStencilState m_stencilState;
		RHIBlendState m_blendState;
		RHIRasterizerState m_rasterizerState;
	};
};

#endif // __WTR_GLSYSTEM_H__