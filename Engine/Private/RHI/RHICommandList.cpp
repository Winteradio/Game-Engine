#include <RHI/RHICommandList.h>

#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIStates.h>
#include <RHI/RHISystem.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	RHICommandList::RHICommandList()
		: CommandList<RHICommandBase>()
		, m_commands()
		, m_frame(0)
	{}

	void RHICommandList::ExecuteAll()
	{
		for (auto* cmd : m_commands)
		{
			if (nullptr != cmd)
			{
				cmd->Execute();
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

	size_t RHICommandList::GetFrame() const
	{
		return m_frame;
	}

	void RHICommandList::Resize(const uint32_t width, const uint32_t height)
	{
		Enqueue<RHICommandResize>(width, height);
	}

	void RHICommandList::Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height)
	{
		Enqueue<RHICommandResize>(posX, posY, width, height);
	}

	Memory::RefPtr<RHIVertexLayout> RHICommandList::CreateVertexLayout(const RHIVertexLayoutCreateDesc& info)
	{
		Memory::RefPtr<RHIVertexLayout> refVertexLayout = RHISystem::CreateVertexLayout(info);
		if (!refVertexLayout)
		{
			LOGWARN() << "[RHI] Failed to create vertex layout";
			return false;
		}

		Enqueue<RHICommandInitializeVertexLayout>(refVertexLayout, info);

		return refVertexLayout;
	}

	Memory::RefPtr<RHIBuffer> RHICommandList::CreateBuffer(const RHIBufferCreateDesc& info)
	{
		Memory::RefPtr<RHIBuffer> refBuffer = RHISystem::CreateBuffer(info);
		if (!refBuffer)
		{
			LOGWARN() << "[RHI] Failed to create buffer";
			return false;
		}

		Enqueue<RHICommandInitializeBuffer>(refBuffer, info);

		return refBuffer;
	}

	Memory::RefPtr<RHITexture> RHICommandList::CreateTexture(const RHITextureCreateDesc& info)
	{
		Memory::RefPtr<RHITexture> refTexture = RHISystem::CreateTexture(info);
		if (!refTexture)
		{
			LOGWARN() << "[RHI] Failed to create texture";
			return false;
		}

		Enqueue<RHICommandInitializeTexture>(refTexture, info);

		return refTexture;
	}

	Memory::RefPtr<RHISampler> RHICommandList::CreateSampler(const RHISamplerCreateDesc& info)
	{
		Memory::RefPtr<RHISampler> refSampler = RHISystem::CreateSampler(info);
		if (!refSampler)
		{
			LOGWARN() << "[RHI] Failed to create sampler";
			return false;
		}

		Enqueue<RHICommandInitializeSampler>(refSampler, info);

		return refSampler;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateVertexShader(const RHIShaderCreateDesc& info)
	{
		Memory::RefPtr<RHIShader> refShader = RHISystem::CreateVertexShader(info);
		if (!refShader)
		{
			LOGWARN() << "[RHI] Failed to create vertex shader";
			return false;
		}

		Enqueue<RHICommandInitializeVertexShader>(refShader, info);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateGeometryShader(const RHIShaderCreateDesc& info)
	{
		Memory::RefPtr<RHIShader> refShader = RHISystem::CreateGeometryShader(info);
		if (!refShader)
		{
			LOGWARN() << "[RHI] Failed to create geometry shader";
			return false;
		}

		Enqueue<RHICommandInitializeGeometryShader>(refShader, info);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateHullShader(const RHIShaderCreateDesc& info)
	{
		Memory::RefPtr<RHIShader> refShader = RHISystem::CreateHullShader(info);
		if (!refShader)
		{
			LOGWARN() << "[RHI] Failed to create hull shader";
			return false;
		}

		Enqueue<RHICommandInitializeHullShader>(refShader, info);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateDomainShader(const RHIShaderCreateDesc& info)
	{
		Memory::RefPtr<RHIShader> refShader = RHISystem::CreateDomainShader(info);
		if (!refShader)
		{
			LOGWARN() << "[RHI] Failed to create domain shader";
			return false;
		}

		Enqueue<RHICommandInitializeDomainShader>(refShader, info);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreatePixelShader(const RHIShaderCreateDesc& info)
	{
		Memory::RefPtr<RHIShader> refShader = RHISystem::CreatePixelShader(info);
		if (!refShader)
		{
			LOGWARN() << "[RHI] Failed to create pixel shader";
			return false;
		}

		Enqueue<RHICommandInitializePixelShader>(refShader, info);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateComputeShader(const RHIShaderCreateDesc& info)
	{
		Memory::RefPtr<RHIShader> refShader = RHISystem::CreateComputeShader(info);
		if (!refShader)
		{
			LOGWARN() << "[RHI] Failed to create compute shader";
			return false;
		}

		Enqueue<RHICommandInitializeComputeShader>(refShader, info);

		return refShader;
	}

	Memory::RefPtr<RHIPipeLine> RHICommandList::CreatePipeLine(const RHIPipeLineCreateDesc& info)
	{
		Memory::RefPtr<RHIPipeLine> refPipeLine = RHISystem::CreatePipeLine(info);
		if (!refPipeLine)
		{
			LOGWARN() << "[RHI] Failed to create pipeline";
			return false;
		}

		Enqueue<RHICommandInitializePipeLine>(refPipeLine, info);

		return refPipeLine;
	}

	Memory::RefPtr<RHIRenderPass> RHICommandList::CreateRenderPass(const RHIRenderPassCreateDesc& info)
	{
		Memory::RefPtr<RHIRenderPass> refRenderPass = RHISystem::CreateRenderPass(info);
		if (!refRenderPass)
		{
			LOGWARN() << "[RHI] Failed to create render pass";
			return false;
		}

		Enqueue<RHICommandInitializeRenderPass>(refRenderPass, info);

		return refRenderPass;
	}

	void RHICommandList::RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout)
	{
		if (layout)
		{
			Enqueue<RHICommandRemoveVertexLayout>(layout);
		}
	}

	void RHICommandList::RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer)
	{
		if (buffer)
		{
			Enqueue<RHICommandRemoveBuffer>(buffer);
		}
	}

	void RHICommandList::RemoveTexture(Memory::RefPtr<RHITexture> texture)
	{
		if (texture)
		{
			Enqueue<RHICommandRemoveTexture>(texture);
		}
	}

	void RHICommandList::RemoveSampler(Memory::RefPtr<RHISampler> sampler)
	{
		if (sampler)
		{
			Enqueue<RHICommandRemoveSampler>(sampler);
		}
	}

	void RHICommandList::RemoveShader(Memory::RefPtr<RHIShader> shader)
	{
		if (shader)
		{
			Enqueue<RHICommandRemoveShader>(shader);
		}
	}

	void RHICommandList::RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline)
	{
		if (pipeline)
		{
			Enqueue<RHICommandRemovePipeLine>(pipeline);
		}
	}

	void RHICommandList::RemoveRenderPass(Memory::RefPtr<RHIRenderPass> pass)
	{
		if (pass)
		{
			Enqueue<RHICommandRemoveRenderPass>(pass);
		}
	}

	void RHICommandList::UpdateVertexLayout(Memory::RefPtr<RHIVertexLayout> layout, const RHIVertexLayoutUpdateDesc& info)
	{
		Enqueue<RHICommandUpdateVertexLayout>(layout, info);
	}

	void RHICommandList::UpdateBuffer(Memory::RefPtr<RHIBuffer> buffer, const RHIBufferUpdateDesc& info)
	{
		if (!buffer)
		{
			LOGWARN() << "[RHI] Failed to update buffer: Buffer is null";
			return;
		}

		Enqueue<RHICommandUpdateBuffer>(buffer, info);
	}

	void RHICommandList::UpdateTexture(Memory::RefPtr<RHITexture> texture, const RHITextureUpdateDesc& info)
	{
		if (!texture)
		{
			LOGWARN() << "[RHI] Failed to update texture: Texture is null";
			return;
		}

		Enqueue<RHICommandUpdateTexture>(texture, info);
	}

	void RHICommandList::SetConstant(const RHIConstDesc& info, const Name& binding)
	{
		Enqueue<RHICommandSetConstant>(info, binding);
	}

	void RHICommandList::SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
	{
		Enqueue<RHICommandSetVertexLayout>(layout);
	}

	void RHICommandList::SetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const VertexKey& key, const uint32_t offset)
	{
		Enqueue<RHICommandSetVertexBuffer>(buffer, key, offset);
	}

	void RHICommandList::SetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset)
	{
		Enqueue<RHICommandSetIndexBuffer>(buffer, offset);
	}

	void RHICommandList::SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const Name& binding, const uint32_t offset, const uint32_t range)
	{
		Enqueue<RHICommandSetBuffer>(buffer, binding, offset, range);
	}

	void RHICommandList::SetTexture(Memory::RefPtr<const RHITexture> texture, const Name& binding)
	{
		Enqueue<RHICommandSetTexture>(texture, binding);
	}

	void RHICommandList::SetSampler(Memory::RefPtr<const RHISampler> sampler, const Name& binding)
	{
		Enqueue<RHICommandSetSampler>(sampler, binding);
	}

	void RHICommandList::SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
	{
		Enqueue<RHICommandSetPipeLine>(pipeline);
	}

	void RHICommandList::SetRenderPass(Memory::RefPtr<const RHIRenderPass> pass)
	{
		Enqueue<RHICommandSetRenderPass>(pass);
	}

	void RHICommandList::UnsetVertexLayout()
	{
		Enqueue<RHICommandUnsetVertexLayout>();
	}

	void RHICommandList::UnsetVertexBuffer(const Name& binding)
	{
		Enqueue<RHICommandUnsetVertexBuffer>(binding);
	}

	void RHICommandList::UnsetIndexBuffer()
	{
		Enqueue<RHICommandUnsetIndexBuffer>();
	}

	void RHICommandList::UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const Name& binding)
	{
		Enqueue<RHICommandUnsetBuffer>(buffer, binding);
	}

	void RHICommandList::UnsetTexture(Memory::RefPtr<const RHITexture> texture, const Name& binding)
	{
		Enqueue<RHICommandUnsetTexture>(texture, binding);
	}

	void RHICommandList::UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const Name& binding)
	{
		Enqueue<RHICommandUnsetSampler>(sampler, binding);
	}

	void RHICommandList::UnsetPipeLine()
	{
		Enqueue<RHICommandUnsetPipeLine>();
	}

	void RHICommandList::UnsetRenderPass()
	{
		Enqueue<RHICommandUnsetRenderPass>();
	}

	void RHICommandList::Transition(Memory::RefPtr<const RHIBuffer> buffer, const RHIBufferState& state)
	{
		Enqueue<RHICommandTransitionBuffer>(buffer, state);
	}

	void RHICommandList::Transition(Memory::RefPtr<const RHITexture> texture, const RHITextureState& state)
	{
		Enqueue<RHICommandTransitionTexture>(texture, state);
	}

	void RHICommandList::DispatchCompute(const RHIDispatchDesc& info)
	{
		Enqueue<RHICommandDispatchCompute>(info);
	}

	void RHICommandList::DrawDirect(const RHIDrawDirectDesc& info)
	{
		Enqueue<RHICommandDrawDirect>(info);
	}

	void RHICommandList::DrawIndirect(const RHIDrawIndirectDesc& info)
	{
		Enqueue<RHICommandDrawIndirect>(info);
	}
}
