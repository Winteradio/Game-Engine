#include <Renderer/RenderGraph.h>

#include <Renderer/RenderView.h>

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

	bool RenderGraph::Init()
	{
		Memory::RefPtr<SimpleColor> simpleColor = Memory::MakeRef<SimpleColor>();

		Add(simpleColor);

		return true;
	}

	void RenderGraph::Add(Memory::RefPtr<PipeLine> pipeLine)
	{
		m_graph.Add(pipeLine);
	}

	void RenderGraph::Remove(Memory::RefPtr<PipeLine> pipeLine)
	{
		m_graph.Remove(pipeLine);
	}

	void RenderGraph::Execute(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<RenderScene>& renderScene, const RenderView& renderView)
	{
		if (m_graph.IsUpdated())
		{
			if (!m_graph.Build())
			{
				LOGERROR() << "[RENDER GRAPH] Failed to build the render graph";
				return;
			}
		}

		for (const auto& pipeLine : m_graph.GetSorted())
		{
			if (!pipeLine)
			{
				continue;
			}

			pipeLine->Init(cmdList);
			pipeLine->Draw(renderView, renderScene, cmdList);
		}
	}
}