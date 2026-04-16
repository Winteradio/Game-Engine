#ifndef __WTR_RHISYSTEM_H__
#define __WTR_RHISYSTEM_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <cstdint>

namespace wtr
{
	class RHIBuffer;
	class RHIVertexLayout;
	class RHITexture;
	class RHISampler;
	class RHIShader;
	class RHIPipeLine;

	struct RHIBufferDesc;
	struct RHIVertexLayoutDesc;
	struct RHITextureDesc;
	struct RHISamplerDesc;
	struct RHIShaderDesc;
	struct RHIPipeLineDesc;
	struct RHIDrawIndexDesc;
	struct RHIDispatchDesc;

	struct RHIBufferCreateDesc;
	struct RHIVertexLayoutCreateDesc;
	struct RHITextureCreateDesc;
	struct RHISamplerCreateDesc;
	struct RHIShaderCreateDesc;
	struct RHIPipeLineCreateDesc;

	struct RHIBufferUpdateDesc;
	struct RHITextureUpdateDesc;

	struct RHIColorState;
	struct RHIDepthState;
	struct RHIStencilState;
	struct RHIBlendState;
	struct RHIRasterizerState;
	struct RHIClearState;

	enum class eBufferType : uint8_t;
	enum class eDataAccess : uint8_t;
	enum class eDataType : uint8_t;
	enum class eMapAccess : uint8_t;
	enum class eTextureType : uint8_t;
	enum class ePixelFormat : uint8_t;
	enum class eFilterMode : uint8_t;
	enum class eWrapMode : uint8_t;
	enum class eCullFace : uint8_t;
	enum class eFrontFace : uint8_t;
	enum class eDrawMode : uint8_t;
	enum class ePrimitiveMode : uint8_t;
	enum class eCompareFunc : uint8_t;
	enum class eStencilOp : uint8_t;
	enum class eBlendFunc : uint8_t;
	enum class eBlendOp : uint8_t;
	enum class eShaderType : uint8_t;
};

namespace wtr
{
	class RHISystem
	{
	public :
		RHISystem();
		virtual ~RHISystem();

	public :
		virtual bool Init(void* nativeHandle) = 0;

	public :
		virtual void Clear(const RHIClearState& state) = 0;
		virtual void Flush() = 0;
		virtual void MakeCurrent() = 0;
		virtual void ReleaseCurrent() = 0;
		virtual void Present() = 0;
		virtual void Resize(const uint32_t width, const uint32_t height) = 0;
		virtual void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height) = 0;

		virtual void SetColorState(const RHIColorState& state) = 0;
		virtual void SetDepthState(const RHIDepthState& state) = 0;
		virtual void SetStencilState(const RHIStencilState& state) = 0;
		virtual void SetBlendState(const RHIBlendState& state) = 0;
		virtual void SetRasterizerState(const RHIRasterizerState& state) = 0;

		virtual Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferDesc desc) = 0;
		virtual Memory::RefPtr<RHIVertexLayout> CreateVertexLayout(const RHIVertexLayoutDesc desc) = 0;
		virtual Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc desc) = 0;
		virtual Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerDesc desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateVertexShader(const RHIShaderDesc desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateGeometryShader(const RHIShaderDesc desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateHullShader(const RHIShaderDesc desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderDesc desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderDesc desc) = 0;
		virtual Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc desc) = 0;

		virtual void InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual void InitializeVertexLayout(const RHIVertexLayoutCreateDesc info, Memory::RefPtr<RHIVertexLayout> layout) = 0;
		virtual void InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) = 0;
		virtual void InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler) = 0;
		virtual void InitializeShader(const RHIShaderCreateDesc info, Memory::RefPtr<RHIShader> shader) = 0;
		virtual void InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline) = 0;

		virtual void UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual void UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture) = 0;

		virtual void ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual void ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) = 0;

		virtual void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual void RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout) = 0;
		virtual void RemoveTexture(Memory::RefPtr<RHITexture> texture) = 0;
		virtual void RemoveSampler(Memory::RefPtr<RHISampler> sampler) = 0;
		virtual void RemoveShader(Memory::RefPtr<RHIShader> shader) = 0;
		virtual void RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline) = 0;

		virtual void SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t slot) = 0;
		virtual void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout) = 0;
		virtual void SetTexture(Memory::RefPtr<const RHITexture> texture, const uint32_t slot) = 0;
		virtual void SetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t slot) = 0;
		virtual void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline) = 0;

		virtual void UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t slot) = 0;
		virtual void UnsetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout) = 0;
		virtual void UnsetTexture(Memory::RefPtr<const RHITexture> texture, const uint32_t slot) = 0;
		virtual void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t slot) = 0;
		virtual void UnsetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline) = 0;

		virtual void DispatchCompute(const RHIDispatchDesc info) = 0;
		virtual void DrawIndexPrimitive(const RHIDrawIndexDesc info) = 0;

	protected :
		virtual const uint32_t GetBufferType(const eBufferType buffer) const = 0;
		virtual const uint32_t GetDataAccess(const eDataAccess access) const = 0;
		virtual const uint32_t GetDataType(const eDataType data) const = 0;
		virtual const uint32_t GetMapAccess(const eMapAccess access) const = 0;
		virtual const uint32_t GetTextureType(const eTextureType type) const = 0;
		virtual const uint32_t GetTextureDimension(const eTextureType type) const = 0;
		virtual const uint32_t GetFilterMode(const eFilterMode filter) const = 0;
		virtual const uint32_t GetWrapMode(const eWrapMode wrap) const = 0;
		virtual const uint32_t GetCullFace(const eCullFace mode) const = 0;
		virtual const uint32_t GetFrontFace(const eFrontFace face) const = 0;
		virtual const uint32_t GetDrawMode(const eDrawMode mode) const = 0;
		virtual const uint32_t GetPrimitiveMode(const ePrimitiveMode mode) const = 0;
		virtual const uint32_t GetCompareFunc(const eCompareFunc func) const = 0;
		virtual const uint32_t GetStencilOp(const eStencilOp op) const = 0;
		virtual const uint32_t GetBlendFunc(const eBlendFunc func) const = 0;
		virtual const uint32_t GetBlendOp(const eBlendOp op) const = 0;
		virtual const uint32_t GetShaderType(const eShaderType type) const = 0;
	};
};

#endif // __WTR_RHISYSTEM_H__