#ifndef __WTR_GLSYSTEM_H__
#define __WTR_GLSYSTEM_H__

#include <RHI/RHISystem.h>
#include <RHI/RHIStates.h>
#include <RHI/OpenGL/Win32/WGLContext.h>

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
		Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc desc) override;

		void InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer);
		void InitializeVertexLayout(const RHIVertexLayoutCreateDesc info, Memory::RefPtr<RHIVertexLayout> layout);
		void InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture);
		void InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler);
		void InitializeShader(const RHIShaderCreateDesc info, Memory::RefPtr<RHIShader> shader);
		void InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline);

		void UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture) override;

		void ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) override;

		void SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t slot) override;
		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout) override;
		void SetTexture(Memory::RefPtr<const RHITexture> texture, const uint32_t slot) override;
		void SetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t slot) override;
		void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline) override;

		void UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t slot) override;
		void UnsetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout) override;
		void UnsetTexture(Memory::RefPtr<const RHITexture> texture, const uint32_t slot) override;
		void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t slot) override;
		void UnsetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline) override;

		void DispatchCompute(const RHIDispatchDesc info) override;
		void DrawIndexPrimitive(const RHIDrawIndexDesc info) override;

	private :
		void InitializeState();

		bool InitializeAttribute(Memory::RefPtr<RHIPipeLine> pipeline);
		bool InitializeSlot(Memory::RefPtr<RHIPipeLine> pipeline);

		bool InitializeTexture1D(const RHITextureCreateDesc info, const uint32_t textureID);
		bool InitializeTexture2D(const RHITextureCreateDesc info, const uint32_t textureID);
		bool InitializeTexture3D(const RHITextureCreateDesc info, const uint32_t textureID);
		bool InitializeTextureMulti(const RHITextureCreateDesc info, const uint32_t textureID);

		bool UpdateTexture1D(const RHITextureUpdateDesc info, const uint32_t textureID);
		bool UpdateTexture2D(const RHITextureUpdateDesc info, const uint32_t textureID);
		bool UpdateTexture3D(const RHITextureUpdateDesc info, const uint32_t textureID);

		const uint32_t GetBufferType(const eBufferType buffer) const override;
		const uint32_t GetDataAccess(const eDataAccess access) const override;
		const uint32_t GetDataType(const eDataType data) const override;
		const uint32_t GetMapAccess(const eMapAccess access) const override;

		const uint32_t GetTextureType(const eTextureType texture) const override;
		const uint32_t GetTextureDimension(const eTextureType texture) const override;
		const uint32_t GetInternalFormat(const ePixelFormat pixel) const;
		const uint32_t GetBaseFormat(const ePixelFormat pixel) const;
		const uint32_t GetPixelDataType(const ePixelFormat pixel) const;
		
		const uint32_t GetFilterMode(const eFilterMode filter) const override;
		const uint32_t GetWrapMode(const eWrapMode wrap) const override;
		
		const uint32_t GetCullFace(const eCullFace face) const override;
		const uint32_t GetFrontFace(const eFrontFace face) const override;
		
		const uint32_t GetDrawMode(const eDrawMode mode) const override;
		const uint32_t GetPrimitiveMode(const ePrimitiveMode mode) const override;
		
		const uint32_t GetCompareFunc(const eCompareFunc func) const override;
		
		const uint32_t GetStencilOp(const eStencilOp op) const override;
		
		const uint32_t GetBlendFunc(const eBlendFunc func) const override;
		const uint32_t GetBlendOp(const eBlendOp op) const override;
		
		const uint32_t GetShaderType(const eShaderType type) const override;

		const eCompareFunc GetCompareFunc(const uint32_t func) const;
		const eStencilOp GetStencilOp(const uint32_t op) const;
		const eBlendFunc GetBlendFunc(const uint32_t func) const;
		const eBlendOp GetBlendOp(const uint32_t op) const;
		const eCullFace GetCullFace(const uint32_t face) const;
		const eFrontFace GetFrontFace(const uint32_t face) const;
		const ePrimitiveMode GetPrimitiveMode(const uint32_t mode) const;

		bool IsSampler(const int32_t type) const;

		void FlushBuffer();
		void FlushVertexLayout();
		void FlushTexture();
		void FlushShader();
		void FlushPipeLine();
		void FlushSampler();

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