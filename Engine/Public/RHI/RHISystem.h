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
	class RHIBufferView;
	class RHITextureView;

	struct VertexKey;
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

	struct RHITransitionDesc;
	struct RHIBufferState;
	struct RHITextureState;

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
		bool Init(const eRenderType type, const size_t frameDiff, void* nativeHandle);
		bool Ready();
		void Release();
		void FlushPending(const bool wait = true);

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
		Memory::RefPtr<RHIBufferView> CreateBufferView(Memory::RefPtr<RHIBuffer> buffer);
		Memory::RefPtr<RHITextureView> CreateTextureView(Memory::RefPtr<RHITexture> texture);

		void RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout);
		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer);
		void RemoveTexture(Memory::RefPtr<RHITexture> texture);
		void RemoveSampler(Memory::RefPtr<RHISampler> sampler);
		void RemoveShader(Memory::RefPtr<RHIShader> shader);
		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline);
		void RemoveRenderPass(Memory::RefPtr<RHIRenderPass> pass);
		void RemoveBufferView(Memory::RefPtr<RHIBufferView> bufferView);
		void RemoveTextureView(Memory::RefPtr<RHITextureView> textureView);

		void InitializeVertexLayout(Memory::RefPtr<RHIVertexLayout> layout, const RHIVertexLayoutCreateDesc& info);
		void InitializeBuffer(Memory::RefPtr<RHIBufferView> buffer, const RHIBufferCreateDesc& info);
		void InitializeTexture(Memory::RefPtr<RHITextureView> texture, const RHITextureCreateDesc& info);
		void InitializeSampler(Memory::RefPtr<RHISampler> sampler, const RHISamplerCreateDesc& info);
		void InitializeShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc& info);
		void InitializePipeLine(Memory::RefPtr<RHIPipeLine> pipeline, const RHIPipeLineCreateDesc& info);
		void InitializeRenderPass(Memory::RefPtr<RHIRenderPass> pass, const RHIRenderPassCreateDesc& info);
		void InitializeBufferView(Memory::RefPtr<RHIBufferView> bufferView, Memory::RefPtr<RHIBuffer> buffer);
		void InitializeTextureView(Memory::RefPtr<RHITextureView> textureView, Memory::RefPtr<RHITexture> texture);

		void UpdateBuffer(Memory::RefPtr<RHIBufferView> bufferView, const RHIBufferUpdateDesc& info);
		void UpdateTexture(Memory::RefPtr<RHITextureView> textureView, const RHITextureUpdateDesc& info);

		void SetConstant(const RHIConstDesc& info, const Name& binding);
		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout);
		void SetVertexBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const VertexKey& key);
		void SetIndexBuffer(Memory::RefPtr<const RHIBufferView> bufferView);
		void SetBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const Name& binding);
		void SetTexture(Memory::RefPtr<const RHITextureView> textureView, const Name& binding);
		void SetSampler(Memory::RefPtr<const RHISampler> sampler, const Name& binding);
		void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline);
		void SetRenderPass(Memory::RefPtr<const RHIRenderPass> pass);

		void UnsetVertexLayout();
		void UnsetVertexBuffer(const VertexKey& key);
		void UnsetIndexBuffer();
		void UnsetBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const Name& binding);
		void UnsetTexture(Memory::RefPtr<const RHITextureView> textureView, const Name& binding);
		void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const Name& binding);
		void UnsetPipeLine();
		void UnsetRenderPass();

		void Transition(Memory::RefPtr<const RHIBufferView> bufferView, const RHIBufferState& after);
		void Transition(Memory::RefPtr<const RHITextureView> textureView, const RHITextureState& after);
		
		void DispatchCompute(const RHIDispatchDesc& info);
		
		void DrawDirect(const RHIDrawDirectDesc& info);
		void DrawIndirect(const RHIDrawIndirectDesc& info);
	};
};

#endif // __WTR_RHISYSTEM_H__
