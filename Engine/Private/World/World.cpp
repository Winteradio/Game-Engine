#include <World/World.h>

#include <World/Entity.h>
#include <World/Commander.h>

namespace wtr
{
	World::World()
		: ECS::Object()
		, m_refCommander()
		, m_entityContainer()
		, m_nodeContainer()
		, m_componentContainer()
		, m_systemRegistry()
	{}

	World::~World()
	{}

	void World::SetCommander(Memory::RefPtr<SceneCommander> refCommander)
	{
		m_refCommander = refCommander;
	}

	void World::Clear()
	{
		m_entityContainer.Clear();
		m_nodeContainer.Clear();
		m_componentContainer.Clear();
		m_systemRegistry.Clear();
	}

	void World::Update(const ECS::TimeStep& timeStep)
	{
		const ECS::SystemRegistry::GraphType graph = m_systemRegistry.BuildGraph();
	}

	Memory::ObjectPtr<Entity> World::CreateEntity()
	{
		return m_entityContainer.Emplace(this);
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
		m_entityContainer.Erase(uuid);
	}

	void World::RemoveSystem(const ECS::UUID& uuid)
	{
		m_systemRegistry.Remove(uuid);
	}

	void World::RemoveComponent(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo)
	{
		m_componentContainer.Remove(uuid, typeinfo);
	}

	void World::RemoveNode(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo)
	{
		m_nodeContainer.Remove(uuid, typeinfo);
	}

	Memory::ObjectPtr<Entity> World::GetEntity(const ECS::UUID& uuid)
	{
		return m_entityContainer.Get(uuid);
	}

	Memory::ObjectPtr<ECS::System> World::GetSystem(const ECS::UUID& uuid)
	{
		return m_systemRegistry.Get(uuid);
	}

	Memory::ObjectPtr<ECS::Component> World::GetComponent(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo)
	{
		auto container = m_componentContainer.GetContainer(typeinfo);
		if (container)
		{
			return container->GetBase(uuid);
		}
		else
		{
			return Memory::ObjectPtr<ECS::Component>();
		}
	}

	Memory::ObjectPtr<ECS::Node> World::GetNode(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo)
	{
		auto container = m_nodeContainer.GetContainer(typeinfo);
		if (container)
		{
			return container->GetBase(uuid);
		}
		else
		{
			return Memory::ObjectPtr<ECS::Node>();
		}
	}
}