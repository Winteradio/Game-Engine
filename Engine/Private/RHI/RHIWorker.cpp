#include <RHI/RHIWorker.h>

#include <RHI/RHISystem.h>
#include <RHI/RHICommandExecutor.h>

namespace wtr
{
	RHIWorker::RHIWorker()
		: m_refSystem(nullptr)
		, m_refExecutor(nullptr)
	{
	}

	RHIWorker::~RHIWorker()
	{
	}

	void RHIWorker::SetSystem(const Memory::RefPtr<RHISystem> rhiSystem)
	{
		m_refSystem = rhiSystem;
	}

	void RHIWorker::SetExecutor(const Memory::RefPtr<RHICommandExecutor> executor)
	{
		m_refExecutor = executor;
	}

	void RHIWorker::onStart()
	{
	}

	void RHIWorker::onUpdate()
	{
		if (!m_refSystem || !m_refExecutor)
		{
			return;
		}

		m_refSystem->MakeCurrent();
		m_refExecutor->Execute(m_refSystem);
		m_refSystem->Present();
		m_refSystem->ReleaseCurrent();
	}

	void RHIWorker::onDestroy()
	{
	}
}