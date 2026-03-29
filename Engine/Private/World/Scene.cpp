#include <World/Scene.h>

#include <World/Node.h>
#include <World/Commander.h>

#include <Reflection/include/Utils.h>

namespace wtr
{
	Scene::Scene()
		: m_refCommander(nullptr)
		, m_nodes()
	{}

	Scene::~Scene()
	{}

	void Scene::SetCommander(Memory::RefPtr<Commander> refCommander)
	{
		m_refCommander = refCommander;
	}

	void Scene::Attach(Memory::ObjectPtr<SceneNode> node)
	{
		if (!m_refCommander || !node)
		{
			return;
		}

		node->SetScene(this);

		m_nodes[node->GetID()] = node;
		m_refCommander->AddNode(node);
	}

	void Scene::Detach(const ECS::UUID& nodeId)
	{
		if (!m_refCommander)
		{
			return;
		}

		auto itr = m_nodes.Find(nodeId);
		if (itr == m_nodes.End())
		{
			return;
		}

		auto node = itr->second;
		node->SetScene(nullptr);

		m_nodes.Erase(itr);
		m_refCommander->RemoveNode(node);
	}

	void Scene::DetachAll()
	{
		if (!m_refCommander)
		{
			return;
		}

		for (auto& [id, sceneNode] : m_nodes)
		{
			if (!sceneNode)
			{
				continue;
			}

			m_refCommander->RemoveNode(sceneNode);
		}

		m_nodes.Clear();
	}

	void Scene::Update(const ECS::UUID& nodeId)
	{
		if (!m_refCommander)
		{
			return;
		}

		auto itr = m_nodes.Find(nodeId);
		if (itr == m_nodes.End())
		{
			return;
		}

		auto node = itr->second;

		m_refCommander->UpdateNode(node);
	}
}