#ifndef __WTR_COMMANDLIST_H__
#define __WTR_COMMANDLIST_H__

#include <Container/include/LinearArena.h>
#include <Reflection/include/Utils.h>

namespace wtr
{
	template<typename CommandBase>
	class CommandList
	{
	public :
		CommandList()
			: m_allocator()
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
				return nullptr;
			}

			CommandBase* command = new (memory) Command(std::forward<Args>(args)...);

			return command;
		}

		void Reset()
		{
			m_allocator.Reset();
		}

	protected :
		wtr::LinearArena m_allocator;
	};
}

#endif // __WTR_COMMANDLIST_H__