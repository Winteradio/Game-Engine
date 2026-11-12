#include <RHI/RHIStateTracker.h>

#include <RHI/RHIResources.h>

#include <algorithm>

namespace wtr
{
	RHIStateTracker::RHIStateTracker()
		: m_buffers()
		, m_textures()
	{
	}

	void RHIStateTracker::AddState(Memory::RefPtr<const RHIBuffer> buffer, const BufferState& state)
	{
		if (!buffer || state.size == 0)
		{
			return;
		}

		auto& stateMap = m_buffers[buffer];
		if (stateMap.empty())
		{
			stateMap[state.offset] = state;
			return;
		}

		auto FindOverlap = [&stateMap](const BufferState& state)
		{
			auto nextItr = stateMap.lower_bound(state.offset);
			if (nextItr != stateMap.begin())
			{
				auto prevItr = std::prev(nextItr);
				if (prevItr->second.offset + prevItr->second.size > state.offset)
				{
					return prevItr;
				}
			}

			if (nextItr != stateMap.end())
			{
				if (state.offset + state.size > nextItr->second.offset)
				{
					return nextItr;
				}
			}

			return stateMap.end();
		};

		BufferState newState = state;
		while (newState.size != 0)
		{
			auto itrOrigin = FindOverlap(newState);
			if (itrOrigin == stateMap.end())
			{
				stateMap[newState.offset] = newState;
				break;
			}

			const BufferState originState = itrOrigin->second;

			BufferState overlapedState;
			overlapedState.offset = std::max(newState.offset, originState.offset);
			overlapedState.size = std::min(newState.offset + newState.size, originState.offset + originState.size) - overlapedState.offset;
			overlapedState.prev = originState.prev;
			overlapedState.next.stage = newState.next.stage | originState.next.stage;
			overlapedState.next.state = newState.next.state | originState.next.state;
			overlapedState.next.usage = newState.next.usage | originState.next.usage;

			if (overlapedState.size != 0)
			{
				stateMap.erase(itrOrigin);

				if (originState.offset < overlapedState.offset)
				{
					BufferState leftState = originState;
					leftState.size = overlapedState.offset - originState.offset;

					stateMap[leftState.offset] = leftState;
				}

				if (originState.offset + originState.size > overlapedState.offset + overlapedState.size)
				{
					BufferState rightState = originState;
					rightState.offset = overlapedState.offset + overlapedState.size;
					rightState.size = (originState.offset + originState.size) - rightState.offset;

					stateMap[rightState.offset] = rightState;
				}

				stateMap[overlapedState.offset] = overlapedState;

				if (newState.offset < originState.offset)
				{
					BufferState gapState = newState;
					gapState.size = originState.offset - newState.offset;

					stateMap[gapState.offset] = gapState;
				}
			}

			newState.offset = overlapedState.offset + overlapedState.size;
			newState.size = (state.offset + state.size) - newState.offset;
		}
	}

	void RHIStateTracker::AddState(Memory::RefPtr<const RHITexture> texture, const TextureState& state)
	{
		if (!texture)
		{
			return;
		}

		auto& stateMap = m_textures[texture];
		if (stateMap.empty())
		{
			stateMap[state.range] = state;
			return;
		}

		auto& originState = stateMap[state.range];
		originState.next.stage = originState.next.stage | state.next.stage;
		originState.next.state = originState.next.state | state.next.state;
		originState.next.usage = originState.next.usage | state.next.usage;
	}

	void RHIStateTracker::RemoveState(Memory::RefPtr<const RHIBuffer> buffer)
	{
		if (!buffer)
		{
			return;
		}

		m_buffers.Erase(buffer);
	}

	void RHIStateTracker::RemoveState(Memory::RefPtr<const RHITexture> texture)
	{
		if (!texture)
		{
			return;
		}

		m_textures.Erase(texture);
	}

	const RHIStateTracker::BufferMap RHIStateTracker::FlushBuffer()
	{
		BufferMap flushedBuffers;
		for (auto& [buffer, stateMap] : m_buffers)
		{
			for (auto& [offset, state] : stateMap)
			{
				if (state.prev != state.next)
				{
					flushedBuffers[buffer][offset] = state;
					state.prev = state.next;
				}
			}
		}

		MergeBufferState();

		return flushedBuffers;
	}

	const RHIStateTracker::TextureMap RHIStateTracker::FlushTexture()
	{
		TextureMap flushedTextures;
		for (auto& [texture, stateMap] : m_textures)
		{
			for (auto& [baseLayer, state] : stateMap)
			{
				if (state.prev != state.next)
				{
					flushedTextures[texture][baseLayer] = state;
					state.prev = state.next;
				}
			}
		}

		MergeTextureState();

		return flushedTextures;
	}

	void RHIStateTracker::MergeBufferState()
	{
		for (auto& [buffer, stateMap] : m_buffers)
		{
			if (stateMap.empty())
			{
				continue;
			}

			auto baseItr = stateMap.begin();
			auto nextItr = std::next(baseItr);
			
			for (; nextItr != stateMap.end();)
			{
				BufferState& baseState = baseItr->second;
				BufferState& nextState = nextItr->second;
				if (baseState.offset + baseState.size == nextState.offset &&
					baseState.next == nextState.next && baseState.prev == nextState.prev)
				{
					baseState.size += nextState.size;
					nextItr = stateMap.erase(nextItr);
				}
				else
				{
					baseItr = nextItr;
					nextItr++;
				}
			}
		}
	}

	void RHIStateTracker::MergeTextureState()
	{
		for (auto& [texture, stateMap] : m_textures)
		{
			if (stateMap.empty())
			{
				continue;
			}

			auto baseItr = stateMap.begin();
			auto nextItr = std::next(baseItr);

			for (; nextItr != stateMap.end();)
			{
				TextureState& baseState = baseItr->second;
				TextureState& nextState = nextItr->second;
				if (baseState.range.baseLayer + baseState.range.layerCount == nextState.range.baseLayer &&
					baseState.range.baseMip + baseState.range.mipCount == nextState.range.baseMip &&
					baseState.next == nextState.next && baseState.prev == nextState.prev)
				{
					baseState.range.layerCount += nextState.range.layerCount;
					baseState.range.mipCount += nextState.range.mipCount;
					nextItr = stateMap.erase(nextItr);
				}
				else
				{
					baseItr = nextItr;
					nextItr++;
				}
			}
		}
	}
}