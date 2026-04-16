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

	struct RHIBufferDesc;
	struct RHIVertexLayoutDesc;
	struct RHITextureDesc;
	struct RHISamplerDesc;
	struct RHIShaderDesc;
	struct RHIPipeLineDesc;
	struct RHIDrawIndexDesc;

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

		template<typename T>
		void* Alloc(const size_t memorySize)
		{
			return m_allocator.Allocate(memorySize, alignof(T));
		}

		void ExecuteAll();
		void Reset();
		void SetFrame(const size_t frame);
		const size_t GetFrame() const;

	public :
		void Clear(const RHIClearState& state);
		void Flush();
		void Present();
		void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height);

		void SetColorState(const RHIColorState& state);
		void SetDepthState(const RHIDepthState& state);
		void SetStencilState(const RHIStencilState& state);
		void SetBlendState(const RHIBlendState& state);
		void SetRasterizerState(const RHIRasterizerState& state);

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

		void UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer);
		void UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture);

		void ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer);
		void ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture);

		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer);
		void RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout);
		void RemoveTexture(Memory::RefPtr<RHITexture> texture);
		void RemoveSampler(Memory::RefPtr<RHISampler> sampler);
		void RemoveShader(Memory::RefPtr<RHIShader> shader);
		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline);

		void SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t slot);
		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout);
		void SetTexture(Memory::RefPtr<const RHITexture> texture, const uint32_t slot);
		void SetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t slot);
		void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline);

		void UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t slot);
		void UnsetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout);
		void UnsetTexture(Memory::RefPtr<const RHITexture> texture, const uint32_t slot);
		void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const uint32_t slot);
		void UnsetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline);
		void DispatchCompute(const RHIDispatchDesc info);
		void DrawIndexPrimitive(const RHIDrawIndexDesc info);

	private :
		Memory::RefPtr<RHISystem> m_system;
		wtr::DynamicArray<RHICommandBase*> m_commands;
		size_t m_frame;
	};
};

#endif // __WTR_RHI_COMMANDLIST_H__