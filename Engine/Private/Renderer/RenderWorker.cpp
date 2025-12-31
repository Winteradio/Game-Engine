#include <Renderer/RenderWorker.h>

#include <Renderer/RenderContext.h>

namespace wtr
{
	RenderWorker::RenderWorker()
		: m_renderer()
		, m_system()
		, m_refRenderContext(nullptr)
	{}

	RenderWorker::~RenderWorker()
	{}

	bool RenderWorker::Init(const Memory::RefPtr<RenderContext> refRenderContext)
	{
		if (!refRenderContext)
		{
			return false;
		}

		m_refRenderContext = refRenderContext;

		return true;
	}

	void RenderWorker::onStart()
	{}

	void RenderWorker::onUpdate()
	{}

	void RenderWorker::onDestroy()
	{}
}