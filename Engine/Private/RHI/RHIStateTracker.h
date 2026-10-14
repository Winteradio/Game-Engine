#ifndef __WTR_RHISTATETRACKER_H__
#define	__WTR_RHISTATETRACKER_H__

#include <RHI/RHIStates.h>
#include <Container/include/HashMap.h>

namespace wtr
{
	class RHIBuffer;
	class RHITexture;
};

namespace wtr
{
	class RHIStateTracker
	{
	public:
		RHIStateTracker();
		virtual ~RHIStateTracker() = default;

	public:
		RHIBufferState GetState(Memory::RefPtr<const RHIBuffer>& buffer) const;
		RHITextureState GetState(Memory::RefPtr<const RHITexture>& texture) const;

		void SetState(Memory::RefPtr<const RHIBuffer>& buffer, const RHIBufferState& state);
		void SetState(Memory::RefPtr<const RHITexture>& texture, const RHITextureState& state);

		void RemoveState(Memory::RefPtr<const RHIBuffer>& buffer);
		void RemoveState(Memory::RefPtr<const RHITexture>& texture);

		bool HasState(Memory::RefPtr<const RHIBuffer>& buffer) const;
		bool HasState(Memory::RefPtr<const RHITexture>& texture) const;

	private :
		wtr::HashMap<Memory::RefPtr<const RHIBuffer>, RHIBufferState> m_BufferStates;
		wtr::HashMap<Memory::RefPtr<const RHITexture>, RHITextureState> m_TextureStates;
	};
};

#endif // __WTR_RHISTATETRACKER_H__