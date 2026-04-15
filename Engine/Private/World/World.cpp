#include <World/World.h>

#include <World/Entity.h>
#include <World/Commander.h>
#include <Log/include/Log.h>

namespace wtr
{
	World::World()
		: ECS::Object()
		, scene()
		, m_entityStorage()
		, m_nodeContainer()
		, m_componentContainer()
		, m_systemRegistry()
	{}

	World::~World()
	{}

	bool World::Init(Memory::RefPtr<Commander> refCommander)
	{
		LOGINFO() << "[World] Initialize the world";

		if (!refCommander)
		{
			LOGERROR() << "[World] Failed to set the commander, the commander is invalid";
			return false;
		}

		scene.SetCommander(refCommander);

		return true;
	}

	void World::Clear()
	{
		for (const auto& [id, entity] : m_entityStorage)
		{
			if (entity)
			{
				entity->Clear();
			}
		}

		scene.DetachAll();
		m_entityStorage.Clear();
		m_nodeContainer.Clear();
		m_componentContainer.Clear();
		m_systemRegistry.Clear();
	}

	void World::Update(const ECS::TimeStep& timeStep)
	{
		const auto& sortedSystems = m_systemRegistry.GetSorted();

		for (auto& system : sortedSystems)
		{
			if (!system)
			{
				continue;
			}

			const auto nodeType = system->GetNodeType();
			auto nodeContainer = m_nodeContainer.GetContainer(nodeType);

			system->Update(timeStep, nodeContainer);
		}
	}

	Memory::ObjectPtr<Entity> World::CreateEntity()
	{
		return m_entityStorage.Emplace(this);
	}

	void World::RegisterEntity(Memory::ObjectPtr<Entity> entity)
	{
		if (!entity)
		{
			// Not implemented for the level and layer system, so just
			return;
		}
	}

	void World::RemoveEntity(const ECS::UUID& uuid)
	{
		scene.Detach(uuid);

		m_entityStorage.Erase(uuid);
	}

	void World::RemoveComponent(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo)
	{
		m_componentContainer.Remove(uuid, typeinfo);
	}

	void World::RemoveNode(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo)
	{
		m_nodeContainer.Remove(uuid, typeinfo);
	}

	void World::RemoveSystem(const ECS::UUID& uuid)
	{
		m_systemRegistry.Remove(uuid);
	}

	Memory::ObjectPtr<Entity> World::GetEntity(const ECS::UUID& uuid)
	{
		return m_entityStorage.Get(uuid);
	}

	Memory::ObjectPtr<BaseComponent> World::GetComponent(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo)
	{
		auto container = m_componentContainer.GetContainer(typeinfo);
		if (container)
		{
			return container->GetBase(uuid);
		}
		else
		{
			return Memory::ObjectPtr<BaseComponent>();
		}
	}

	Memory::ObjectPtr<BaseNode> World::GetNode(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo)
	{
		auto container = m_nodeContainer.GetContainer(typeinfo);
		if (container)
		{
			return container->GetBase(uuid);
		}
		else
		{
			return Memory::ObjectPtr<BaseNode>();
		}
	}

	Memory::RefPtr<BaseSystem> World::GetSystem(const ECS::UUID& uuid)
	{
		return m_systemRegistry.Get(uuid);
	}
}