#include <Renderer/RenderGraph.h>

#include <Renderer/MeshBatch.h>
#include <Renderer/RenderScene.h>
#include <Renderer/RenderView.h>
#include <Renderer/GlobalRenderer.h>
#include <Renderer/Proxy/LightProxy.h>
#include <Renderer/RenderPass/TransformPass.h>
#include <Renderer/RenderPass/CullingPass.h>
#include <Renderer/RenderPass/GeometryPass.h>
#include <Renderer/RenderPass/LightingPass.h>
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
		Memory::RefPtr<TransformPass> transformPass = Memory::MakeRef<TransformPass>();
		//Memory::RefPtr<CullingPass> cullingPass = Memory::MakeRef<CullingPass>();
		Memory::RefPtr<GeometryPass> geometryPass = Memory::MakeRef<GeometryPass>();
		Memory::RefPtr<LightingPass> lightingPass = Memory::MakeRef<LightingPass>();

		if (!transformPass || !geometryPass || !lightingPass)
		//if (!transformPass || !cullingPass || !geometryPass || !lightingPass)
		{
			return false;
		}

		geometryPass->InitState();
		lightingPass->InitState();

		Add(transformPass);
		//Add(cullingPass);
		Add(geometryPass);
		Add(lightingPass);

		auto& transformNode = GetNode(transformPass->GetID());
		//auto& cullingNode = GetNode(cullingPass->GetID());
		auto& geometryNode = GetNode(geometryPass->GetID());
		auto& lightingNode = GetNode(lightingPass->GetID());

		geometryNode.SetDependency(transformNode);
		lightingNode.SetDependency(geometryNode);

		return GlobalResource::Init(cmdList);
	}

	void RenderGraph::RemoveAll(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		m_addable.Clear();
		FlushRemovable(cmdList);

		const auto& sortedNode = GetSorted();
		for (auto& renderPass : sortedNode)
		{
			if (renderPass)
			{
				renderPass->Unload(cmdList);
			}
		}

		GlobalResource::Release(cmdList);
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
			auto& renderpass = *itr;
			if (!renderpass)
			{
				LOGINFO() << "[Render Graph] Failed to add the render pass, cause the render pass is invalid";
				itr = m_addable.Erase(itr);
				continue;
			}

			const eResourceState state = renderpass->GetResourceState();
			if (state == eResourceState::eReady)
			{
				itr = m_addable.Erase(itr);

				continue;
			}
			else if (state == eResourceState::eError)
			{
				LOGINFO() << "[Render Graph] Failed to add the render pass, cause the render pass is in error state";
				itr = m_addable.Erase(itr);

				continue;
			}
			else
			{
				renderpass->Upload(cmdList);

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
			auto& renderPass = *itr;
			if (!renderPass)
			{
				LOGINFO() << "[Render Graph] Failed to remove the render pass, cause the render pass is invalid";
				itr = m_removable.Erase(itr);
				continue;
			}

			const eResourceState state = renderPass->GetResourceState();
			if (state != eResourceState::eNone)
			{
				renderPass->Unload(cmdList);
			}
			else
			{
				const Reflection::TypeInfo* typeInfo = renderPass->GetTypeInfo();
				LOGINFO() << "[Render Graph] Failed to remove the render pass, the state(" << static_cast<uint8_t>(state) << ") is invalid, ID " << typeInfo->GetTypeName();
			}

			itr = m_removable.Erase(itr);
		}
	}

	void RenderGraph::Add(Memory::RefPtr<RenderPass> renderPass)
	{
		if (!renderPass)
		{
			LOGINFO() << "[Render Graph] Failed to add the render pass, cause the render pass is invalid";
			return;
		}

		Super::Add(renderPass);
		m_addable.Insert(renderPass);

		const Reflection::TypeInfo* typeInfo = renderPass->GetTypeInfo();
		LOGINFO() << "[Render Graph] Add the render pass, the name : " << typeInfo->GetTypeName();
	}

	void RenderGraph::Remove(Memory::RefPtr<RenderPass> renderPass)
	{
		if (!renderPass)
		{
			LOGINFO() << "[Render Graph] Failed to remove the render pass, cause the render pass is invalid";
			return;
		}

		Super::Remove(renderPass->GetID());
		m_removable.Insert(renderPass);

		const Reflection::TypeInfo* typeInfo = renderPass->GetTypeInfo();
		LOGINFO() << "[Render Graph] Remove the render pass, the name : " << typeInfo->GetTypeName();
	}

	void RenderGraph::Execute(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<RenderScene> renderScene, const RenderView& renderView)
	{
		if (!cmdList || !renderScene)
		{
			LOGERROR() << "[Render Graph] Failed to execute the render graph, cause the command list or render scene is invalid";
			return;
		}

		GlobalResource::Update(cmdList, renderView);

		m_drawCommands.Clear();
		const auto& meshBatches = renderScene->GetMeshBatches();
		for (const auto& [id, batch] : meshBatches)
		{
			if (batch && batch->GetResourceState() == eResourceState::eReady)
			{
				m_drawCommands.PushBack(batch->GetDrawCommand());
			}
		}

		m_lightProxies.Clear();
		const auto& lightProxies = renderScene->GetLightProxies();
		for (const auto& [id, lightProxy] : lightProxies)
		{
			if (lightProxy && lightProxy->GetResourceState() == eResourceState::eReady)
			{
				m_lightProxies.PushBack(lightProxy);
			}
		}

		cmdList->Resize(renderView.viewport.posX, renderView.viewport.posY, renderView.viewport.width, renderView.viewport.height);

		for (const auto& renderPass : GetSorted())
		{
			if (!renderPass || renderPass->GetResourceState() != eResourceState::eReady)
			{
				break;
			}

			if (!renderPass->Draw(m_drawCommands, m_lightProxies, cmdList))
			{
				break;
			}
		}
	}
}