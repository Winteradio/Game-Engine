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
		, m_cache()
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

	size_t RHICommandList::GetFrame() const
	{
		return m_frame;
	}

	void RHICommandList::Clear(const RHIClearState& state)
	{
		Enqueue<RHICommandClear>(state);
	}

	void RHICommandList::Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height)
	{
		Enqueue<RHICommandResize>(posX, posY, width, height);
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

	void RHICommandList::SetConstant(const RHIConstDesc info, const int32_t binding)
	{
		Enqueue<RHICommandSetConstant>(info, binding);
	}

	void RHICommandList::SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding)
	{
		if (m_cache.buffers[binding] == buffer)
		{
			return;
		}

		Enqueue<RHICommandSetBuffer>(buffer, binding);
	}

	void RHICommandList::SetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding)
	{
		if (m_cache.textures[binding] == texture)
		{
			return;
		}

		Enqueue<RHICommandSetTexture>(texture, binding);
	}

	void RHICommandList::SetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding)
	{
		if (m_cache.samplers[binding] == sampler)
		{
			return;
		}

		Enqueue<RHICommandSetSampler>(sampler, binding);
	}

	void RHICommandList::SetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t bindingIndex, const uint32_t offset)
	{
		if (m_cache.vertices[bindingIndex] == buffer)
		{
			return;
		}

		Enqueue<RHICommandSetVertexBuffer>(buffer, bindingIndex, offset);
	}

	void RHICommandList::SetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset)
	{
		if (m_cache.index == buffer)
		{
			return;
		}

		Enqueue<RHICommandSetIndexBuffer>(buffer, offset);
	}

	void RHICommandList::SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
	{
		if (m_cache.vertexLayout == layout)
		{
			return;
		}

		Enqueue<RHICommandSetVertexLayout>(layout);
	}

	void RHICommandList::SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
	{
		if (m_cache.pipeline == pipeline)
		{
			return;
		}

		Enqueue<RHICommandSetPipeLine>(pipeline);
	}

	void RHICommandList::SetRenderTarget(Memory::RefPtr<const RHIRenderTarget> target)
	{
		if (m_cache.renderTarget == target)
		{
			return;
		}

		Enqueue<RHICommandSetRenderTarget>(target);
	}

	void RHICommandList::ClearRenderTarget(Memory::RefPtr<const RHIRenderTarget> target, const RHIClearState& state)
	{
		Enqueue<RHICommandClearRenderTarget>(target, state);
	}

	void RHICommandList::UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding)
	{
		m_cache.buffers[binding] = nullptr;

		Enqueue<RHICommandUnsetBuffer>(buffer, binding);
	}

	void RHICommandList::UnsetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding)
	{
		m_cache.textures[binding] = nullptr;

		Enqueue<RHICommandUnsetTexture>(texture, binding);
	}

	void RHICommandList::UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding)
	{
		m_cache.samplers[binding] = nullptr;

		Enqueue<RHICommandUnsetSampler>(sampler, binding);
	}

	void RHICommandList::UnsetVertexBuffer(const uint32_t binding)
	{
		m_cache.vertices[binding] = nullptr;

		Enqueue<RHICommandUnsetVertexBuffer>(binding);
	}

	void RHICommandList::UnsetIndexBuffer()
	{
		m_cache.index = nullptr;

		Enqueue<RHICommandUnsetIndexBuffer>();
	}

	void RHICommandList::UnsetVertexLayout()
	{
		m_cache.vertexLayout = nullptr;

		Enqueue<RHICommandUnsetVertexLayout>();
	}

	void RHICommandList::UnsetPipeLine()
	{
		m_cache.pipeline = nullptr;

		Enqueue<RHICommandUnsetPipeLine>();
	}

	void RHICommandList::UnsetRenderTarget()
	{
		m_cache.renderTarget = nullptr;

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