#include <Renderer/RenderWorker.h>

#include <Framework/FrameContext.h>
#include <Renderer/RenderGraph.h>
#include <RHI/RHIExecutor.h>
#include <RHI/RHICommandList.h>

namespace wtr
{
	RenderWorker::RenderWorker()
		: m_refFrameContext(nullptr)
		, m_refGraph(nullptr)
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

	void RenderWorker::SetGraph(const Memory::RefPtr<RenderGraph> graph)
	{
		if (graph)
		{
			m_refGraph = graph;
		}
	}

	void RenderWorker::SetExecutor(const Memory::RefPtr<RHIExecutor> executor)
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
		if (!m_refFrameContext || !m_refGraph || !m_refExecutor)
		{
			return;
		}

		auto& frame = m_refFrameContext->Acquire(eWorkerType::eConsumer);
		auto cmdList = m_refExecutor->Acquire();
		if (!cmdList)
		{
			return;
		}
		
		cmdList->SetFrame(frame.GetFrame());
		m_refGraph->PreDraw(cmdList);
		m_refGraph->Draw(frame, cmdList);
		m_refGraph->PostDraw(cmdList);

		m_refFrameContext->Return(eWorkerType::eConsumer, frame);
		m_refExecutor->Submit(cmdList);
	}

	void RenderWorker::onDestroy()
	{
		if (m_refFrameContext)
		{
			m_refFrameContext->Notify(eWorkerType::eConsumer);
		}
	}
}