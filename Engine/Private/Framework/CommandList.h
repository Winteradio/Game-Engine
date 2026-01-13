#ifndef __WTR_COMMANDLIST_H__
#define __WTR_COMMANDLIST_H__

#include <cstdint>
#include <atomic>

#include <Container/include/LinearArena.h>

#include <Reflection/include/Utils.h>

namespace wtr
{
	enum class eCommandState : uint8_t
	{
		eFree = 0x00,
		eWriting = 0x01,
		eReady = 0x02,
		eReading = 0x03
	};

	template<typename CommandBase>
	class CommandList
	{
	public :
		CommandList()
			: m_allocator()
			, m_eState(eCommandState::eFree)
		{}

		virtual ~CommandList()
		{}

	public :
		template<typename Command, typename... Args>
		CommandBase* Create(Args&&... args)
		{
			static_assert(Reflection::Utils::IsBase<CommandBase, Command>::value && "The invalid the command type");

			void* memory = m_allocator.Allocate<Command>();
			if (nullptr == memory)
			{
				return;
			}

			CommandBase* command = new (memory) Command(std::forward<Args>(args)...);

			return command;
		}

		void Clear()
		{
			m_allocator.Reset();
		}

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
		wtr::LinearArena m_allocator;
		std::atomic<eCommandState> m_eState;
	};
}

#endif // __WTR_COMMANDLIST_H__