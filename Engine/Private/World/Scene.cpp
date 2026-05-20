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
			node->ClearDirty();

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
			node->ClearDirty();

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

	Memory::ObjectPtr<ProxyNode> Scene::GetProxyNode(const ECS::UUID& id) const
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

		m_proxies[node->GetID()] = node;
		
		m_addedProxies.Insert(node->GetID());
		m_removedProxies.Erase(node->GetID());
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

		m_removedProxies.Insert(entityId);

		m_addedProxies.Erase(entityId);
		m_updatedProxies.Erase(entityId);
		m_proxies.Erase(entityId);
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

	void Scene::Update(const ECS::UUID& entityId)
	{
		if (!m_refCommander)
		{
			return;
		}

		m_updatedProxies.Insert(entityId);
	}
}