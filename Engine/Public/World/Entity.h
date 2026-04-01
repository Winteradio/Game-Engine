#ifndef __WTR_ENTITY_H__
#define __WTR_ENTITY_H__

#include <Container/include/TypeTraits.h>
#include <ECS/include/Object/Entity.h>
#include <Memory/include/Core.h>

#include <World/World.h>

namespace wtr
{
	class Entity : public ECS::Entity
	{
	public :
		Entity();
		Entity(World* owner, const std::string& name = "default");
		virtual ~Entity();

	public :
		void Clear();

	public :
		template<typename T, typename... Args>
		bool AddComponent(Args&&... args)
		{
			if (!m_owner)
			{
				return false;
			}

			Memory::ObjectPtr<T> component = m_owner->CreateComponent<T>(GetID(), std::forward<Args>(args)...);
			if (!component)
			{
				return false;
			}

			ECS::Entity::AddComponent<T>();

			return true;
		}

		template<typename T, typename... Args>
		bool AddNode(Args&&... args)
		{
			if (!HasAllComponents(typename T::Required{}))
			{
				return false;
			}

			Memory::ObjectPtr<T> node = CreateNode<T>(typename T::Required{});
			if (!node)
			{
				return false;
			}

			ECS::Entity::AddNode<T>();

			return true;
		}

		template<typename T>
		Memory::ObjectPtr<T> GetComponent()
		{
			if (!m_owner)
			{
				return Memory::ObjectPtr<T>();
			}

			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
			Memory::ObjectPtr<BaseComponent> component = m_owner->GetComponent(GetID(), typeInfo);
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
			if (!m_owner)
			{
				return Memory::ObjectPtr<ResultType>();
			}

			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
			Memory::ObjectPtr<const BaseComponent> component = m_owner->GetComponent(GetID(), typeInfo);
			if (component)
			{
				return Memory::Cast<typename ResultType>(component);
			}

			return Memory::ObjectPtr<typename ResultType>();
		}

		template<typename T>
		Memory::ObjectPtr<T> GetNode()
		{
			if (!m_owner)
			{
				return Memory::ObjectPtr<T>();
			}

			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
			Memory::ObjectPtr<BaseNode> node = m_owner->GetNode(GetID(), typeInfo);
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
			if (!m_owner)
			{
				return Memory::ObjectPtr<ResultType>();
			}

			const Reflection::TypeInfo* typeInfo = Reflection::TypeInfo::Get<T>();
			Memory::ObjectPtr<BaseNode> node = m_owner->GetNode(GetID(), typeInfo);
			if (node)
			{
				return Memory::Cast<ResultType>(node);
			}

			return Memory::ObjectPtr<ResultType>();
		}

		template<typename... Components>
		bool HasAllComponents(TypeList<Components...>) const
		{
			return (... && HasComponent<Components>());
		}

	private :
		template<typename T, typename... Components>
		Memory::ObjectPtr<T> CreateNode(TypeList<Components...>)
		{
			if (!m_owner)
			{
				return {};
			}

			Memory::ObjectPtr<T> node = m_owner->CreateNode<T>(GetID(), GetComponent<Components>()...);
			return node;
		}

	private :
		World* m_owner;
	};
};

#endif // __WTR_ENTITY_H__