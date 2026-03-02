#include <RHI/RHIWorker.h>

#include <RHI/RHISystem.h>
#include <RHI/RHIExecutor.h>

namespace wtr
{
	RHIWorker::RHIWorker()
		: m_refSystem(nullptr)
		, m_refFrameExecutor(nullptr)
		, m_refTaskExecutor(nullptr)
	{
	}

	RHIWorker::~RHIWorker()
	{
	}

	void RHIWorker::SetSystem(const Memory::RefPtr<RHISystem> rhiSystem)
	{
		m_refSystem = rhiSystem;
	}

	void RHIWorker::SetFrameExecutor(const Memory::RefPtr<RHIExecutor> executor)
	{
		m_refFrameExecutor = executor;
	}

	void RHIWorker::SetTaskExecutor(const Memory::RefPtr<RHIExecutor> executor)
	{
		m_refTaskExecutor = executor;
	}

	void RHIWorker::onStart()
	{
		m_refSystem->MakeCurrent();
	}

	void RHIWorker::onUpdate()
	{
		if (m_refTaskExecutor)
		{
			m_refTaskExecutor->Execute();
		}

		if (m_refFrameExecutor)
		{
			m_refFrameExecutor->Execute();
		}
	}

	void RHIWorker::onDestroy()
	{
		m_refSystem->ReleaseCurrent();
	}
}