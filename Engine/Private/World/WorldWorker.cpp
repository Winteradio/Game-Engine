#include <World/WorldWorker.h>

#include <Framework/Input/InputStorage.h>
#include <Framework/FrameContext.h>

namespace wtr
{
	WorldWorker::WorldWorker()
		: m_timeStep()
		, m_refInputStorage(nullptr)
		, m_refFrameContext(nullptr)
		, m_world()
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

	void WorldWorker::SetWorld(const Memory::ObjectPtr<World> world)
	{
		if (world)
		{
			m_world = world;
		}
	}

	void WorldWorker::onStart()
	{}

	void WorldWorker::onUpdate()
	{
		m_timeStep.Tick();

		if (!m_refInputStorage || !m_world || !m_refFrameContext)
		{
			return;
		}

		m_refInputStorage->SwapInput();

		auto& frame = m_refFrameContext->Acquire(eWorkerType::eProceduer);
		frame.SetFrame(m_timeStep.frame);

		m_world->Update(m_timeStep);
		m_world->Render(frame);

		m_refFrameContext->Return(eWorkerType::eProceduer, frame);
	}

	void WorldWorker::onDestroy()
	{
		if (m_refFrameContext)
		{
			m_refFrameContext->Notify(eWorkerType::eProceduer);
		}
	}
}