#include <RHI/RHIFrameExecutor.h>

#include <Memory/include/Core.h>

#include <RHI/RHISystem.h>
#include <RHI/RHICommandList.h>

#include <Log/include/Log.h>

namespace wtr
{
	RHIFrameExecutor::RHIFrameExecutor(Memory::RefPtr<RHISystem> system)
		: RHIExecutor(system)
		, m_frameCount(0)
		, m_recordIndex(0)
		, m_beginIndex(0)
		, m_endIndex(0)
		, m_mutexWriting()
		, m_cvWriting()
		, m_listPool()
	{}

	RHIFrameExecutor::~RHIFrameExecutor()
	{}

	Memory::RefPtr<RHICommandList> RHIFrameExecutor::Acquire()
	{
		const size_t now = m_recordIndex;
		const size_t next = GetNext(now);

		if (next == m_beginIndex.load(std::memory_order_acquire))
		{
			std::unique_lock<std::mutex> lock(m_mutexWriting);

			auto checkWritable = [this, next]()
			{
				return next != m_beginIndex.load(std::memory_order_acquire);
			};

			m_cvWriting.wait(lock, checkWritable);
		}

		m_recordIndex = next;

		return m_listPool[now];
	}

	void RHIFrameExecutor::Submit(Memory::RefPtr<RHICommandList> cmdList)
	{
		const size_t recordNow = m_recordIndex;
		const size_t recordPrev = GetPrev(recordNow);

		if (m_listPool[recordPrev] != cmdList)
		{
			return;
		}

		const size_t endNow = m_endIndex.load(std::memory_order_relaxed);
		const size_t endNext = GetNext(endNow);

		m_endIndex.store(endNext, std::memory_order_release);
	}

	void RHIFrameExecutor::Execute()
	{
		if (!m_system)
		{
			return;
		}

		const size_t begin = m_beginIndex.load(std::memory_order_relaxed);
		const size_t end = m_endIndex.load(std::memory_order_acquire);
		size_t now = begin;

		while (now != end)
		{
			Memory::RefPtr<RHICommandList> cmdList = m_listPool[now];
			if (cmdList)
			{
				cmdList->ExecuteAll();
				m_system->Present();
			}
			now = GetNext(now);
		}

		m_beginIndex.store(end, std::memory_order_release);
		m_cvWriting.notify_all();
	}

	bool RHIFrameExecutor::Init(const size_t frameCount)
	{
		if (frameCount < 3)
		{
			return false;
		}

		m_frameCount = frameCount;
		m_listPool.Reserve(m_frameCount);

		for (size_t count = 0; count < m_frameCount; count++)
		{
			Memory::RefPtr<RHICommandList> cmdList = Memory::MakeRef<RHICommandList>(m_system);
			if (!cmdList)
			{
				break;
			}

			m_listPool.PushBack(cmdList);
		}

		if (m_listPool.Size() != m_frameCount)
		{
			return false;
		}

		m_beginIndex = m_frameCount - 1;
		m_endIndex = m_frameCount - 1;

		return true;
	}

	size_t RHIFrameExecutor::GetNext(const size_t index)
	{
		if ((index + 1)>= m_frameCount)
		{
			return 0;
		}
		else
		{
			return index + 1;
		}
	}

	size_t RHIFrameExecutor::GetPrev(const size_t index)
	{
		if (index == 0)
		{
			return m_frameCount - 1;
		}
		else
		{
			return index - 1;
		}
	}
}