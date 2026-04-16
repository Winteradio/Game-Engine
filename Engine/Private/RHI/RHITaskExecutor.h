#ifndef __WTR_RHITASKEXECUTOR_H__
#define __WTR_RHITASKEXECUTOR_H__

#include <RHI/RHIExecutor.h>

#include <Container/include/DynamicArray.h>
#include <Container/include/List.h>

#include <mutex>

namespace wtr
{
	class RHITaskExecutor : public RHIExecutor
	{
	public :
		RHITaskExecutor(Memory::RefPtr<RHISystem> system);
		virtual ~RHITaskExecutor();

	public :
		Memory::RefPtr<RHICommandList> Acquire() override;
		void Submit(Memory::RefPtr<RHICommandList> cmdList) override;
		void Execute() override;

	private :
		std::mutex m_listMutex;
		std::mutex m_freeMutex;

		wtr::List<Memory::RefPtr<RHICommandList>> m_listPool;
		wtr::DynamicArray<Memory::RefPtr<RHICommandList>> m_freePool;
	};
};

#endif // __WTR_RHITASKEXECUTOR_H__