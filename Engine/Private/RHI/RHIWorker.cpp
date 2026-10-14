#include <RHI/RHIWorker.h>

#include <RHI/RHISystem.h>
#include <RHI/RHIExecutor.h>
#include <Framework/FrameGate.h>

namespace wtr
{
	RHIWorker::RHIWorker()
		: m_refFrameExecutor(nullptr)
		, m_refTaskExecutor(nullptr)
		, m_refConsumer(nullptr)
	{
	}

	RHIWorker::~RHIWorker()
	{
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
		if (RHISystem::IsValid())
		{
			RHISystem::BeginContext();

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

		RHISystem::FlushPending();
	}

	void RHIWorker::onDestroy()
	{
		RHISystem::EndContext();
	}

	void RHIWorker::onNotify()
	{
		if (m_refConsumer)
		{
			m_refConsumer->NotifyAll();
		}
	}
}