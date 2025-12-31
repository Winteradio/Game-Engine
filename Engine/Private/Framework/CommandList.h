#ifndef __WTR_COMMANDLIST_H__
#define __WTR_COMMANDLIST_H__

#include <cstdint>
#include <atomic>

namespace wtr
{
	enum class eCommandState : uint8_t
	{
		eFree = 0x00,
		eWriting = 0x01,
		eReady = 0x02,
		eReading = 0x03
	};

	template<typename T>
	class CommandList
	{
	public :
		CommandList()
			: m_eState(eCommandState::eFree)
		{}

		virtual ~CommandList()
		{}

	public :
		const eCommandState GetState() const
		{
			return m_eState.load();
		}

		void SetState(const eCommandState state)
		{
			m_eState.exchange(state);
		}

	private :
		std::atomic<eCommandState> m_eState;
	};
}

#endif // __WTR_COMMANDLIST_H__