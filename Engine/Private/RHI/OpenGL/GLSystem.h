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
		Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc desc) override;
		Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerDesc desc) override;
		Memory::RefPtr<RHIVertexShader> CreateVertexShader(const RHIVertexShaderDesc desc) override;
		Memory::RefPtr<RHIGeometryShader> CreateGeometryShader(const RHIGeometryShaderDesc desc) override;
		Memory::RefPtr<RHIPixelShader> CreatePixelShader(const RHIPixelShaderDesc desc) override;
		Memory::RefPtr<RHIComputeShader> CreateComputeShader(const RHIComputeShaderDesc desc) override;
		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc desc) override;

		void InitializeBuffer(const RHIBufferCreateInfo info, Memory::RefPtr<RHIBuffer> buffer);
		void InitializeTexture(const RHITextureCreateInfo info, Memory::RefPtr<RHITexture> texture);
		void InitializeSampler(const RHISamplerCreateInfo info, Memory::RefPtr<RHISampler> sampler);
		void InitializeVertexShader(const RHIVertexShaderCreateInfo info, Memory::RefPtr<RHIVertexShader> shader);
		void InitializeGeometryShader(const RHIGeometryShaderCreateInfo info, Memory::RefPtr<RHIGeometryShader> shader);
		void InitializePixelShader(const RHIPixelShaderCreateInfo info, Memory::RefPtr<RHIPixelShader> shader);
		void InitializeComputeShader(const RHIComputeShaderCreateInfo info, Memory::RefPtr<RHIComputeShader> shader);
		void InitializePipeLine(const RHIPipeLineCreateInfo info, Memory::RefPtr<RHIPipeLine> pipeline);

		void UpdateBuffer(const RHIBufferCreateInfo info, Memory::RefPtr<RHIBuffer> buffer) override;
		void UpdateTexture(const RHITextureCreateInfo info, Memory::RefPtr<RHITexture> texture) override;

		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer) override;
		void RemoveTexture(Memory::RefPtr<RHITexture> texture) override;
		void RemoveSampler(Memory::RefPtr<RHISampler> sampler) override;
		void RemoveShader(Memory::RefPtr<RHIShader> shader) override;
		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline) override;

		void DrawIndexPrimitive() override;

	private :
		const uint32_t GetBufferType(const eBufferType buffer) const;
		const uint32_t GetDataAccess(const eDataAccess access) const;
		const uint32_t GetDataType(const eDataType data) const;
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