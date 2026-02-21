#include <RHI/RHIWorker.h>

#include <RHI/RHISystem.h>
#include <RHI/RHICommandExecutor.h>

namespace wtr
{
	RHIWorker::RHIWorker()
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
	}

	void RHIWorker::onDestroy()
	{
	}
}