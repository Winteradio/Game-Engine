#ifndef __WTR_RHI_COMMANDLIST_H__
#define __WTR_RHI_COMMANDLIST_H__

#include <Framework/CommandList.h>
#include <RHI/RHICommands.h>

#include <Container/include/DynamicArray.h>
#include <Container/include/HashMap.h>

namespace wtr
{
	class Name;

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

	struct RHIBufferState;
	struct RHITextureState;
};

namespace wtr
{
	class RHICommandList : public CommandList<RHICommandBase>
	{
	public :
		using Super = CommandList<RHICommandBase>;

		RHICommandList();
		~RHICommandList() = default;

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
		void Resize(const uint32_t width, const uint32_t height);
		void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height);

		Memory::RefPtr<RHIVertexLayout> CreateVertexLayout(const RHIVertexLayoutCreateDesc& info);
		Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferCreateDesc& info);
		Memory::RefPtr<RHITexture> CreateTexture(const RHITextureCreateDesc& info);
		Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerCreateDesc& info);
		Memory::RefPtr<RHIShader> CreateVertexShader(const RHIShaderCreateDesc& info);
		Memory::RefPtr<RHIShader> CreateGeometryShader(const RHIShaderCreateDesc& info);
		Memory::RefPtr<RHIShader> CreateHullShader(const RHIShaderCreateDesc& info);
		Memory::RefPtr<RHIShader> CreateDomainShader(const RHIShaderCreateDesc& info);
		Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderCreateDesc& info);
		Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderCreateDesc& info);
		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineCreateDesc& info);
		Memory::RefPtr<RHIRenderPass> CreateRenderPass(const RHIRenderPassCreateDesc& info);

		void RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout);
		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer);
		void RemoveTexture(Memory::RefPtr<RHITexture> texture);
		void RemoveSampler(Memory::RefPtr<RHISampler> sampler);
		void RemoveShader(Memory::RefPtr<RHIShader> shader);
		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline);
		void RemoveRenderPass(Memory::RefPtr<RHIRenderPass> pass);

		void UpdateVertexLayout(const RHIVertexLayoutUpdateDesc& info, Memory::RefPtr<RHIVertexLayout> layout);
		void UpdateBuffer(const RHIBufferUpdateDesc& info, Memory::RefPtr<RHIBuffer> buffer);
		void UpdateTexture(const RHITextureUpdateDesc& info, Memory::RefPtr<RHITexture> texture);

		void SetConstant(const RHIConstDesc& info, const Name& binding);
		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout);
		void SetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const Name& binding, const uint32_t offset);
		void SetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset);
		void SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const Name& binding, const uint32_t offset = 0, const uint32_t range = 0);
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

	private :
		wtr::DynamicArray<RHICommandBase*> m_commands;
		size_t m_frame;
	};
};

#endif // __WTR_RHI_COMMANDLIST_H__
