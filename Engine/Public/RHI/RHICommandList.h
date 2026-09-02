#ifndef __WTR_RHI_COMMANDLIST_H__
#define __WTR_RHI_COMMANDLIST_H__

#include <Framework/CommandList.h>
#include <RHI/RHICommands.h>

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>
#include <Container/include/HashMap.h>

namespace wtr
{
	class RHISystem;
	class RHIBuffer;
	class RHIVertexLayout;
	class RHITexture;
	class RHISampler;
	class RHIShader;
	class RHIPipeLine;
	class RHIRenderTarget;

	struct RHIConstDesc;
	struct RHIBufferDesc;
	struct RHIVertexLayoutDesc;
	struct RHITextureDesc;
	struct RHISamplerDesc;
	struct RHIShaderDesc;
	struct RHIPipeLineDesc;
	struct RHIRenderTargetDesc;
	struct RHIDrawIndexDesc;
	struct RHIDispatchDesc;

	struct RHIBufferCreateDesc;
	struct RHIVertexLayoutCreateDesc;
	struct RHITextureCreateDesc;
	struct RHISamplerCreateDesc;
	struct RHIShaderCreateDesc;
	struct RHIPipeLineCreateDesc;
	struct RHIRenderTargetCreateDesc;

	struct RHIBufferUpdateDesc;
	struct RHITextureUpdateDesc;
	struct RHIVertexLayoutUpdateDesc;
};

namespace wtr
{
	class RHICommandList : public CommandList<RHICommandBase>
	{
	public :
		using Super = CommandList<RHICommandBase>;

		RHICommandList(Memory::RefPtr<RHISystem> system);
		~RHICommandList();

		template<typename T, typename... Args>
		void Enqueue(Args&&... args)
		{
			RHICommandBase* command = Create<T, Args...>(std::forward<Args>(args)...);
			if (nullptr != command)
			{
				m_commands.PushBack(command);
			}
		}

		void ExecuteAll();
		void Reset();
		void SetFrame(const size_t frame);
		size_t GetFrame() const;

	public :
		void Clear(const RHIClearState& state);
		void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height);

		Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferCreateDesc info);
		Memory::RefPtr<RHIVertexLayout> CreateVertexLayout(const RHIVertexLayoutCreateDesc info);
		Memory::RefPtr<RHITexture> CreateTexture(const RHITextureCreateDesc info);
		Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerCreateDesc info);
		Memory::RefPtr<RHIShader> CreateVertexShader(const RHIShaderCreateDesc info);
		Memory::RefPtr<RHIShader> CreateGeometryShader(const RHIShaderCreateDesc info);
		Memory::RefPtr<RHIShader> CreateHullShader(const RHIShaderCreateDesc info);
		Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderCreateDesc info);
		Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderCreateDesc info);
		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineCreateDesc info);
		Memory::RefPtr<RHIRenderTarget> CreateRenderTarget(const RHIRenderTargetCreateDesc info);

		void UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer);
		void UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture);
		void UpdateVertexLayout(const RHIVertexLayoutUpdateDesc info, Memory::RefPtr<RHIVertexLayout> layout);

		void ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer);
		void ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture);

		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer);
		void RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout);
		void RemoveTexture(Memory::RefPtr<RHITexture> texture);
		void RemoveSampler(Memory::RefPtr<RHISampler> sampler);
		void RemoveShader(Memory::RefPtr<RHIShader> shader);
		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline);
		void RemoveRenderTarget(Memory::RefPtr<RHIRenderTarget> target);

		void SetConstant(const RHIConstDesc info, const int32_t binding);
		void SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding);
		void SetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding);
		void SetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding);

		void SetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t bindingIndex, const uint32_t offset);
		void SetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset);

		void UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding);
		void UnsetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding);
		void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding);

		void UnsetVertexBuffer(const uint32_t binding);
		void UnsetIndexBuffer();

		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout);
		void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline);
		void SetRenderTarget(Memory::RefPtr<const RHIRenderTarget> target);

		void ClearRenderTarget(Memory::RefPtr<const RHIRenderTarget> target, const RHIClearState& state);

		void UnsetVertexLayout();
		void UnsetPipeLine();
		void UnsetRenderTarget();

		void DispatchCompute(const RHIDispatchDesc info);
		void DrawIndexPrimitive(const RHIDrawIndexDesc info);

	private :
		struct RHICache
		{
			wtr::HashMap<int32_t, Memory::RefPtr<const RHIBuffer>> buffers;
			wtr::HashMap<int32_t, Memory::RefPtr<const RHITexture>> textures;
			wtr::HashMap<int32_t, Memory::RefPtr<const RHISampler>> samplers;
			wtr::HashMap<uint32_t, Memory::RefPtr<const RHIBuffer>> vertices;
			Memory::RefPtr<const RHIBuffer> index;
			Memory::RefPtr<const RHIVertexLayout> vertexLayout;
			Memory::RefPtr<const RHIPipeLine> pipeline;
			Memory::RefPtr<const RHIRenderTarget> renderTarget;
		};

		RHICache m_cache;
		Memory::RefPtr<RHISystem> m_system;
		wtr::DynamicArray<RHICommandBase*> m_commands;
		size_t m_frame;
	};
};

#endif // __WTR_RHI_COMMANDLIST_H__