#ifndef __WTR_ENTITY_H__
#define __WTR_ENTITY_H__

#include <ECS/include/Object/Entity.h>
#include <Memory/include/Pointer/ObjectPtr.h>

#include <World/World.h>

namespace wtr
{
	class Entity : public ECS::Entity
	{
	public :
		using IDData = wtr::HashMap<size_t, ECS::UUID>;

		Entity();
		Entity(World* owner, const std::string& name = "default");
		virtual ~Entity();

	public :
		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			if (m_owner)
			{
				Memory::ObjectPtr<T> component = m_owner->CreateComponent<T>(GetID(), std::forward<Args>(args)...);
				if (component)
				{
					ECS::Entity::AddComponent<T>();
				}
			}
		}

		template<typename T, typename... Args>
		void AddNode(Args&&... args)
		{
			if (m_owner)
			{
				Memory::ObjectPtr<T> node = m_owner->CreateNode<T>(GetID(), std::forward<Args>(args)...);
				if (node)
				{
					ECS::Entity::AddComponent<T>();
				}
			}
		}

		template<typename T>
		Memory::ObjectPtr<T> GetComponent()
		{
			if (nullptr == m_owner)
			{
				return Memory::ObjectPtr<T>();
			}

			Memory::ObjectPtr<ECS::Component> component = m_owner->GetComponent(GetID(), ECS::GetTypeInfo<T>());
			if (component)
			{
				return Memory::Cast<T>(component);
			}

			return Memory::ObjectPtr<T>();
		}

		template<typename T>
		Memory::ObjectPtr<T> GetNode()
		{
			if (nullptr == m_owner)
			{
				return Memory::ObjectPtr<T>();
			}

			Memory::ObjectPtr<ECS::Node> node = m_owner->GetNode(GetID(), ECS::GetTypeInfo<T>());
			if (node)
			{
				return Memory::Cast<T>(node);
			}

			return Memory::ObjectPtr<T>();
		}

	private :
		World* m_owner;
	};
};

#endif // __WTR_ENTITY_H__