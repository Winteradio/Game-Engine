#ifndef __WTR_SAMPLERSTATE_H__
#define __WTR_SAMPLERSTATE_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Renderer/RenderResource.h>
#include <RHI/RHIDescriptions.h>

#include <cstdint>

namespace wtr
{
	class RHISampler;
	class RHICommandList;
};

namespace wtr
{
	class SamplerState : public RenderState
	{
	public:
		SamplerState();
		virtual ~SamplerState() = default;

	public:
		void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
		void Unload(Memory::RefPtr<RHICommandList> cmdList) override;
		void Bind(Memory::RefPtr<RHICommandList> cmdList) override;
		void UnBind(Memory::RefPtr<RHICommandList> cmdList) override;
		eResourceState GetResourceState() const override;
		
		void SetTextureSlot(const eTextureSlot slot);
		void SetSamplerDesc(const RHISamplerDesc& desc);

	private:
		Memory::RefPtr<RHISampler> m_sampler;
		RHISamplerDesc m_desc;
		eTextureSlot m_slot;
	};
};

#endif // __WTR_SAMPLERSTATE_H__