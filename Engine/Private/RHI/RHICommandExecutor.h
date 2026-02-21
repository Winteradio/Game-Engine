#ifndef __WTR_RHICOMMANDEXECUTOR_H__
#define __WTR_RHICOMMANDEXECUTOR_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>

#include <mutex>
#include <queue>

namespace wtr
{
	class RHISystem;
	class RHICommandList;
};

namespace wtr
{
	class RHICommandExecutor
	{
	public :
		RHICommandExecutor();
		~RHICommandExecutor();

	public :
		Memory::RefPtr<RHICommandList> Acquire();
		void Submit(Memory::RefPtr<RHICommandList> cmdList);
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
		DynamicArray<Memory::RefPtr<RHICommandList>> m_wrotePool;
		DynamicArray<Memory::RefPtr<RHICommandList>> m_submittedPool;
		DynamicArray<Memory::RefPtr<RHICommandList>> m_executedPool;

		std::mutex m_writeMutex;
		std::mutex m_submitMutex;
	};
};

#endif // __WTR_RHICOMMANDEXECUTOR_H__