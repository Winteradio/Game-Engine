#include <World/Scene.h>

#include <World/RenderNode.h>
#include <World/Commander.h>

#include <Renderer/Renderer.h>
#include <Renderer/RenderScene.h>
#include <Renderer/RenderTask.h>
#include <Renderer/Proxy/SceneProxy.h>

#include <Reflection/include/Utils.h>
#include <Memory/include/Core.h>

namespace wtr
{
	bool Scene::NodeID::operator==(const NodeID& other) const
	{
		if (this != &other)
		{
			return id == other.id && typeInfo == other.typeInfo;
		}

		return true;
	}

	bool Scene::NodeID::operator!=(const NodeID& other) const
	{
		return !(*this == other);
	}

	Scene::Scene()
		: m_refCommander(nullptr)
		, m_proxies()
		, m_addedProxies()
		, m_removedProxies()
		, m_updatedProxies()
	{}

	Scene::~Scene()
	{}

	void Scene::SetCommander(Memory::RefPtr<Commander> refCommander)
	{
		m_refCommander = refCommander;
	}

	void Scene::Flush()
	{
		FlushAdded();
		FlushRemoved();
		FlushUpdated();
	}

	void Scene::FlushAdded()
	{
		if (!m_refCommander)
		{
			return;
		}

		for (const auto& id : m_addedProxies)
		{
			auto node = GetProxyNode(id);
			if (!node)
			{
				continue;
			}

			auto createTask = node->CreateProxy();
			node->OnUpdated();

			if (!createTask.func)
			{
				continue;
			}

			m_refCommander->Enqueue(std::move(createTask));
		}

		m_addedProxies.Clear();
	}

	void Scene::FlushRemoved()
	{
		if (!m_refCommander)
		{
			return;
		}

		for (const auto& id : m_removedProxies)
		{
			auto node = GetProxyNode(id);
			if (!node)
			{
				continue;
			}

			auto removeTask = node->RemoveProxy();
			if (!removeTask.func)
			{
				continue;
			}

			m_refCommander->Enqueue(std::move(removeTask));
			m_proxies.Erase(id);
		}

		m_removedProxies.Clear();
	}

	void Scene::FlushUpdated()
	{
		if (!m_refCommander)
		{
			return;
		}

		for (const auto& id : m_updatedProxies)
		{
			auto node = GetProxyNode(id);
			if (!node)
			{
				continue;
			}

			auto updateTaskList = node->UpdateProxy();
			node->OnUpdated();

			for (auto& updateTask : updateTaskList)
			{
				if (!updateTask.func)
				{
					continue;
				}

				m_refCommander->Enqueue(std::move(updateTask));
			}
		}

		m_updatedProxies.Clear();
	}

	Memory::ObjectPtr<ProxyNode> Scene::GetProxyNode(const NodeID& id) const
	{
		auto itr = m_proxies.Find(id);
		if (itr != m_proxies.End())
		{
			return itr->second;
		}
		else
		{
			return nullptr;
		}
	}

	void Scene::Attach(Memory::ObjectPtr<ProxyNode> node)
	{
		if (!m_refCommander || !node)
		{
			return;
		}

		node->OnAttached(this);

		NodeID nodeID = { node->GetID(), node->GetTypeInfo() };
		m_proxies[nodeID] = node;
		m_proxyIDs[node->GetID()].Insert(nodeID);
		
		m_addedProxies.Insert(nodeID);
		m_removedProxies.Erase(nodeID);
	}

	void Scene::Detach(Memory::ObjectPtr<ProxyNode> node)
	{
		if (!m_refCommander || !node)
		{
			return;
		}

		node->OnDetached();
		Detach(node->GetID());
	}

	void Scene::Detach(const ECS::UUID& entityId)
	{
		if (!m_refCommander)
		{
			return;
		}

		auto itr = m_proxyIDs.Find(entityId);
		if (itr == m_proxyIDs.End())
		{
			return;
		}

		for (const auto& proxyID : itr->second)
		{
			m_removedProxies.Insert(proxyID);

			m_addedProxies.Erase(proxyID);
			m_updatedProxies.Erase(proxyID);
			m_proxies.Erase(proxyID);
		}
	}

	void Scene::DetachAll()
	{
		if (!m_refCommander)
		{
			return;
		}

		for (const auto& [id, proxy] : m_proxies)
		{
			if (proxy)
			{
				proxy->OnDetached();
			}
		}

		m_refCommander->RemoveAll();
	}

	void Scene::Update(BaseComponent* component)
	{
		if (!m_refCommander || !component)
		{
			return;
		}

		const ECS::UUID& entityID = component->GetID();
		auto itr = m_proxyIDs.Find(entityID);
		if (itr == m_proxyIDs.End())
		{
			return;
		}

		const Reflection::TypeInfo* componentType = component->GetTypeInfo();
		for (const auto& proxyID : itr->second)
		{
			auto& proxyNode = m_proxies[proxyID];
			if (!proxyNode)
			{
				continue;
			}

			const auto& componentSet = proxyNode->GetComponentSet();
			if (componentSet.Find(componentType) != componentSet.End())
			{
				component->OnPending();
				m_updatedProxies.Insert(proxyID);
			}
		}
	}
}