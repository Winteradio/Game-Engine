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

	void RHICommandList::SetStencilState(const RHIStencilState& state)
	{
		Enqueue<RHICommandStencilState>(state);
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

	Memory::RefPtr<RHIShader> RHICommandList::CreateVertexShader(const RHIShaderCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIShader> refShader = m_system->CreateVertexShader(info);

		Enqueue<RHICommandInitializeVertexShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateGeometryShader(const RHIShaderCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIShader> refShader = m_system->CreateGeometryShader(info);

		Enqueue<RHICommandInitializeGeometryShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateHullShader(const RHIShaderCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIShader> refShader = m_system->CreateHullShader(info);
		
		Enqueue<RHICommandInitializeHullShader>(info, refShader);
		
		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreatePixelShader(const RHIShaderCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIShader> refShader = m_system->CreatePixelShader(info);

		Enqueue<RHICommandInitializePixelShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateComputeShader(const RHIShaderCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIShader> refShader = m_system->CreateComputeShader(info);

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

	Memory::RefPtr<RHIRenderTarget> RHICommandList::CreateRenderTarget(const RHIRenderTargetCreateDesc info)
	{
		if (!m_system)
		{
			return nullptr;
		}

		Memory::RefPtr<RHIRenderTarget> refRenderTarget = m_system->CreateRenderTarget(info);

		Enqueue<RHICommandInitializeRenderTarget>(info, refRenderTarget);

		return refRenderTarget;
	}

	void RHICommandList::UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (buffer)
		{
			buffer->SetDesc(info);
		}

		Enqueue<RHICommandUpdateBuffer>(info, buffer);
	}

	void RHICommandList::UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture)
	{
		if (texture)
		{
			texture->SetDesc(info);
		}

		Enqueue<RHICommandUpdateTexture>(info, texture);
	}

	void RHICommandList::UpdateVertexLayout(const RHIVertexLayoutUpdateDesc info, Memory::RefPtr<RHIVertexLayout> layout)
	{
		Enqueue<RHICommandUpdateVertexLayout>(info, layout);
	}

	void RHICommandList::ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (buffer)
		{
			buffer->SetDesc(info);
		}

		Enqueue<RHICommandResizeBuffer>(info, buffer);
	}

	void RHICommandList::ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture)
	{
		if (texture)
		{
			texture->SetDesc(info);
		}

		Enqueue<RHICommandResizeTexture>(info, texture);
	}

	void RHICommandList::RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer)
	{
		Enqueue<RHICommandRemoveBuffer>(buffer);
	}

	void RHICommandList::RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout)
	{
		Enqueue<RHICommandRemoveVertexLayout>(layout);
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

	void RHICommandList::RemoveRenderTarget(Memory::RefPtr<RHIRenderTarget> target)
	{
		Enqueue<RHICommandRemoveRenderTarget>(target);
	}

	void RHICommandList::SetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const RHIConstDesc info, const eResourceSlot slot)
	{
		const std::string slotName = GetSlotName(slot);
		if (slotName.empty())
		{
			return;
		}

		Enqueue<RHICommandSetConstant>(pipeline, info, slotName);
	}

	void RHICommandList::SetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const RHIConstDesc info, const std::string& slotName)
	{
		Enqueue<RHICommandSetConstant>(pipeline, info, slotName);
	}

	void RHICommandList::SetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const eResourceSlot slot)
	{
		const std::string slotName = GetSlotName(slot);
		if (slotName.empty())
		{
			return;
		}

		Enqueue<RHICommandSetBuffer>(pipeline, buffer, slotName);
	}

	void RHICommandList::SetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const std::string& slotName)
	{
		Enqueue<RHICommandSetBuffer>(pipeline, buffer, slotName);
	}

	void RHICommandList::SetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const eResourceSlot slot)
	{
		const std::string slotName = GetSlotName(slot);
		if (slotName.empty())
		{
			return;
		}

		Enqueue<RHICommandSetTexture>(pipeline, texture, slotName);
	}

	void RHICommandList::SetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const std::string& slotName)
	{
		Enqueue<RHICommandSetTexture>(pipeline, texture, slotName);
	}

	void RHICommandList::SetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const eResourceSlot slot)
	{
		const std::string slotName = GetSlotName(slot);
		if (slotName.empty())
		{
			return;
		}

		Enqueue<RHICommandSetSampler>(pipeline, sampler, slotName);
	}

	void RHICommandList::SetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const std::string& slotName)
	{
		Enqueue<RHICommandSetSampler>(pipeline, sampler, slotName);
	}

	void RHICommandList::SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
	{
		Enqueue<RHICommandSetVertexLayout>(layout);
	}

	void RHICommandList::SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
	{
		Enqueue<RHICommandSetPipeLine>(pipeline);
	}

	void RHICommandList::SetRenderTarget(Memory::RefPtr<const RHIRenderTarget> target)
	{
		Enqueue<RHICommandSetRenderTarget>(target);
	}

	void RHICommandList::UnsetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const eResourceSlot slot)
	{
		const std::string slotName = GetSlotName(slot);
		if (slotName.empty())
		{
			return;
		}

		Enqueue<RHICommandUnsetConstant>(pipeline, slotName);
	}

	void RHICommandList::UnsetConstant(Memory::RefPtr<const RHIPipeLine> pipeline, const std::string& slotName)
	{
		Enqueue<RHICommandUnsetConstant>(pipeline, slotName);
	}

	void RHICommandList::UnsetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const eResourceSlot slot)
	{
		const std::string slotName = GetSlotName(slot);
		if (slotName.empty())
		{
			return;
		}

		Enqueue<RHICommandUnsetBuffer>(pipeline, buffer, slotName);
	}

	void RHICommandList::UnsetBuffer(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHIBuffer> buffer, const std::string& slotName)
	{
		Enqueue<RHICommandUnsetBuffer>(pipeline, buffer, slotName);
	}

	void RHICommandList::UnsetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const eResourceSlot slot)
	{
		const std::string slotName = GetSlotName(slot);
		if (slotName.empty())
		{
			return;
		}

		Enqueue<RHICommandUnsetTexture>(pipeline, texture, slotName);
	}

	void RHICommandList::UnsetTexture(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHITexture> texture, const std::string& slotName)
	{
		Enqueue<RHICommandUnsetTexture>(pipeline, texture, slotName);
	}

	void RHICommandList::UnsetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const eResourceSlot slot)
	{
		const std::string slotName = GetSlotName(slot);
		if (slotName.empty())
		{
			return;
		}

		Enqueue<RHICommandUnsetSampler>(pipeline, sampler, slotName);
	}

	void RHICommandList::UnsetSampler(Memory::RefPtr<const RHIPipeLine> pipeline, Memory::RefPtr<const RHISampler> sampler, const std::string& slotName)
	{
		Enqueue<RHICommandUnsetSampler>(pipeline, sampler, slotName);
	}

	void RHICommandList::UnsetVertexLayout()
	{
		Enqueue<RHICommandUnsetVertexLayout>();
	}

	void RHICommandList::UnsetPipeLine()
	{
		Enqueue<RHICommandUnsetPipeLine>();
	}

	void RHICommandList::UnsetRenderTarget()
	{
		Enqueue<RHICommandUnsetRenderTarget>();
	}

	void RHICommandList::DispatchCompute(const RHIDispatchDesc info)
	{
		Enqueue<RHICommandDispatchCompute>(info);
	}

	void RHICommandList::DrawIndexPrimitive(const RHIDrawIndexDesc info)
	{
		Enqueue<RHICommandDrawIndexPrimitive>(info);
	}
}