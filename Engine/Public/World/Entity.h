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

			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
			Memory::ObjectPtr<ECS::Component> component = m_owner->GetComponent(GetID(), typeInfo);
			if (component)
			{
				return Memory::Cast<T>(component);
			}

			return Memory::ObjectPtr<T>();
		}

		template<typename T, 
			typename PureType = Reflection::Utils::RemoveConst_t<T>,
			typename ResultType = const PureType>
		Memory::ObjectPtr<ResultType> GetComponent() const
		{
			if (nullptr == m_owner)
			{
				return Memory::ObjectPtr<ResultType>();
			}

			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
			Memory::ObjectPtr<const ECS::Component> component = m_owner->GetComponent(GetID(), typeInfo);
			if (component)
			{
				return Memory::Cast<ResultType>(component);
			}

			return Memory::ObjectPtr<ResultType>();
		}

		template<typename T>
		Memory::ObjectPtr<T> GetNode()
		{
			if (nullptr == m_owner)
			{
				return Memory::ObjectPtr<T>();
			}

			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
			Memory::ObjectPtr<ECS::Node> node = m_owner->GetNode(GetID(), typeInfo);
			if (node)
			{
				return Memory::Cast<T>(node);
			}

			return Memory::ObjectPtr<T>();
		}

		template<typename T,
			typename PureType = Reflection::Utils::RemoveConst_t<T>, 
			typename ResultType = const PureType>
		Memory::ObjectPtr<ResultType> GetNode() const
		{
			if (nullptr == m_owner)
			{
				return Memory::ObjectPtr<ResultType>();
			}

			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
			Memory::ObjectPtr<ECS::Node> node = m_owner->GetNode(GetID(), typeInfo);
			if (node)
			{
				return Memory::Cast<ResultType>(node);
			}

			return Memory::ObjectPtr<ResultType>();
		}

	private :
		World* m_owner;
	};
};

#endif // __WTR_ENTITY_H__