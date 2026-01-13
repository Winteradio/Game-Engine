#include <Renderer/RenderWorker.h>

#include <Framework/FrameContext.h>

namespace wtr
{
	RenderWorker::RenderWorker()
		: m_refFrameContext(nullptr)
		, m_renderFunc()
	{}

	RenderWorker::~RenderWorker()
	{}

	void RenderWorker::SetFrameContext(const Memory::RefPtr<FrameContext> frameContext)
	{
		if (frameContext)
		{
			m_refFrameContext = frameContext;
		}
	}

	void RenderWorker::SetFunction(const RenderFunc func)
	{
		if (func)
		{
			m_renderFunc = func;
		}
	}

	void RenderWorker::onStart()
	{}

	void RenderWorker::onUpdate()
	{
		if (m_renderFunc && m_refFrameContext)
		{
			auto& commandList = m_refFrameContext->Acquire(eWorkerType::eConsumer);

			m_renderFunc(commandList);

			m_refFrameContext->Return(eWorkerType::eConsumer, commandList);
		}
	}

	void RenderWorker::onDestroy()
	{
		if (m_refFrameContext)
		{
			m_refFrameContext->Notify(eWorkerType::eConsumer);
		}
	}
}