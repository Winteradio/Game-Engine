#include <World/WorldWorker.h>

#include <Framework/Input/InputStorage.h>
#include <Framework/FrameContext.h>

namespace wtr
{
	WorldWorker::WorldWorker()
		: m_timeStep()
		, m_updateFunc()
		, m_renderFunc()
		, m_refInputStorage(nullptr)
		, m_refFrameContext(nullptr)
	{}

	WorldWorker::~WorldWorker()
	{}

	void WorldWorker::SetInputStorage(const Memory::RefPtr<InputStorage> inputStorage)
	{
		if (inputStorage)
		{
			m_refInputStorage = inputStorage;
		}
	}

	void WorldWorker::SetFrameContext(const Memory::RefPtr<FrameContext> frameContext)
	{
		if (frameContext)
		{
			m_refFrameContext = frameContext;
		}
	}

	void WorldWorker::SetFunction(const UpdateFunc func)
	{
		if (func)
		{
			m_updateFunc = func;
		}
	}

	void WorldWorker::SetFunction(const RenderFunc func)
	{
		if (func)
		{
			m_renderFunc = func;
		}
	}

	void WorldWorker::onStart()
	{}

	void WorldWorker::onUpdate()
	{
		m_timeStep.Tick();

		if (m_refInputStorage)
		{
			m_refInputStorage->SwapInput();
		}

		if (m_updateFunc)
		{
			m_updateFunc(m_timeStep);
		}

		if (m_renderFunc && m_refFrameContext)
		{
			auto& frame = m_refFrameContext->Acquire(eWorkerType::eProceduer);
			frame.SetFrame(m_timeStep.frame);

			m_renderFunc(frame);

			m_refFrameContext->Return(eWorkerType::eProceduer, frame);
		}
	}

	void WorldWorker::onDestroy()
	{
		if (m_refFrameContext)
		{
			m_refFrameContext->Notify(eWorkerType::eProceduer);
		}
	}
}