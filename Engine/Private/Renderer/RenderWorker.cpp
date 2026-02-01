#include <Renderer/RenderWorker.h>

#include <Framework/FrameContext.h>
#include <Renderer/Renderer.h>
#include <RHI/RHICommandExecutor.h>

namespace wtr
{
	RenderWorker::RenderWorker()
		: m_refFrameContext(nullptr)
		, m_refRenderer(nullptr)
		, m_refExecutor(nullptr)
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

	void RenderWorker::SetRenderer(const Memory::RefPtr<Renderer> renderer)
	{
		if (renderer)
		{
			m_refRenderer = renderer;
		}
	}

	void RenderWorker::SetExecutor(const Memory::RefPtr<RHICommandExecutor> executor)
	{
		if (executor)
		{
			m_refExecutor = executor;
		}
	}

	void RenderWorker::onStart()
	{}

	void RenderWorker::onUpdate()
	{
		if (!m_refFrameContext || !m_refRenderer || !m_refExecutor)
		{
			return;
		}

		auto& frame = m_refFrameContext->Acquire(eWorkerType::eConsumer);
		auto& cmdList = m_refCommandExecutor->Acquire
		m_refRenderer->Render(frame, )
		m_refRenderer->RenderFrame(frame, m_refExecutor);
		m_refFrameContext->Return(eWorkerType::eConsumer, frame);
	}

	void RenderWorker::onDestroy()
	{
		if (m_refFrameContext)
		{
			m_refFrameContext->Notify(eWorkerType::eConsumer);
		}
	}
}