#include <World/Entity.h>

#include <World/World.h>

#include <World/Component.h>

namespace wtr
{
	Entity::Entity()
		: ECS::Entity()
		, m_owner()
	{}

	Entity::Entity(World* owner, const std::string& name)
		: ECS::Entity(name)
		, m_owner(owner)
	{}

	Entity::~Entity()
	{}

	void Entity::Clear()
	{
		if (nullptr == m_owner)
		{
			return;
		}

		for (const auto* nodeType : GetNodeType())
		{
			m_owner->RemoveNode(GetID(), nodeType);
		}

		for (const auto* componentType : GetComponentType())
		{
			m_owner->RemoveComponent(GetID(), componentType);
		}

		m_owner->RemoveEntity(GetID());
		m_owner = nullptr;
	}
}