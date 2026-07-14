#ifndef __WTR_RHISYSTEM_H__
#define __WTR_RHISYSTEM_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/HashSet.h>
#include <cstdint>

namespace wtr
{
	class RHIBuffer;
	class RHIVertexLayout;
	class RHITexture;
	class RHISampler;
	class RHIShader;
	class RHIPipeLine;
	class RHIRenderTarget;
	class RHIRenderPass;

	struct RHIConstDesc;
	struct RHIBufferDesc;
	struct RHIVertexLayoutDesc;
	struct RHITextureDesc;
	struct RHISamplerDesc;
	struct RHIShaderDesc;
	struct RHIPipeLineDesc;
	struct RHIRenderTargetDesc;
	struct RHIRenderPassDesc;
	struct RHIDrawIndexDesc;
	struct RHIDispatchDesc;

	struct RHIBufferCreateDesc;
	struct RHIVertexLayoutCreateDesc;
	struct RHITextureCreateDesc;
	struct RHISamplerCreateDesc;
	struct RHIShaderCreateDesc;
	struct RHIPipeLineCreateDesc;
	struct RHIRenderTargetCreateDesc;
	struct RHIRenderPassCreateDesc;

	struct RHIBufferUpdateDesc;
	struct RHITextureUpdateDesc;
	struct RHIVertexLayoutUpdateDesc;

	struct RHIColorState;
	struct RHIDepthState;
	struct RHIStencilState;
	struct RHIBlendState;
	struct RHIRasterizerState;
	struct RHIClearState;

	enum class eRenderType : uint8_t;
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
	enum class eAttachment : uint8_t;
	enum class eResourceSlot : uint8_t;
};

namespace wtr
{
	class RHISystem
	{
	public :
		RHISystem(const eRenderType type);
		virtual ~RHISystem();

	public :
		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer);
		void RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout);
		void RemoveTexture(Memory::RefPtr<RHITexture> texture);
		void RemoveSampler(Memory::RefPtr<RHISampler> sampler);
		void RemoveShader(Memory::RefPtr<RHIShader> shader);
		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline);
		void RemoveRenderTarget(Memory::RefPtr<RHIRenderTarget> target);
		void RemoveRenderPass(Memory::RefPtr<RHIRenderPass> pass);

	public :
		virtual bool Init(void* nativeHandle) = 0;
		virtual void FlushPending() = 0;

	public :
		virtual void Clear(const RHIClearState& state) = 0;

		virtual void BeginContext() = 0;
		virtual void EndContext() = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void Resize(const uint32_t width, const uint32_t height) = 0;
		virtual void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height) = 0;

		virtual Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferDesc desc) = 0;
		virtual Memory::RefPtr<RHIVertexLayout> CreateVertexLayout(const RHIVertexLayoutDesc desc) = 0;
		virtual Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc desc) = 0;
		virtual Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerDesc desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateVertexShader(const RHIShaderDesc desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateGeometryShader(const RHIShaderDesc desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateHullShader(const RHIShaderDesc desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateDomainShader(const RHIShaderDesc desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderDesc desc) = 0;
		virtual Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderDesc desc) = 0;
		virtual Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc desc) = 0;
		virtual Memory::RefPtr<RHIRenderTarget> CreateRenderTarget(const RHIRenderTargetDesc desc) = 0;
		virtual Memory::RefPtr<RHIRenderPass> CreateRenderPass(const RHIRenderPassDesc desc) = 0;

		virtual void InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual void InitializeVertexLayout(const RHIVertexLayoutCreateDesc info, Memory::RefPtr<RHIVertexLayout> layout) = 0;
		virtual void InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) = 0;
		virtual void InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler) = 0;
		virtual void InitializeShader(const RHIShaderCreateDesc info, Memory::RefPtr<RHIShader> shader) = 0;
		virtual void InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline) = 0;
		virtual void InitializeRenderTarget(const RHIRenderTargetCreateDesc info, Memory::RefPtr<RHIRenderTarget> target) = 0;
		virtual void InitializeRenderPass(const RHIRenderPassCreateDesc info, Memory::RefPtr<RHIRenderPass> pass) = 0;

		virtual void UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual void UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture) = 0;
		virtual void UpdateVertexLayout(const RHIVertexLayoutUpdateDesc info, Memory::RefPtr<RHIVertexLayout> layout) = 0;

		virtual void ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) = 0;
		virtual void ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) = 0;

		virtual void SetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const RHIConstDesc info, const std::string& name) = 0;
		virtual void SetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const std::string& name) = 0;
		virtual void SetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const std::string& name) = 0;
		virtual void SetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const std::string& name) = 0;

		virtual void UnsetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const std::string& name) = 0;
		virtual void UnsetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const std::string& name) = 0;
		virtual void UnsetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const std::string& name) = 0;
		virtual void UnsetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const std::string& name) = 0;

		virtual void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout) = 0;
		virtual void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline) = 0;
		virtual void SetRenderTarget(Memory::RefPtr<const RHIRenderTarget> target) = 0;
		virtual void SetRenderPass(Memory::RefPtr<const RHIRenderPass> pass) = 0;

		virtual void ClearRenderTarget(Memory::RefPtr<const RHIRenderTarget> target, const RHIClearState& state) = 0;

		virtual void UnsetVertexLayout() = 0;
		virtual void UnsetPipeLine() = 0;
		virtual void UnsetRenderTarget() = 0;
		virtual void UnsetRenderPass() = 0;

		virtual void DispatchCompute(const RHIDispatchDesc info) = 0;
		virtual void DrawIndexPrimitive(const RHIDrawIndexDesc info) = 0;

	protected :
		bool CheckShaderVariable(Memory::RefPtr<RHIShader> shader);

	protected:
		wtr::HashSet<Memory::RefPtr<RHIBuffer>> m_pendingBuffers;
		wtr::HashSet<Memory::RefPtr<RHIVertexLayout>> m_pendingVertexLayouts;
		wtr::HashSet<Memory::RefPtr<RHITexture>> m_pendingTextures;
		wtr::HashSet<Memory::RefPtr<RHISampler>> m_pendingSamplers;
		wtr::HashSet<Memory::RefPtr<RHIShader>> m_pendingShaders;
		wtr::HashSet<Memory::RefPtr<RHIPipeLine>> m_pendingPipeLines;
		wtr::HashSet<Memory::RefPtr<RHIRenderTarget>> m_pendingRenderTargets;
		wtr::HashSet<Memory::RefPtr<RHIRenderPass>> m_pendingRenderPasses;
	};
};

#endif // __WTR_RHISYSTEM_H__