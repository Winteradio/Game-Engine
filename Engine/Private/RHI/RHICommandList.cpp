#include <RHI/RHICommandList.h>

#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIStates.h>
#include <RHI/RHISystem.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	void RHICommandList::RHICache::Clear()
	{
		buffers.Clear();
		textures.Clear();
		samplers.Clear();
		vertices.Clear();

		index = nullptr;
		vertexLayout = nullptr;
		pipeline = nullptr;
		renderPass = nullptr;
	}

	bool RHICommandList::RHIPending::Empty() const
	{
		return buffers.Empty() && textures.Empty();
	}

	void RHICommandList::RHIPending::Clear()
	{
		buffers.Clear();
		textures.Clear();
	}

	RHICommandList::RHICommandList()
		: CommandList<RHICommandBase>()
		, m_cache()
		, m_pending()
		, m_commands()
		, m_frame(0)
	{}

	RHICommandList::~RHICommandList()
	{}

	void RHICommandList::ExecuteAll()
	{
		if (!RHISystem::IsValid())
		{
			return;
		}

		m_cache.Clear();

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
		if (!m_pending.Empty())
		{
			// flush 지점(BeginRenderPass / DispatchCompute / Update*) 없이 기록이 끝남. 추적 상태는 flush에서만 바뀌므로 버려도 어긋나지 않음
			LOGWARN() << "[RHICommandList] The pending transitions were dropped without flush";
		}

		m_pending.Clear();
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

	Memory::RefPtr<RHIBuffer> RHICommandList::CreateBuffer(const RHIBufferCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHIBuffer> refBuffer = RHISystem::CreateBuffer(info);

		Enqueue<RHICommandInitializeBuffer>(info, refBuffer);

		// 백엔드가 업로드 배리어를 모든 읽기로 걸어두므로, 데이터가 있으면 바로 읽을 수 있는 상태에서 시작
		if (refBuffer && !info.blobs.Empty())
		{
			refBuffer->SetBufferState(RHIBufferState::Readable());
		}

		return refBuffer;
	}

	Memory::RefPtr<RHIVertexLayout> RHICommandList::CreateVertexLayout(const RHIVertexLayoutCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHIVertexLayout> refVertexLayout = RHISystem::CreateVertexLayout(info);

		Enqueue<RHICommandInitializeVertexLayout>(info, refVertexLayout);

		return refVertexLayout;
	}

	Memory::RefPtr<RHITexture> RHICommandList::CreateTexture(const RHITextureCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHITexture> refTexture = RHISystem::CreateTexture(info);

		Enqueue<RHICommandInitializeTexture>(info, refTexture);

		// 데이터가 있으면 업로드 후 샘플링 상태, 없으면(어태치먼트 등) 빈 상태에서 시작해 첫 전환이 UNDEFINED에서 출발
		if (refTexture && !info.blobs.Empty())
		{
			refTexture->SetTextureState(RHITextureState::Readable());
		}

		return refTexture;
	}

	Memory::RefPtr<RHISampler> RHICommandList::CreateSampler(const RHISamplerCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHISampler> refSampler = RHISystem::CreateSampler(info);

		Enqueue<RHICommandInitializeSampler>(info, refSampler);

		return refSampler;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateVertexShader(const RHIShaderCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHIShader> refShader = RHISystem::CreateVertexShader(info);

		Enqueue<RHICommandInitializeVertexShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateGeometryShader(const RHIShaderCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHIShader> refShader = RHISystem::CreateGeometryShader(info);

		Enqueue<RHICommandInitializeGeometryShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateHullShader(const RHIShaderCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHIShader> refShader = RHISystem::CreateHullShader(info);

		Enqueue<RHICommandInitializeHullShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateDomainShader(const RHIShaderCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHIShader> refShader = RHISystem::CreateDomainShader(info);

		Enqueue<RHICommandInitializeDomainShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreatePixelShader(const RHIShaderCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHIShader> refShader = RHISystem::CreatePixelShader(info);

		Enqueue<RHICommandInitializePixelShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIShader> RHICommandList::CreateComputeShader(const RHIShaderCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHIShader> refShader = RHISystem::CreateComputeShader(info);

		Enqueue<RHICommandInitializeComputeShader>(info, refShader);

		return refShader;
	}

	Memory::RefPtr<RHIPipeLine> RHICommandList::CreatePipeLine(const RHIPipeLineCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHIPipeLine> refPipeLine = RHISystem::CreatePipeLine(info);

		Enqueue<RHICommandInitializePipeLine>(info, refPipeLine);

		return refPipeLine;
	}

	Memory::RefPtr<RHIRenderPass> RHICommandList::CreateRenderPass(const RHIRenderPassCreateDesc& info)
	{
		if (!RHISystem::IsValid())
		{
			return nullptr;
		}

		Memory::RefPtr<RHIRenderPass> refRenderPass = RHISystem::CreateRenderPass(info);

		Enqueue<RHICommandInitializeRenderPass>(info, refRenderPass);

		return refRenderPass;
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

	void RHICommandList::RemoveRenderPass(Memory::RefPtr<RHIRenderPass> pass)
	{
		Enqueue<RHICommandRemoveRenderPass>(pass);
	}

	void RHICommandList::UpdateBuffer(const RHIBufferUpdateDesc& info, Memory::RefPtr<RHIBuffer> buffer)
	{
		if (buffer)
		{
			buffer->SetDesc(info);
		}

		// 앞서 모인 전환이 업로드보다 먼저 나가야 추적 상태 순서가 맞음
		FlushTransitions();

		Enqueue<RHICommandUpdateBuffer>(info, buffer);

		if (buffer && !info.blobs.Empty())
		{
			buffer->SetBufferState(RHIBufferState::Readable());
		}
	}

	void RHICommandList::UpdateTexture(const RHITextureUpdateDesc& info, Memory::RefPtr<RHITexture> texture)
	{
		if (texture)
		{
			texture->SetDesc(info);
		}

		// 앞서 모인 전환이 업로드보다 먼저 나가야 추적 상태 순서가 맞음
		FlushTransitions();

		Enqueue<RHICommandUpdateTexture>(info, texture);

		if (texture && !info.blobs.Empty())
		{
			texture->SetTextureState(RHITextureState::Readable());
		}
	}

	void RHICommandList::UpdateVertexLayout(const RHIVertexLayoutUpdateDesc& info, Memory::RefPtr<RHIVertexLayout> layout)
	{
		Enqueue<RHICommandUpdateVertexLayout>(info, layout);
	}

	void RHICommandList::SetConstant(const RHIConstDesc& info, const int32_t binding)
	{
		Enqueue<RHICommandSetConstant>(info, binding);
	}

	void RHICommandList::SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding, const uint32_t offset, const uint32_t range)
	{
		// 같은 버퍼라도 offset / range가 다르면 다시 바인딩해야 함
		RHICache::BufferBinding bound;
		bound.buffer = buffer;
		bound.offset = offset;
		bound.range = range;

		if (m_cache.buffers[binding] == bound)
		{
			return;
		}

		m_cache.buffers[binding] = bound;

		Enqueue<RHICommandSetBuffer>(buffer, binding, offset, range);
	}

	void RHICommandList::SetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding)
	{
		if (m_cache.textures[binding] == texture)
		{
			return;
		}

		m_cache.textures[binding] = texture;

		Enqueue<RHICommandSetTexture>(texture, binding);
	}

	void RHICommandList::SetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding)
	{
		if (m_cache.samplers[binding] == sampler)
		{
			return;
		}

		m_cache.samplers[binding] = sampler;

		Enqueue<RHICommandSetSampler>(sampler, binding);
	}

	void RHICommandList::SetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t bindingIndex, const uint32_t offset)
	{
		if (m_cache.vertices[bindingIndex] == buffer)
		{
			return;
		}

		m_cache.vertices[bindingIndex] = buffer;

		Enqueue<RHICommandSetVertexBuffer>(buffer, bindingIndex, offset);
	}

	void RHICommandList::SetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset)
	{
		if (m_cache.index == buffer)
		{
			return;
		}

		m_cache.index = buffer;

		Enqueue<RHICommandSetIndexBuffer>(buffer, offset);
	}

	void RHICommandList::SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
	{
		if (m_cache.vertexLayout == layout)
		{
			return;
		}

		m_cache.vertexLayout = layout;

		Enqueue<RHICommandSetVertexLayout>(layout);
	}

	void RHICommandList::SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
	{
		if (m_cache.pipeline == pipeline)
		{
			return;
		}

		m_cache.pipeline = pipeline;

		Enqueue<RHICommandSetPipeLine>(pipeline);
	}

	void RHICommandList::UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding)
	{
		m_cache.buffers[binding] = RHICache::BufferBinding();

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

	void RHICommandList::UnsetVertexBuffer(const uint32_t bindingIndex)
	{
		m_cache.vertices[bindingIndex] = nullptr;

		Enqueue<RHICommandUnsetVertexBuffer>(bindingIndex);
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

	void RHICommandList::TransitionBuffer(Memory::RefPtr<const RHIBuffer> buffer, const RHIBufferState& state)
	{
		if (!buffer || state.IsEmpty())
		{
			return;
		}

		if (m_cache.renderPass)
		{
			// 렌더 패스 안에서는 배리어를 넣을 수 없어 다음 flush 지점으로 밀림 → 이번 패스에는 적용 안 됨
			LOGWARN() << "[RHICommandList] The buffer transition was requested inside the render pass, deferred to the next flush";
		}

		// 같은 구간에서 이미 요구된 버퍼면 목표만 합친다
		// 현재 상태로 덮이는 요구라도 합쳐야 배리어의 목표(dst)에서 빠지지 않음
		auto itr = m_pending.buffers.Find(buffer);
		if (itr != m_pending.buffers.End())
		{
			itr->second.Merge(state);
			return;
		}

		// 현재 상태로 그대로 쓸 수 있으면 배리어 불필요 (대부분 여기서 끝남)
		if (buffer->GetBufferState().Covers(state))
		{
			return;
		}

		m_pending.buffers[buffer] = state;
	}

	void RHICommandList::TransitionTexture(Memory::RefPtr<const RHITexture> texture, const RHITextureState& state)
	{
		if (!texture || state.IsEmpty())
		{
			return;
		}

		if (m_cache.renderPass)
		{
			// 렌더 패스 안에서는 레이아웃을 바꿀 수 없어 다음 flush 지점으로 밀림 → 이번 패스에는 적용 안 됨
			LOGWARN() << "[RHICommandList] The texture transition was requested inside the render pass, deferred to the next flush";
		}

		auto itr = m_pending.textures.Find(texture);
		if (itr != m_pending.textures.End())
		{
			if (!itr->second.Merge(state))
			{
				// 한 구간에 레이아웃이 두 개 필요 (어태치먼트이면서 샘플링되는 피드백 루프 등). 나중 요구로 덮음
				LOGWARN() << "[RHICommandList] Conflicting texture usages in one pass, usage : "
					<< static_cast<uint32_t>(itr->second.usage) << " -> " << static_cast<uint32_t>(state.usage);

				itr->second = state;
			}

			return;
		}

		if (texture->GetTextureState().Covers(state))
		{
			return;
		}

		m_pending.textures[texture] = state;
	}

	void RHICommandList::FlushTransitions()
	{
		if (m_pending.Empty())
		{
			return;
		}

		if (m_cache.renderPass)
		{
			// VUID-vkCmdPipelineBarrier-bufferMemoryBarrierCount-01178 (버퍼 배리어 금지), oldLayout-01181 (레이아웃 변경 금지)
			LOGWARN() << "[RHICommandList] Can't flush the pending transitions inside the render pass";
			return;
		}

		RHITransitionDesc info;
		info.buffers.Reserve(m_pending.buffers.Size());
		info.textures.Reserve(m_pending.textures.Size());

		for (const auto& [buffer, state] : m_pending.buffers)
		{
			const RHIBufferState before = buffer->GetBufferState();
			info.buffers.PushBack({ buffer, before, state });

			// 읽기 → 읽기면 앞선 읽기도 여전히 유효하니 합쳐서 기억 (같은 stage로 다시 읽을 때 배리어 생략)
			RHIBufferState tracked = state;
			if (!before.HasWrite() && !state.HasWrite())
			{
				tracked.Merge(before);
			}

			buffer->SetBufferState(tracked);
		}

		for (const auto& [texture, state] : m_pending.textures)
		{
			const RHITextureState before = texture->GetTextureState();
			info.textures.PushBack({ texture, before, state });

			// 텍스처는 usage(레이아웃)가 같을 때만 합쳐짐, 다르면 Merge가 실패하고 state 그대로
			RHITextureState tracked = state;
			if (!before.HasWrite() && !state.HasWrite())
			{
				tracked.Merge(before);
			}

			texture->SetTextureState(tracked);
		}

		m_pending.Clear();

		Enqueue<RHICommandTransition>(info);
	}

	void RHICommandList::BeginRenderPass(Memory::RefPtr<const RHIRenderPass> pass, const RHIClearState& clear)
	{
		if (m_cache.renderPass == pass)
		{
			return;
		}

		// 어태치먼트는 패스가 정한다. load / blend로 읽을 수도 있어서 읽기 | 쓰기로 잡음
		if (pass)
		{
			const RHIRenderPassDesc& desc = pass->GetDesc();
			const eDataState attachState = eDataState::eRead | eDataState::eWrite;

			for (const auto& color : desc.colors)
			{
				TransitionTexture(color.texture, RHITextureState(attachState, eDataStage::ePixel, eTextureUsage::eColorAttach));
			}

			TransitionTexture(desc.depthStencil.texture, RHITextureState(attachState, eDataStage::ePixel, eTextureUsage::eDepthStencilAttach));
		}

		FlushTransitions();

		m_cache.renderPass = pass;

		Enqueue<RHICommandBeginRenderPass>(pass, clear);
	}

	void RHICommandList::EndRenderPass()
	{
		m_cache.renderPass = nullptr;

		Enqueue<RHICommandEndRenderPass>();
	}

	void RHICommandList::DispatchCompute(const RHIDispatchDesc& info)
	{
		FlushTransitions();

		Enqueue<RHICommandDispatchCompute>(info);
	}

	void RHICommandList::DrawDirectPrimitive(const RHIDrawDirectDesc& info)
	{
		Enqueue<RHICommandDrawDirectPrimitive>(info);
	}

	void RHICommandList::DrawIndirectPrimitive(const RHIDrawIndirectDesc& info)
	{
		Enqueue<RHICommandDrawIndirectPrimitive>(info);
	}
}
