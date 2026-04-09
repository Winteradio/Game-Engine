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

	void RenderGraph::Flush(Memory::RefPtr<RHICommandList> cmdList)
	{
		FlushAddable(cmdList);
		FlushRemovable(cmdList);
	}

	void RenderGraph::FlushAddable(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		auto itr = m_addable.begin();
		while (itr == m_addable.end())
		{
			auto& pipeLine = *itr;
			if (!pipeLine)
			{
				LOGINFO() << "[Render Graph] Failed to add the pipeline, cause the pipeline is invalid";
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
				LOGINFO() << "[Render Graph] Failed to add the pipeline, cause the pipeline is in error state";
				itr = m_addable.Erase(itr);

				continue;
			}
			else
			{
				itr++;
			}
		}
	}

	void RenderGraph::FlushRemovable(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		auto itr = m_removable.begin();
		while (itr != m_removable.end())
		{
			auto& pipeLine = *itr;
			if (!pipeLine)
			{
				LOGINFO() << "[Render Graph] Failed to remove the pipeline, cause the pipeline is invalid";
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
				LOGINFO() << "[Render Graph] Failed to remove the pipeline, cause the pipeline is in error state";
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
			LOGINFO() << "[Render Graph] Failed to add the pipeline, cause the pipeline is invalid";
			return;
		}

		m_addable.Insert(pipeLine);

		LOGINFO() << "[Render Graph] Add the pipeline, ID : " << PipeLineString()(pipeLine);
	}

	void RenderGraph::Remove(Memory::RefPtr<PipeLine> pipeLine)
	{
		if (!pipeLine)
		{
			LOGINFO() << "[Render Graph] Failed to remove the pipeline, cause the pipeline is invalid";
			return;
		}

		m_removable.Insert(pipeLine);

		LOGINFO() << "[Render Graph] Remove the pipeline, ID : " << PipeLineString()(pipeLine);
	}

	void RenderGraph::Execute(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<RenderScene> renderScene, const RenderView& renderView)
	{
		if (!cmdList || !renderScene)
		{
			LOGERROR() << "[Render Graph] Failed to execute the render graph, cause the command list or render scene is invalid";
			return;
		}

		m_drawCommands.Clear();

		if (m_graph.IsUpdated())
		{
			if (!m_graph.Build())
			{
				LOGERROR() << "[Render Graph] Failed to build the render graph";
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