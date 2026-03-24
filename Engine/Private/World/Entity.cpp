#include <World/Entity.h>

namespace wtr
{
	Entity::Entity()
		: ECS::Entity()
		, m_owner(nullptr)
	{}

	Entity::Entity(World* owner, const std::string& name)
		: ECS::Entity(name)
		, m_owner(owner)
	{}

	Entity::~Entity()
	{
		if (!m_owner)
		{
			return;
		}

		for (const auto* typeInfo : GetComponentType())
		{
			m_owner->RemoveComponent(GetID(), typeInfo);
		}

		for (const auto* typeInfo : GetNodeType())
		{
			m_owner->RemoveNode(GetID(), typeInfo);
		}

		m_owner->RemoveEntity(GetID());

		m_owner = nullptr;
	}
}