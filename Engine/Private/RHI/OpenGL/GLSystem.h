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
		Memory::RefPtr<RHIVertexShader> CreateVertexShader(const RHIVertexShaderDesc desc) override;
		Memory::RefPtr<RHIGeometryShader> CreateGeometryShader(const RHIGeometryShaderDesc desc) override;
		Memory::RefPtr<RHIHullShader> CreateHullShader(const RHIHullShaderDesc desc) override;
		Memory::RefPtr<RHIPixelShader> CreatePixelShader(const RHIPixelShaderDesc desc) override;
		Memory::RefPtr<RHIComputeShader> CreateComputeShader(const RHIComputeShaderDesc desc) override;
		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc desc) override;

		void InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer);
		void InitializeVertexLayout(const RHIVertexLayoutCreateDesc info, Memory::RefPtr<RHIVertexLayout> layout);
		void InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture);
		void InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler);
		void InitializeVertexShader(const RHIVertexShaderCreateDesc info, Memory::RefPtr<RHIVertexShader> shader);
		void InitializeGeometryShader(const RHIGeometryShaderCreateDesc info, Memory::RefPtr<RHIGeometryShader> shader);
		void InitializeHullShader(const RHIHullShaderCreateDesc info, Memory::RefPtr<RHIHullShader> shader);
		void InitializePixelShader(const RHIPixelShaderCreateDesc info, Memory::RefPtr<RHIPixelShader> shader);
		void InitializeComputeShader(const RHIComputeShaderCreateDesc info, Memory::RefPtr<RHIComputeShader> shader);
		void InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline);

		void UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture) override;

		void ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) override;

		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer) override;
		void RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout) override;
		void RemoveTexture(Memory::RefPtr<RHITexture> texture) override;
		void RemoveSampler(Memory::RefPtr<RHISampler> sampler) override;
		void RemoveShader(Memory::RefPtr<RHIShader> shader) override;
		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline) override;

		void SetBuffer(Memory::RefPtr<RHIBuffer> buffer, const uint32_t slot) override;
		void SetVertexLayout(Memory::RefPtr<RHIVertexLayout> layout) override;
		void SetTexture(Memory::RefPtr<RHITexture> texture, const uint32_t slot) override;
		void SetSampler(Memory::RefPtr<RHISampler> sampler, const uint32_t slot) override;
		void SetPipeLine(Memory::RefPtr<RHIPipeLine> pipeline) override;

		void UnsetBuffer(Memory::RefPtr<RHIBuffer> buffer, const uint32_t slot) override;
		void UnsetVertexLayout(Memory::RefPtr<RHIVertexLayout> layout) override;
		void UnsetTexture(Memory::RefPtr<RHITexture> texture, const uint32_t slot) override;
		void UnsetSampler(Memory::RefPtr<RHISampler> sampler, const uint32_t slot) override;
		void UnsetPipeLine(Memory::RefPtr<RHIPipeLine> pipeline) override;

		void DispatchCompute(const RHIDispatchDesc info) override;
		void DrawIndexPrimitive(const RHIDrawIndexDesc info) override;

	private :
		const uint32_t GetBufferType(const eBufferType buffer) const;
		const uint32_t GetDataAccess(const eDataAccess access) const;
		const uint32_t GetDataType(const eDataType data) const;
		const uint32_t GetMapAccess(const eMapAccess access) const;
		const uint32_t GetTextureType(const eTextureType texture) const;
		const uint32_t GetPixelFormat(const ePixelFormat pixel) const;
		const uint32_t GetFilterMode(const eFilterMode filter) const;
		const uint32_t GetWrapMode(const eWrapMode wrap) const;
		const uint32_t GetCullFace(const eCullFace face) const;
		const uint32_t GetFrontFace(const eFrontFace face) const;
		const uint32_t GetDrawMode(const eDrawMode mode) const;
		const uint32_t GetPrimitiveMode(const ePrimitiveMode mode) const;
		const uint32_t GetCompareFunc(const eCompareFunc func) const;
		const uint32_t GetStencilOp(const eStencilOp op) const;
		const uint32_t GetBlendFunc(const eBlendFunc func) const;
		const uint32_t GetBlendOp(const eBlendOp op) const;

	private:
		WGLContext m_context;

		RHIColorState m_colorState;
		RHIDepthState m_depthState;
		RHIStencilState m_stencilState;
		RHIBlendState m_blendState;
		RHIRasterizerState m_rasterizerState;
	};
};

#endif // __WTR_GLSYSTEM_H__