#include <World/Commander.h>

#include <Renderer/RenderView.h>
#include <Renderer/RenderCommandList.h>
#include <Renderer/RenderScene.h>
#include <Renderer/Renderer.h>
#include <Renderer/Proxy/PrimitiveProxy.h>
#include <Renderer/Proxy/LightProxy.h>

#include <World/Node.h>
#include <Log/include/Log.h>

namespace wtr
{
	Commander::Commander()
		: m_refCmdList()
	{}

	Commander::~Commander()
	{}

	void Commander::SetCommand(Memory::RefPtr<RenderCommandList> refCmdList)
	{
		m_refCmdList = refCmdList;
	}

	void Commander::SetView(const RenderView& renderView)
	{
		if (!m_refCmdList)
		{
			return;
		}

		m_refCmdList->Enqueue([renderView](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (nullptr != renderer)
				{
					renderer->SetView(renderView);
				}
			}
		);
	}

	void Commander::AddPrimitive(Memory::ObjectPtr<MeshNode> meshNode)
	{
		if (!meshNode || !m_refCmdList)
		{
			return;
		}

		auto& meshComponent = meshNode->mesh;
		auto& materialComponent = meshNode->overrideMaterial;
		auto& transformComponent = meshNode->transform;

		if (!meshComponent || !transformComponent)
		{
			return;
		}

		Memory::RefPtr<PrimitiveProxy> primitive = Memory::MakeRef<PrimitiveProxy>();
		if (!primitive)
		{
			return;
		}

		primitive->SetID(meshNode->GetID());
		primitive->SetMesh(meshComponent->meshAsset);
		primitive->UpdatePosition(transformComponent->GetPosition());
		primitive->UpdateRotation(transformComponent->GetRotation());
		primitive->UpdateScale(transformComponent->GetScale());

		if (materialComponent)
		{
			primitive->SetOverrideMaterial(materialComponent->materialAsset);
		}

		m_refCmdList->Enqueue([primitive](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (nullptr == renderer)
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				if (!renderScene)
				{
					return;
				}

				renderScene->AddPrimitive(primitive);
			}
		);
	}

	void Commander::RemovePrimitive(Memory::ObjectPtr<MeshNode> meshNode)
	{
		if (!meshNode || !m_refCmdList)
		{
			return;
		}

		const ECS::UUID& id = meshNode->GetID();
		m_refCmdList->Enqueue([id](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (nullptr == renderer)
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				if (!renderScene)
				{
					return;
				}

				renderScene->RemovePrimitive(id, cmdList);
			}
		);
	}

	void Commander::AddLight(Memory::ObjectPtr<LightNode> lightNode)
	{
		if (!lightNode || !m_refCmdList)
		{
			return;
		}

		auto& lightComponent = lightNode->light;
		auto& transformComponent = lightNode->transform;
		if (!lightComponent || !transformComponent)
		{
			return;
		}

		Memory::RefPtr<LightProxy> light = Memory::MakeRef<LightProxy>();
		if (!light)
		{
			return;
		}

		light->SetID(lightNode->GetID());
		light->UpdatePosition(transformComponent->GetPosition());
		light->UpdateRotation(transformComponent->GetRotation());
		light->UpdateScale(transformComponent->GetScale());

		// TODO : Set light properties
		m_refCmdList->Enqueue([light](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (nullptr == renderer)
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				if (!renderScene)
				{
					return;
				}

				renderScene->AddLight(light);
			}
		);
	}

	void Commander::RemoveLight(Memory::ObjectPtr<LightNode> lightNode)
	{
		if (!lightNode || !m_refCmdList)
		{
			return;
		}

		const ECS::UUID& id = lightNode->GetID();
		m_refCmdList->Enqueue([id](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (nullptr == renderer)
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				if (!renderScene)
				{
					return;
				}

				renderScene->RemoveLight(id);
			}
		);
	}

	void Commander::Update(Memory::ObjectPtr<SceneComponent> sceneComponent)
	{
		if (!sceneComponent || !m_refCmdList)
		{
			return;
		}

		const UpdateProxyInfo updateInfo = {
			sceneComponent->GetID(),
			sceneComponent->GetPosition(),
			sceneComponent->GetRotation(),
			sceneComponent->GetScale()
		};

		m_refCmdList->Enqueue([updateInfo](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (nullptr == renderer)
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				if (!renderScene)
				{
					return;
				}

				renderScene->UpdateProxy(updateInfo, cmdList);
			}
		);
	}

	void Commander::Remove(const ECS::UUID& entityId)
	{
		if (!m_refCmdList)
		{
			return;
		}
		

		m_refCmdList->Enqueue([entityId](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (nullptr == renderer)
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				if (!renderScene)
				{
					return;
				}

				renderScene->RemoveLight(entityId);
				renderScene->RemovePrimitive(entityId, cmdList);
			}
		);
	}

	void Commander::RemoveAll()
	{
		if (!m_refCmdList)
		{
			return;
		}

		m_refCmdList->Enqueue([](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				LOGINFO() << "[Render] Remove all primitives and lights";

				if (nullptr == renderer)
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				if (!renderScene)
				{
					return;
				}

				renderScene->RemoveAll(cmdList);
			}
		);
	}
}