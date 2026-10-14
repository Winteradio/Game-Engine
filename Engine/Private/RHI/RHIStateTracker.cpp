#include <RHI/RHIStateTracker.h>

namespace wtr
{
	RHIStateTracker::RHIStateTracker()
		: m_BufferStates()
		, m_TextureStates()
	{
	}

	RHIBufferState RHIStateTracker::GetState(Memory::RefPtr<const RHIBuffer>& buffer) const
	{
		auto itr = m_BufferStates.Find(buffer);
		if (itr != m_BufferStates.End())
		{
			return itr->second;
		}
		
		return RHIBufferState{};
	}

	RHITextureState RHIStateTracker::GetState(Memory::RefPtr<const RHITexture>& texture) const
	{
		auto itr = m_TextureStates.Find(texture);
		if (itr != m_TextureStates.End())
		{
			return itr->second;
		}
		return RHITextureState{};
	}

	void RHIStateTracker::SetState(Memory::RefPtr<const RHIBuffer>& buffer, const RHIBufferState& state)
	{
		m_BufferStates[buffer] = state;
	}

	void RHIStateTracker::SetState(Memory::RefPtr<const RHITexture>& texture, const RHITextureState& state)
	{
		m_TextureStates[texture] = state;
	}

	void RHIStateTracker::RemoveState(Memory::RefPtr<const RHIBuffer>& buffer)
	{
		m_BufferStates.Erase(buffer);
	}

	void RHIStateTracker::RemoveState(Memory::RefPtr<const RHITexture>& texture)
	{
		m_TextureStates.Erase(texture);
	}

	bool RHIStateTracker::HasState(Memory::RefPtr<const RHIBuffer>& buffer) const
	{
		return m_BufferStates.Find(buffer) != m_BufferStates.End();
	}

	bool RHIStateTracker::HasState(Memory::RefPtr<const RHITexture>& texture) const
	{
		return m_TextureStates.Find(texture) != m_TextureStates.End();
	}
}