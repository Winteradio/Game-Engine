#include <RHI/RHICommandList.h>

#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIStates.h>
#include <RHI/RHISystem.h>

#include <Memory/include/Core.h>

namespace wtr
{
	RHICommandList::RHICommandList()
		: CommandList<RHICommandBase>()
		, m_commands()
		, m_frame(0)
	{}

	RHICommandList::~RHICommandList()
	{}

	void RHICommandList::ExecuteAll(Memory::RefPtr<RHISystem> system)
	{
		if (!system)
		{
			return;
		}

		for (auto& cmd : m_commands)
		{
			cmd->Execute(system);
		}
	}

	void RHICommandList::Reset()
	{
		m_commands.Clear();
		Super::Reset();
	}

	void RHICommandList::SetFrame(const size_t frame)
	{
		m_frame = frame;
	}

	const size_t RHICommandList::GetFrame() const
	{
		return m_frame;
	}

	void RHICommandList::Clear(const RHIClearState& state)
	{
		Enqueue<RHICommandClear>(state);
	}

	void RHICommandList::Flush()
	{
		Enqueue<RHICommandFlush>();
	}

	void RHICommandList::Present()
	{
		Enqueue<RHICommandPresent>();
	}

	void RHICommandList::Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height)
	{
		Enqueue<RHICommandResize>(posX, posY, width, height);
	}

	void RHICommandList::SetColorState(const RHIColorState& state)
	{
		Enqueue<RHICommandColorState>(state);
	}

	void RHICommandList::SetDepthState(const RHIDepthState& state)
	{
		Enqueue<RHICommandDepthState>(state);
	}

	void RHICommandList::SetBlendState(const RHIBlendState& state)
	{
		Enqueue<RHICommandBlendState>(state);
	}

	void RHICommandList::SetRasterizerState(const RHIRasterizerState& state)
	{
		Enqueue<RHICommandRasterizerState>(state);
	}

	Memory::RefPtr<RHIBuffer> RHICommandList::CreateBuffer(const RHIBufferDesc& desc)
	{
		Memory::RefPtr<RHIBuffer> refBuffer = Memory::MakeRef<RHIBuffer>(desc);

		Enqueue<RHICommandInitializeBuffer>(desc, refBuffer);

		return refBuffer;
	}

	Memory::RefPtr<RHITexture> RHICommandList::CreateTexture(const RHITextureDesc& desc)
	{
		Memory::RefPtr<RHITexture> refTexture = Memory::MakeRef<RHITexture>(desc);

		Enqueue<RHICommandInitializeTexture>(desc, refTexture);

		return refTexture;
	}

	Memory::RefPtr<RHISampler> RHICommandList::CreateSampler(const RHISamplerDesc& desc)
	{
		Memory::RefPtr<RHISampler> refSampler = Memory::MakeRef<RHISampler>(desc);

		Enqueue<RHICommandInitializeSampler>(desc, refSampler);

		return refSampler;
	}

	Memory::RefPtr<RHIVertexShader> RHICommandList::CreateVertexShader(const RHIVertexShaderDesc& desc)
	{
		Memory::RefPtr<RHIVertexShader> refShader = Memory::MakeRef<RHIVertexShader>(desc);

		Enqueue<RHICommandInitializeVertexShader>(desc, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIGeometryShader> RHICommandList::CreateGeometryShader(const RHIGeometryShaderDesc& desc)
	{
		Memory::RefPtr<RHIGeometryShader> refShader = Memory::MakeRef<RHIGeometryShader>(desc);

		Enqueue<RHICommandInitializeGeometryShader>(desc, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIPixelShader> RHICommandList::CreatePixelShader(const RHIPixelShaderDesc& desc)
	{
		Memory::RefPtr<RHIPixelShader> refShader = Memory::MakeRef<RHIPixelShader>(desc);

		Enqueue<RHICommandInitializePixelShader>(desc, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIComputeShader> RHICommandList::CreateComputeShader(const RHIComputeShaderDesc& desc)
	{
		Memory::RefPtr<RHIComputeShader> refShader = Memory::MakeRef<RHIComputeShader>(desc);

		Enqueue<RHICommandInitializeComputeShader>(desc, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIPipeLine> RHICommandList::CreatePipeLine(const RHIPipeLineDesc& desc)
	{
		Memory::RefPtr<RHIPipeLine> refPipeLine = Memory::MakeRef<RHIPipeLine>(desc);

		Enqueue<RHICommandInitializePipeLine>(desc, refPipeLine);

		return refPipeLine;
	}

	void RHICommandList::UpdateBuffer(const RHIBufferDesc& desc, Memory::RefPtr<RHIBuffer> buffer)
	{
		Enqueue<RHICommandUpdateBuffer>(desc, buffer);
	}

	void RHICommandList::UpdateTexture(const RHITextureDesc& desc, Memory::RefPtr<RHITexture> texture)
	{
		Enqueue<RHICommandUpdateTexture>(desc, texture);
	}

	void RHICommandList::RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer)
	{
		Enqueue<RHICommandRemoveBuffer>(buffer);
	}

	void RHICommandList::RemoveTexture(Memory::RefPtr<RHITexture> texture)
	{
		Enqueue<RHICommandRemoveTexture>(texture);
	}

	void RHICommandList::RemoveSampler(Memory::RefPtr<RHISampler> sampler)
	{
		Enqueue<RHICommandRemoveSampler>(sampler);
	}

	void RHICommandList::RemoveShader(Memory::RefPtr<RHIShader> shader)
	{
		Enqueue<RHICommandRemoveShader>(shader);
	}

	void RHICommandList::RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline)
	{
		Enqueue<RHICommandRemovePipeLine>(pipeline);
	}

	void RHICommandList::DrawIndexPrimitive()
	{
		Enqueue<RHICommandDrawIndexPrimitive>();
	}
}