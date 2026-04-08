#include <Renderer/RenderGraph.h>

#include <Renderer/RenderView.h>
#include <Renderer/PipeLine/SimpleColor.h>
#include <RHI/RHIResources.h>

#include <Log/include/Log.h>
#include <Memory/include/Core.h>

namespace wtr
{
	RenderGraph::RenderGraph()
		: m_graph()
		, m_addable()
		, m_removable()
		, m_drawCommands()
	{}

	RenderGraph::~RenderGraph()
	{}

	bool RenderGraph::Init()
	{
		Memory::RefPtr<SimpleColor> simpleColor = Memory::MakeRef<SimpleColor>();

		Add(simpleColor);

		return true;
	}

	void RenderGraph::FlushPending()
	{
		FlushAddable();
		FlushRemovable();
	}

	void RenderGraph::FlushAddable()
	{
		auto itr = m_addable.begin();
		while (itr == m_addable.end())
		{
			auto& pipeLine = *itr;
			if (!pipeLine)
			{
				LOGINFO() << "[RENDER GRAPH] Failed to add the pipeline, cause the pipeline is invalid";
				itr = m_addable.Erase(itr);
				continue;
			}

			if (pipeLine->GetResourceState() == eResourceState::eReady)
			{
				itr = m_addable.Erase(itr);
				m_graph.Add(pipeLine);

				continue;
			}
			else if (pipeLine->GetResourceState() == eResourceState::eError)
			{
				LOGINFO() << "[RENDER GRAPH] Failed to add the pipeline, cause the pipeline is in error state";
				itr = m_addable.Erase(itr);

				continue;
			}
			else
			{
				itr++;
			}
		}
	}

	void RenderGraph::FlushRemovable()
	{
		auto itr = m_removable.begin();
		while (itr != m_removable.end())
		{
			auto& pipeLine = *itr;
			if (!pipeLine)
			{
				LOGINFO() << "[RENDER GRAPH] Failed to remove the pipeline, cause the pipeline is invalid";
				itr = m_removable.Erase(itr);
				continue;
			}


			if (pipeLine->GetResourceState() == eResourceState::eNone)
			{
				itr = m_removable.Erase(itr);
				m_graph.Remove(pipeLine);
				continue;
			}
			else if (pipeLine->GetResourceState() == eResourceState::eError)
			{
				LOGINFO() << "[RENDER GRAPH] Failed to remove the pipeline, cause the pipeline is in error state";
				itr = m_removable.Erase(itr);

				m_graph.Remove(pipeLine);

				continue;
			}
			else
			{
				itr++;
			}
		}
	}

	void RenderGraph::Add(Memory::RefPtr<PipeLine> pipeLine)
	{
		if (!pipeLine)
		{
			LOGINFO() << "[RENDER GRAPH] Failed to add the pipeline, cause the pipeline is invalid";
			return;
		}

		m_addable.Insert(pipeLine);

		LOGINFO() << "[RENDER GRAPH] Add the pipeline, ID : " << PipeLineString()(pipeLine);
	}

	void RenderGraph::Remove(Memory::RefPtr<PipeLine> pipeLine)
	{
		if (!pipeLine)
		{
			LOGINFO() << "[RENDER GRAPH] Failed to remove the pipeline, cause the pipeline is invalid";
			return;
		}

		m_removable.Insert(pipeLine);

		LOGINFO() << "[RENDER GRAPH] Remove the pipeline, ID : " << PipeLineString()(pipeLine);
	}

	void RenderGraph::Execute(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<RenderScene> renderScene, const RenderView& renderView)
	{
		if (!cmdList || !renderScene)
		{
			LOGERROR() << "[RENDER GRAPH] Failed to execute the render graph, cause the command list or render scene is invalid";
			return;
		}

		m_drawCommands.Clear();

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
			if (!pipeLine || pipeLine->GetResourceState() == eResourceState::eReady)
			{
				continue;
			}

			pipeLine->Init(cmdList);
			pipeLine->Draw(renderView, renderScene, cmdList);
		}
	}

	Memory::RefPtr<PipeLine> RenderGraph::GetPipeLine(const ECS::UUID& id) const
	{
		for (const auto& pipeLine : m_graph.GetSorted())
		{
			if (pipeLine && pipeLine->GetID() == id)
			{
				return pipeLine;
			}
		}

		return nullptr;
	}

	RenderGraph::PendingPipeLine& RenderGraph::GetAddable()
	{
		return m_addable;
	}

	RenderGraph::PendingPipeLine& RenderGraph::GetRemovable()
	{
		return m_removable;
	}
}