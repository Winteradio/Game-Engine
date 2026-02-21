#include <RHI/RHICommandExecutor.h>

#include <Memory/include/Core.h>

#include <RHI/RHISystem.h>
#include <RHI/RHICommandList.h>

#include <thread>
#include <Log/include/Log.h>

namespace wtr
{
	RHICommandExecutor::RHICommandExecutor()
		: m_wrotePool()
		, m_submittedPool()
		, m_executedPool()
		, m_writeMutex()
		, m_submitMutex()
	{}

	RHICommandExecutor::~RHICommandExecutor()
	{}

	Memory::RefPtr<RHICommandList> RHICommandExecutor::Acquire()
	{
		Memory::RefPtr<RHICommandList> cmdList;

		{
			std::lock_guard<std::mutex> lock(m_writeMutex);
			if (!m_wrotePool.Empty())
			{
				cmdList = m_wrotePool.Back();
				m_wrotePool.PopBack();
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

	void RHICommandExecutor::Submit(Memory::RefPtr<RHICommandList> cmdList)
	{
		std::lock_guard<std::mutex> lock(m_submitMutex);
		m_submittedPool.PushBack(cmdList);
	}

	void RHICommandExecutor::Execute(Memory::RefPtr<RHISystem> system)
	{
		if (!system)
		{
			return;
		}

		{
			std::lock_guard<std::mutex> lock(m_submitMutex);
			std::swap(m_submittedPool, m_executedPool);
		}

		for (auto cmdList : m_executedPool)
		{
			if (!cmdList)
			{
				continue;
			}

			cmdList->ExecuteAll(system);
		}

		{
			std::lock_guard<std::mutex> lock(m_writeMutex);
			m_wrotePool.Insert(m_wrotePool.End(), m_executedPool.Begin(), m_executedPool.End());
		}

		m_executedPool.Clear();
	}
}