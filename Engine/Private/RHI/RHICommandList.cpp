#include <RHI/RHICommandList.h>

#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIStates.h>
#include <RHI/RHISystem.h>

#include <Memory/include/Core.h>

namespace wtr
{
	RHICommandList::RHICommandList(Memory::RefPtr<RHISystem> system)
		: CommandList<RHICommandBase>()
		, m_system(system)
		, m_commands()
		, m_frame(0)
	{}

	RHICommandList::~RHICommandList()
	{}

	void RHICommandList::ExecuteAll()
	{
		if (!m_system)
		{
			return;
		}

		for (auto* cmd : m_commands)
		{
			if (nullptr != cmd)
			{
				cmd->Execute(m_system);
			}
		}

		Reset();
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

	Memory::RefPtr<RHIBuffer> RHICommandList::CreateBuffer(const RHIBufferCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIBuffer> refBuffer = m_system->CreateBuffer(info);

		Enqueue<RHICommandInitializeBuffer>(info, refBuffer);

		return refBuffer;
	}

	Memory::RefPtr<RHIVertexLayout> RHICommandList::CreateVertexLayout(const RHIVertexLayoutCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}
		
		Memory::RefPtr<RHIVertexLayout> refVertexLayout = m_system->CreateVertexLayout(info);
	
		Enqueue<RHICommandInitializeVertexLayout>(info, refVertexLayout);
		
		return refVertexLayout;
	}

	Memory::RefPtr<RHITexture> RHICommandList::CreateTexture(const RHITextureCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHITexture> refTexture = m_system->CreateTexture(info);

		Enqueue<RHICommandInitializeTexture>(info, refTexture);

		return refTexture;
	}

	Memory::RefPtr<RHISampler> RHICommandList::CreateSampler(const RHISamplerCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHISampler> refSampler = m_system->CreateSampler(info);

		Enqueue<RHICommandInitializeSampler>(info, refSampler);

		return refSampler;
	}

	Memory::RefPtr<RHIVertexShader> RHICommandList::CreateVertexShader(const RHIVertexShaderCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIVertexShader> refShader = m_system->CreateVertexShader(info);

		Enqueue<RHICommandInitializeVertexShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIGeometryShader> RHICommandList::CreateGeometryShader(const RHIGeometryShaderCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIGeometryShader> refShader = m_system->CreateGeometryShader(info);

		Enqueue<RHICommandInitializeGeometryShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIHullShader> RHICommandList::CreateHullShader(const RHIHullShaderCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIHullShader> refShader = m_system->CreateHullShader(info);
		
		Enqueue<RHICommandInitializeHullShader>(info, refShader);
		
		return refShader;
	}

	Memory::RefPtr<RHIPixelShader> RHICommandList::CreatePixelShader(const RHIPixelShaderCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIPixelShader> refShader = m_system->CreatePixelShader(info);

		Enqueue<RHICommandInitializePixelShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIComputeShader> RHICommandList::CreateComputeShader(const RHIComputeShaderCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIComputeShader> refShader = m_system->CreateComputeShader(info);

		Enqueue<RHICommandInitializeComputeShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIPipeLine> RHICommandList::CreatePipeLine(const RHIPipeLineCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}
		
		Memory::RefPtr<RHIPipeLine> refPipeLine = m_system->CreatePipeLine(info);

		Enqueue<RHICommandInitializePipeLine>(info, refPipeLine);

		return refPipeLine;
	}

	void RHICommandList::UpdateBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		Enqueue<RHICommandUpdateBuffer>(info, buffer);
	}

	void RHICommandList::UpdateTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{
		Enqueue<RHICommandUpdateTexture>(info, texture);
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

	void RHICommandList::DrawIndexPrimitive(const RHIDrawIndexDesc info)
	{
		Enqueue<RHICommandDrawIndexPrimitive>(info);
	}
}