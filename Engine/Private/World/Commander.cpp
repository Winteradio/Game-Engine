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
		// TODO
	}

	void Commander::AddPrimitive(Memory::ObjectPtr<MeshNode> meshNode)
	{
		if (!meshNode || !m_refCmdList)
		{
			return;
		}

		Memory::RefPtr<PrimitiveProxy> primitive = Memory::MakeRef<PrimitiveProxy>();
		if (!primitive)
		{
			return;
		}

		m_refCmdList->Enqueue([primitive](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
		{
			LOGINFO() << "[Render] Add primitive";
		});
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
				LOGINFO() << "[Render] Remove Primitive";
		});
	}

	void Commander::AddLight(Memory::ObjectPtr<LightNode> lightNode)
	{
		if (!lightNode || !m_refCmdList)
		{
			return;
		}

		//m_refCmdList->Enqueue();
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
			LOGINFO() << "[Render] Remove Primitive";
		});
	}

	void Commander::Update(Memory::ObjectPtr<SceneComponent> sceneComponent)
	{
		if (!sceneComponent || !m_refCmdList)
		{
			return;
		}

		m_refCmdList->Enqueue([](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
		{
			LOGINFO() << "[Render] Update the scene transform";
		});
	}

	void Commander::Remove(const ECS::UUID& entityId)
	{
		if (!m_refCmdList)
		{
			return;
		}
		//m_refCmdList->Enqueue();
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
		});
	}
}