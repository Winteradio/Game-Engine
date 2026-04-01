#include <Renderer/RenderWorker.h>

#include <Renderer/Renderer.h>
#include <RHI/RHIExecutor.h>
#include <RHI/RHICommandList.h>

namespace wtr
{
	RenderWorker::RenderWorker()
		: m_refRenderer(nullptr)
		, m_refExecutor(nullptr)
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

	void RenderWorker::onStart()
	{}

	void RenderWorker::onUpdate()
	{
		if (!m_refRenderer || !m_refExecutor)
		{
			return;
		}

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
	}

	void RenderWorker::onDestroy()
	{
	}
}