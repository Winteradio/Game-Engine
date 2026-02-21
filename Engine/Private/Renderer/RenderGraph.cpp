#include <Renderer/RenderGraph.h>

#include <FrameWork/FrameView.h>

#include <Log/include/Log.h>
#include <Memory/include/Core.h>

namespace wtr
{
	RenderGraph::RenderGraph()
		: m_graph()
	{}

	RenderGraph::~RenderGraph()
	{}

	void RenderGraph::Add(Memory::RefPtr<PipeLine> pipeLine)
	{}

	void RenderGraph::Remove(Memory::RefPtr<PipeLine> pipeLine)
	{}

	void RenderGraph::Prepare()
	{}

	void RenderGraph::Draw(const FrameView& frame)
	{}
}