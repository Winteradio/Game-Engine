#include <RHI/RHITaskExecutor.h>

#include <Memory/include/Core.h>

#include <RHI/RHISystem.h>
#include <RHI/RHICommandList.h>

#include <thread>
#include <Log/include/Log.h>

namespace wtr
{
	RHITaskExecutor::RHITaskExecutor()
		: RHIExecutor()
		, m_listMutex()
		, m_freeMutex()
		, m_listPool()
		, m_freePool()
	{}

	RHITaskExecutor::~RHITaskExecutor()
	{}

	Memory::RefPtr<RHICommandList> RHITaskExecutor::Acquire()
	{
		Memory::RefPtr<RHICommandList> cmdList;

		{
			std::lock_guard<std::mutex> lock(m_freeMutex);

			if (!m_freePool.Empty())
			{
				cmdList = m_freePool.Back();
				m_freePool.PopBack();
			}
		}

		if (cmdList)
		{
			cmdList->Reset();
		}
		else
		{
			cmdList = Memory::MakeRef<RHICommandList>();
		}

		return cmdList;
	}

	void RHITaskExecutor::Submit(Memory::RefPtr<RHICommandList> cmdList)
	{
		std::lock_guard<std::mutex> lock(m_listMutex);

		m_listPool.PushBack(cmdList);
	}

	void RHITaskExecutor::Execute()
	{
		if (!RHISystem::IsValid())
		{
			return;
		}

		static thread_local wtr::List<Memory::RefPtr<RHICommandList>> listPool;
		static thread_local wtr::DynamicArray<Memory::RefPtr<RHICommandList>> freePool;

		listPool.Clear();
		freePool.Clear();

		{
			std::lock_guard<std::mutex> lock(m_listMutex);
			std::swap(m_listPool, listPool);
		}

		RHISystem::BeginFrame();

		while (!listPool.Empty())
		{
			auto cmdList = listPool.Front();
			listPool.PopFront();

			if (cmdList)
			{
				cmdList->ExecuteAll();
				freePool.PushBack(cmdList);
			}
		}

		RHISystem::EndFrame();

		{
			std::lock_guard<std::mutex> lock(m_freeMutex);

			m_freePool.Insert(m_freePool.end(), freePool.begin(), freePool.end());
		}
	}
}