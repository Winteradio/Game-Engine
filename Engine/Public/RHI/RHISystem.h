#ifndef __WTR_RHISYSTEM_H__
#define __WTR_RHISYSTEM_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <cstdint>

namespace wtr
{
	class Name;
	class RHIContext;
	class RHIResource;
	class RHIBuffer;
	class RHIVertexLayout;
	class RHITexture;
	class RHISampler;
	class RHIShader;
	class RHIPipeLine;
	class RHIRenderPass;

	struct RHIConstDesc;
	struct RHIBufferDesc;
	struct RHIVertexLayoutDesc;
	struct RHITextureDesc;
	struct RHISamplerDesc;
	struct RHIShaderDesc;
	struct RHIPipeLineDesc;
	struct RHIRenderPassDesc;
	struct RHIDrawDirectDesc;
	struct RHIDrawIndirectDesc;
	struct RHIDispatchDesc;

	struct RHIBufferCreateDesc;
	struct RHIVertexLayoutCreateDesc;
	struct RHITextureCreateDesc;
	struct RHISamplerCreateDesc;
	struct RHIShaderCreateDesc;
	struct RHIPipeLineCreateDesc;
	struct RHIRenderPassCreateDesc;

	struct RHIBufferUpdateDesc;
	struct RHITextureUpdateDesc;
	struct RHIVertexLayoutUpdateDesc;

	struct RHITransitionDesc;

	struct RHIColorState;
	struct RHIDepthState;
	struct RHIStencilState;
	struct RHIBlendState;
	struct RHIRasterizerState;
	struct RHIClearState;

	enum class eRenderType : uint8_t;
};

namespace wtr
{
	namespace RHISystem
	{
		bool Init(const eRenderType type, void* nativeHandle);
		void Release();
		void FlushPending();

		void BeginContext();
		void EndContext();
		void BeginFrame();
		void EndFrame();

		void Resize(const uint32_t width, const uint32_t height);
		void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height);

		Memory::RefPtr<RHIVertexLayout> CreateVertexLayout(const RHIVertexLayoutDesc& desc);
		Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferDesc& desc);
		Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc& desc);
		Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerDesc& desc);
		Memory::RefPtr<RHIShader> CreateVertexShader(const RHIShaderDesc& desc);
		Memory::RefPtr<RHIShader> CreateGeometryShader(const RHIShaderDesc& desc);
		Memory::RefPtr<RHIShader> CreateHullShader(const RHIShaderDesc& desc);
		Memory::RefPtr<RHIShader> CreateDomainShader(const RHIShaderDesc& desc);
		Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderDesc& desc);
		Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderDesc& desc);
		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc& desc);
		Memory::RefPtr<RHIRenderPass> CreateRenderPass(const RHIRenderPassDesc& desc);

		void RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout);
		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer);
		void RemoveTexture(Memory::RefPtr<RHITexture> texture);
		void RemoveSampler(Memory::RefPtr<RHISampler> sampler);
		void RemoveShader(Memory::RefPtr<RHIShader> shader);
		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline);
		void RemoveRenderPass(Memory::RefPtr<RHIRenderPass> pass);

		void InitializeVertexLayout(const RHIVertexLayoutCreateDesc& info, Memory::RefPtr<RHIVertexLayout> layout);
		void InitializeBuffer(const RHIBufferCreateDesc& info, Memory::RefPtr<RHIBuffer> buffer);
		void InitializeTexture(const RHITextureCreateDesc& info, Memory::RefPtr<RHITexture> texture);
		void InitializeSampler(const RHISamplerCreateDesc& info, Memory::RefPtr<RHISampler> sampler);
		void InitializeShader(const RHIShaderCreateDesc& info, Memory::RefPtr<RHIShader> shader);
		void InitializePipeLine(const RHIPipeLineCreateDesc& info, Memory::RefPtr<RHIPipeLine> pipeline);
		void InitializeRenderPass(const RHIRenderPassCreateDesc& info, Memory::RefPtr<RHIRenderPass> pass);

		void UpdateVertexLayout(const RHIVertexLayoutUpdateDesc& info, Memory::RefPtr<RHIVertexLayout> layout);
		void UpdateBuffer(const RHIBufferUpdateDesc& info, Memory::RefPtr<RHIBuffer> buffer);
		void UpdateTexture(const RHITextureUpdateDesc& info, Memory::RefPtr<RHITexture> texture);

		void SetConstant(const RHIConstDesc& info, const Name& binding);
		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout);
		void SetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const Name& binding, const uint32_t offset);
		void SetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset);
		void SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const Name& binding, const uint32_t offset, const uint32_t range);
		void SetTexture(Memory::RefPtr<const RHITexture> texture, const Name& binding);
		void SetSampler(Memory::RefPtr<const RHISampler> sampler, const Name& binding);
		void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline);
		void SetRenderPass(Memory::RefPtr<const RHIRenderPass> pass);

		void UnsetVertexLayout();
		void UnsetVertexBuffer(const Name& binding);
		void UnsetIndexBuffer();
		void UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const Name& binding);
		void UnsetTexture(Memory::RefPtr<const RHITexture> texture, const Name& binding);
		void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const Name& binding);
		void UnsetPipeLine();
		void UnsetRenderPass();

		void Transition(Memory::RefPtr<RHIBuffer> buffer, const RHIBufferState& next);
		void Transition(Memory::RefPtr<RHITexture> texture, const RHITextureState& next);
		
		void DispatchCompute(const RHIDispatchDesc& info);
		
		void DrawDirect(const RHIDrawDirectDesc& info);
		void DrawIndirect(const RHIDrawIndirectDesc& info);
	};
};

#endif // __WTR_RHISYSTEM_H__
