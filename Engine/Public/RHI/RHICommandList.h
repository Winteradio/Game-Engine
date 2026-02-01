#ifndef __WTR_RHI_COMMANDLIST_H__
#define __WTR_RHI_COMMANDLIST_H__

#include <Framework/CommandList.h>
#include <RHI/RHICommands.h>

#include <Container/include/DynamicArray.h>
#include <Container/include/HashMap.h>

namespace wtr
{
	class RHICommandList : public CommandList<RHICommandBase>
	{
	public :
		RHICommandList();
		~RHICommandList();

		template<typename T, typename... Args>
		void Enqueue(Args&&... args)
		{
			RHICommandBase* command = Create<T, Args...>(std::forward<Args>(args)...);

			m_commands.PushBack(command);
		}

		void ExecuteAll();
		void Clear();

	private :

		wtr::DynamicArray<RHICommandBase*> m_commands;
	};
};

#endif // __WTR_RHI_COMMANDLIST_H__