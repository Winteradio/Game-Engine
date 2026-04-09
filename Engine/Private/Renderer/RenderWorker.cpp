#include <Renderer/RenderWorker.h>

#include <Framework/FrameGate.h>
#include <Renderer/Renderer.h>
#include <RHI/RHIExecutor.h>
#include <RHI/RHICommandList.h>

namespace wtr
{
	RenderWorker::RenderWorker()
		: m_refRenderer(nullptr)
		, m_refExecutor(nullptr)
		, m_refConsumer(nullptr)
		, m_refProducer(nullptr)
	{}

	RenderWorker::~RenderWorker()
	{}

	void RenderWorker::SetRenderer(const Memory::RefPtr<Renderer> renderer)
	{
		if (renderer)
		{
			m_refRenderer = renderer;
		}
	}

	void RenderWorker::SetExecutor(const Memory::RefPtr<RHIExecutor> executor)
	{
		if (executor)
		{
			m_refExecutor = executor;
		}
	}

	void RenderWorker::SetConsumer(const Memory::RefPtr<FrameConsumer> consumer)
	{
		if (consumer)
		{
			m_refConsumer = consumer;
		}
	}

	void RenderWorker::SetProducer(const Memory::RefPtr<FrameProducer> producer)
	{
		if (producer)
		{
			m_refProducer = producer;
		}
	}

	void RenderWorker::onStart()
	{}

	void RenderWorker::onUpdate()
	{
		if (!m_refRenderer || !m_refExecutor || !m_refConsumer || !m_refProducer)
		{
			return;
		}

		m_refConsumer->Acquire();

		auto cmdList = m_refExecutor->Acquire();
		if (!cmdList)
		{
			return;
		}
		
		static size_t frame = 0;
		cmdList->SetFrame(frame++);

		m_refRenderer->Execute(cmdList);
		m_refRenderer->PreDraw(cmdList);
		m_refRenderer->Draw(cmdList);
		m_refRenderer->PostDraw(cmdList);
;
		m_refExecutor->Submit(cmdList);

		m_refProducer->Submit();
	}

	void RenderWorker::onNotify()
	{
		if (m_refConsumer)
		{
			m_refConsumer->NotifyAll();
		}

		if (m_refProducer)
		{
			m_refProducer->NotifyAll();
		}
	}
}