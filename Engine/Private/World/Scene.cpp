#include <World/Scene.h>

#include <World/Node.h>
#include <World/Commander.h>

#include <Reflection/include/Utils.h>
#include <Memory/include/Core.h>

namespace wtr
{
	Scene::Scene()
		: m_refCommander(nullptr)
		, m_sceneDatas()
	{}

	Scene::~Scene()
	{}

	void Scene::SetCommander(Memory::RefPtr<Commander> refCommander)
	{
		m_refCommander = refCommander;
	}

	void Scene::Attach(Memory::ObjectPtr<BaseNode> node)
	{
		if (!m_refCommander || !node)
		{
			return;
		}

		if (auto meshNode = Memory::Cast<MeshNode>(node))
		{
			AttachNode(meshNode, meshNode->transform);

			m_refCommander->AddPrimitive(meshNode);
		}
		else if (auto lightNode = Memory::Cast<LightNode>(node))
		{
			AttachNode(lightNode, lightNode->transform);

			m_refCommander->AddLight(lightNode);
		}
		else
		{
			// nothing
		}
	}

	void Scene::Detach(Memory::ObjectPtr<BaseNode> node)
	{
		if (!m_refCommander || !node)
		{
			return;
		}

		if (auto meshNode = Memory::Cast<MeshNode>(node))
		{
			DetachNode(meshNode);

			m_refCommander->RemovePrimitive(meshNode);
		}
		else if (auto lightNode = Memory::Cast<LightNode>(node))
		{
			DetachNode(lightNode);

			m_refCommander->RemoveLight(lightNode);
		}
		else
		{
			// nothing
		}
	}

	void Scene::Detach(const ECS::UUID& entityId)
	{
		if (!m_refCommander)
		{
			return;
		}

		m_refCommander->Remove(entityId);
		m_sceneDatas.Erase(entityId);
	}

	void Scene::DetachAll()
	{
		if (!m_refCommander)
		{
			return;
		}

		m_refCommander->RemoveAll();
	}

	void Scene::Update(const ECS::UUID& entityId)
	{
		if (!m_refCommander)
		{
			return;
		}

		auto itr = m_sceneDatas.Find(entityId);
		if (itr == m_sceneDatas.End())
		{
			return;
		}

		auto& scenePair = itr->second;

		m_refCommander->Update(scenePair.transform);
	}

	void Scene::AttachNode(Memory::ObjectPtr<BaseNode> node, Memory::ObjectPtr<SceneComponent> transform)
	{
		if (!node || !transform)
		{
			return;
		}

		const Reflection::TypeInfo* nodeType = node->GetTypeInfo();

		auto itr = m_sceneDatas.Find(node->GetID());
		if (itr == m_sceneDatas.End())
		{
			ScenePair scenePair;
			scenePair.transform = transform;
			scenePair.transform->OnAttached(this);
			scenePair.nodeTypes.Insert(nodeType->GetTypeHash());

			m_sceneDatas[node->GetID()] = scenePair;
		}
		else
		{
			ScenePair& scenePair = itr->second;

			scenePair.nodeTypes.Insert(nodeType->GetTypeHash());
		}
	}

	void Scene::DetachNode(Memory::ObjectPtr<BaseNode> node)
	{
		if (!node)
		{
			return;
		}

		const Reflection::TypeInfo* nodeType = node->GetTypeInfo();
		
		auto itr = m_sceneDatas.Find(node->GetID());
		if (itr == m_sceneDatas.End())
		{
			return;
		}
		
		ScenePair& scenePair = itr->second;
		scenePair.transform->OnDetached();
		scenePair.nodeTypes.Erase(nodeType->GetTypeHash());
		
		if (scenePair.nodeTypes.Empty())
		{
			m_sceneDatas.Erase(itr);
		}
	}
}