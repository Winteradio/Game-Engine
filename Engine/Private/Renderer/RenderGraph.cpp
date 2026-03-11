#include <Renderer/RenderGraph.h>

#include <FrameWork/FrameView.h>
#include <FrameWork/SceneView.h>

#include <Log/include/Log.h>
#include <Memory/include/Core.h>

#include <Renderer/PipeLine/SimpleColor.h>

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

	void RenderGraph::PreDraw(Memory::RefPtr<RHICommandList> cmdList)
	{}

	void RenderGraph::Draw(const FrameView& frame, Memory::RefPtr<RHICommandList> cmdList)
	{
		static SimpleColor color;
		static bool init = false;
		static SceneView sceneView;

		if (!init)
		{
			color.Init(cmdList);

			init = true;
		}

		color.Draw(sceneView, cmdList);
	}

	void RenderGraph::PostDraw(Memory::RefPtr<RHICommandList> cmdList)
	{}
}