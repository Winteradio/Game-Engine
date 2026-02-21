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

		void InitializeBuffer(const RHIBufferDesc& desc, Memory::RefPtr<RHIBuffer> buffer);
		void InitializeTexture(const RHITextureDesc& desc, Memory::RefPtr<RHITexture> texture);
		void InitializeSampler(const RHISamplerDesc& desc, Memory::RefPtr<RHISampler> sampler);
		void InitializeVertexShader(const RHIVertexShaderDesc& desc, Memory::RefPtr<RHIVertexShader> shader);
		void InitializeGeometryShader(const RHIGeometryShaderDesc& desc, Memory::RefPtr<RHIGeometryShader> shader);
		void InitializePixelShader(const RHIPixelShaderDesc& desc, Memory::RefPtr<RHIPixelShader> shader);
		void InitializeComputeShader(const RHIComputeShaderDesc& desc, Memory::RefPtr<RHIComputeShader> shader);
		void InitializePipeLine(const RHIPipeLineDesc& desc, Memory::RefPtr<RHIPipeLine> pipeline);

		void UpdateBuffer(const RHIBufferDesc& desc, Memory::RefPtr<RHIBuffer> buffer) override;
		void UpdateTexture(const RHITextureDesc& desc, Memory::RefPtr<RHITexture> texture) override;

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