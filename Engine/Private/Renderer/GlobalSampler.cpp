#include <Renderer/SamplerCache.h>

#include <Renderer/SamplerState.h>
#include <RHI/RHIResources.h>
#include <RHI/RHICommandList.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	SamplerCache::SamplerCache()
	{}

	SamplerCache::~SamplerCache()
	{}

	void SamplerCache::RemoveAll(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		for (const auto& [slot, sampler] : m_cache)
		{
			if (sampler)
			{
				sampler->Unload(cmdList);
			}
		}

		m_cache.Clear();
		m_addable.Clear();
	}

	void SamplerCache::Flush(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		for (const auto& sampler : m_addable)
		{
			if (sampler)
			{
				sampler->Upload(cmdList);
			}
		}

		m_addable.Clear();
	}

	Memory::RefPtr<const SamplerState> SamplerCache::GetSampler(const eTextureSlot slot)
	{
		auto itr = m_cache.Find(slot);
		if (itr != m_cache.End())
		{
			return itr->second;
		}

		Memory::RefPtr<SamplerState> samplerState = CreateSampler(slot);
		if (samplerState)
		{
			m_cache.Insert({ slot, samplerState });
			m_addable.Insert(samplerState);
		}

		return samplerState;
	}

	Memory::RefPtr<const SamplerState> SamplerCache::GetSampler(const eTextureSlot slot) const
	{
		const auto itr = m_cache.Find(slot);
		if (itr != m_cache.End())
		{
			return itr->second;
		}
		
		return nullptr;
	}

	Memory::RefPtr<SamplerState> SamplerCache::CreateSampler(const eTextureSlot slot)
	{
		if (slot == eTextureSlot::eNone)
		{
			LOGWARN() << "[SamplerCache] Invalid texture slot.";
			return nullptr;
		}

		Memory::RefPtr<SamplerState> samplerState = Memory::MakeRef<SamplerState>();
		if (!samplerState)
		{
			LOGERROR() << "[SamplerCache] Failed to create sampler state.";
			return nullptr;
		}

		// TODO : Set default sampler description based on the texture slot
		RHISamplerDesc defaultDesc;
		defaultDesc.minFilter = eFilterMode::eLinear;
		defaultDesc.magFilter = eFilterMode::eLinear;
		defaultDesc.mipFilter = eFilterMode::eLinear;
		defaultDesc.wrapS = eWrapMode::eRepeat;
		defaultDesc.wrapT = eWrapMode::eRepeat;
		defaultDesc.wrapR = eWrapMode::eRepeat;

		samplerState->SetTextureSlot(slot);
		samplerState->SetSamplerDesc(defaultDesc);

		return samplerState;
	}
}