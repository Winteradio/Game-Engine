#include <Renderer/RenderGraph.h>

#include <Renderer/MeshBatch.h>
#include <Renderer/RenderScene.h>
#include <Renderer/RenderView.h>
#include <Renderer/GlobalResource.h>
#include <Renderer/PipeLine/SimpleColor.h>
#include <RHI/RHIResources.h>
#include <RHI/RHICommandList.h>

#include <Log/include/Log.h>
#include <Memory/include/Core.h>

namespace wtr
{
	RenderGraph::RenderGraph()
		: Graph()
		, m_addable()
		, m_removable()
		, m_drawCommands()
	{}

	RenderGraph::~RenderGraph()
	{}

	bool RenderGraph::Init(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!InitResource(cmdList))
		{
			LOGERROR() << "[Render Graph] Failed to initialize the render graph resource";
			return false;
		}

		if (!InitPipeLine())
		{
			LOGERROR() << "[Render Graph] Failed to initialize the render graph pipeline";
			return false;
		}

		return true;
	}

	bool RenderGraph::InitResource(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			LOGERROR() << "[Render Graph] Failed to initialize the render graph resource, cause the command list is invalid";
			return false;
		}

		m_globalResource = Memory::MakeRef<GlobalResource>();
		if (!m_globalResource)
		{
			LOGERROR() << "[Render Graph] Failed to create the global resource";
			return false;
		}

		if (!m_globalResource->Init(cmdList))
		{
			LOGERROR() << "[Render Graph] Failed to initialize the global resource";
			return false;
		}

		return true;
	}

	bool RenderGraph::InitPipeLine()
	{
		if (Memory::RefPtr<SimpleColor> simpleColor = Create<SimpleColor>())
		{
			simpleColor->Init();
			Add(simpleColor);
		}
		else
		{
			LOGERROR() << "[Render Graph] Failed to create the simple color pipeline";
			return false;
		}

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
		while (itr != m_addable.end())
		{
			auto& pipeLine = *itr;
			if (!pipeLine)
			{
				LOGINFO() << "[Render Graph] Failed to add the pipeline, cause the pipeline is invalid";
				itr = m_addable.Erase(itr);
				continue;
			}

			const eResourceState state = pipeLine->GetShaderState();
			if (state == eResourceState::eReady)
			{
				itr = m_addable.Erase(itr);

				pipeLine->Upload(cmdList);

				continue;
			}
			else if (state == eResourceState::eError)
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

			const eResourceState state = pipeLine->GetResourceState();
			if (state >= eResourceState::eNone)
			{
				pipeLine->Unload(cmdList);
			}
			else
			{
				LOGINFO() << "[Render Graph] Failed to remove the pipeline, the state(" << static_cast<uint8_t>(state) << ") is invalid, ID " << pipeLine->GetID().ToString();
			}

			itr = m_removable.Erase(itr);
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

		Super::Remove(pipeLine->GetID());
		m_removable.Insert(pipeLine);

		LOGINFO() << "[Render Graph] Remove the pipeline, ID : " << PipeLineString()(pipeLine);
	}

	void RenderGraph::Execute(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<RenderScene> renderScene, const RenderView& renderView)
	{
		if (!cmdList || !renderScene || !m_globalResource)
		{
			LOGERROR() << "[Render Graph] Failed to execute the render graph, cause the command list or render scene is invalid";
			return;
		}

		m_globalResource->UpdateCamera(renderView, cmdList);

		m_drawCommands.Clear();
		const auto& meshBatches = renderScene->GetMeshBatches();
		for (const auto& batchPair : meshBatches)
		{
			const auto& batch = batchPair.second;
			if (batch && batch->GetResourceState() == eResourceState::eReady)
			{
				m_drawCommands.PushBack(batch->GetDrawCommand());
			}
		}

		cmdList->Resize(renderView.viewport.posX, renderView.viewport.posY, renderView.viewport.width, renderView.viewport.height);

		for (const auto& pipeLine : GetSorted())
		{
			if (!pipeLine || pipeLine->GetResourceState() != eResourceState::eReady)
			{
				continue;
			}

			pipeLine->Execute(m_drawCommands, m_globalResource, cmdList);
		}
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