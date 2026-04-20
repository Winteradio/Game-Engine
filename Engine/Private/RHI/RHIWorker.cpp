#include <RHI/RHIWorker.h>

#include <RHI/RHISystem.h>
#include <RHI/RHIExecutor.h>
#include <Framework/FrameGate.h>

namespace wtr
{
	RHIWorker::RHIWorker()
		: m_refSystem(nullptr)
		, m_refFrameExecutor(nullptr)
		, m_refTaskExecutor(nullptr)
		, m_refConsumer(nullptr)
	{
	}

	RHIWorker::~RHIWorker()
	{
	}

	void RHIWorker::SetSystem(const Memory::RefPtr<RHISystem> rhiSystem)
	{
		if (rhiSystem)
		{
			m_refSystem = rhiSystem;
		}
	}

	void RHIWorker::SetFrameExecutor(const Memory::RefPtr<RHIExecutor> executor)
	{
		if (executor)
		{
			m_refFrameExecutor = executor;
		}
	}

	void RHIWorker::SetTaskExecutor(const Memory::RefPtr<RHIExecutor> executor)
	{
		if (executor)
		{
			m_refTaskExecutor = executor;
		}
	}

	void RHIWorker::SetConsumer(const Memory::RefPtr<FrameConsumer> consumer)
	{
		if (consumer)
		{
			m_refConsumer = consumer;
		}
	}

	bool RHIWorker::onStart()
	{
		if (m_refSystem)
		{
			m_refSystem->MakeCurrent();

			return true;
		}
		else
		{
			return false;
		}
	}

	void RHIWorker::onUpdate()
	{
		if (m_refConsumer)
		{
			m_refConsumer->Acquire();
		}

		if (m_refTaskExecutor)
		{
			m_refTaskExecutor->Execute();
		}

		if (m_refFrameExecutor)
		{
			m_refFrameExecutor->Execute();
		}

		if (m_refSystem)
		{
			m_refSystem->FlushPending();
		}
	}

	void RHIWorker::onDestroy()
	{
		if (m_refSystem)
		{
			m_refSystem->Flush();
			m_refSystem->ReleaseCurrent();
		}
	}

	void RHIWorker::onNotify()
	{
		if (m_refConsumer)
		{
			m_refConsumer->NotifyAll();
		}
	}
}