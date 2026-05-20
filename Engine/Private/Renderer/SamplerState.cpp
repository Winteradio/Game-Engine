#include <Renderer/SamplerState.h>

#include <RHI/RHIResources.h>
#include <RHI/RHICommandList.h>

#include <Log/include/Log.h>

namespace wtr
{
	SamplerState::SamplerState()
		: m_sampler(nullptr)
		, m_desc()
		, m_slot(eTextureSlot::eNone)
	{}

	void SamplerState::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || m_sampler)
		{
			LOGWARN() << "[SamplerState] Invalid command list or sampler already created.";
			return;
		}

		RHISamplerCreateDesc createDesc;
		createDesc.magFilter = m_desc.magFilter;
		createDesc.minFilter = m_desc.minFilter;
		createDesc.mipFilter = m_desc.mipFilter;
		createDesc.wrapS = m_desc.wrapS;
		createDesc.wrapT = m_desc.wrapT;
		createDesc.wrapR = m_desc.wrapR;

		Memory::RefPtr<RHISampler> sampler = cmdList->CreateSampler(createDesc);
		if (!sampler)
		{
			LOGERROR() << "[SamplerState] Failed to create sampler.";
			return;
		}

		m_sampler = sampler;
	}

	void SamplerState::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_sampler)
		{
			LOGWARN() << "[SamplerState] Invalid command list or sampler not created.";
			return;
		}

		cmdList->RemoveSampler(m_sampler);

		m_sampler = nullptr;
		m_slot = eTextureSlot::eNone;
	}

	void SamplerState::Bind(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_sampler)
		{
			LOGWARN() << "[SamplerState] Invalid command list or sampler not created.";
			return;
		}

		cmdList->SetSampler(m_sampler, static_cast<uint32_t>(m_slot));
	}

	void SamplerState::UnBind(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_sampler)
		{
			LOGWARN() << "[SamplerState] Invalid command list or sampler not created.";
			return;
		}

		cmdList->UnsetSampler(m_sampler, static_cast<uint32_t>(m_slot));
	}

	eResourceState SamplerState::GetResourceState() const
	{
		if (!m_sampler)
		{
			return eResourceState::eNone;
		}

		return m_sampler->GetState();
	}

	void SamplerState::SetTextureSlot(const eTextureSlot slot)
	{
		m_slot = slot;
	}

	void SamplerState::SetSamplerDesc(const RHISamplerDesc& desc)
	{
		m_desc = desc;
	}
}