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

	void Scene::Attach(Memory::ObjectPtr<BaseNode> node)
	{
		if (!m_refCommander || !node)
		{
			return;
		}

		// TODO
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

		// TODO
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