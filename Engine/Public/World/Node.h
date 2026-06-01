#ifndef __WTR_NODE_H__
#define __WTR_NODE_H__

#include <Container/include/TypeTraits.h>
#include <Memory/include/Pointer/ObjectPtr.h>
#include <Reflection/include/Type/TypeMacro.h>

#include <World/Component.h>

namespace wtr
{
	namespace Generation
	{
		template<typename...>
		class Node {};

		template<>
		class Node<> {};

		template<typename Head, typename... Tails>
		class Node<Head, Tails...> : Node<Tails...>
		{
		public:
			Node() : m_lastSeen(0) {};
			virtual ~Node() = default;

		public:
			template<typename T>
			bool IsDirty(Memory::ObjectPtr<T> component) const
			{
				if (!component)
				{
					return false;
				}

				if constexpr (Reflection::Utils::IsSame<Head, T>::value)
				{
					return component->GetGeneration() != m_lastSeen;
				}
				else
				{
					return Node<Tails...>::IsDirty(component);
				}
			};

			template<typename T>
			void OnSynced(Memory::ObjectPtr<T> component)
			{
				if (!component)
				{
					return;
				}

				if constexpr (Reflection::Utils::IsSame<Head, T>::value)
				{
					m_lastSeen = component->GetGeneration();
				}
				else
				{
					Node<Tails...>::OnSynced(component);
				}
			}

		private:
			uint64_t m_lastSeen;
		};
	};

	template<typename T>
	struct Optional
	{};

	template<typename T>
	struct RemoveOptional
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveOptional<Optional<T>>
	{
		using Type = T;
	};

	template<typename T>
	struct IsOptional : std::false_type
	{};

	template<typename T>
	struct IsOptional<Optional<T>> : std::true_type
	{};

	template<typename ComponentList, typename ...>
	struct RequiredList
	{
		using Type = ComponentList;
	};

	template<typename... Components, typename Head, typename... Tail>
	struct RequiredList<TypeList<Components...>, Head, Tail...>
	{
		using Type = std::conditional_t<
			!IsOptional<Head>::value,
			typename RequiredList<TypeList<Components..., Head>, Tail...>::Type,
			typename RequiredList<TypeList<Components...>, Tail...>::Type
		>;
	};

	template<typename ComponentList, typename ...>
	struct OptionalList
	{
		using Type = ComponentList;
	};

	template<typename... Components, typename Head, typename... Tail>
	struct OptionalList<TypeList<Components...>, Head, Tail...>
	{
		using Type = std::conditional_t<
			IsOptional<Head>::value,
			typename OptionalList<TypeList<Components..., Head>, Tail...>::Type,
			typename OptionalList<TypeList<Components...>, Tail...>::Type
		>;
	};

	class BaseNode : public ECS::Node
	{
		GENERATE(BaseNode);
	public:
		using ECS::Node::Node;

		virtual ~BaseNode() = default;
	};

	template<typename... Components>
	class Node : public BaseNode, public Generation::Node<typename RemoveOptional<Components>::Type...>
	{
		GENERATE(Node);
	public:
		using RequiredType = typename RequiredList<TypeList<>, Components...>::Type;
		using OptionalType = typename OptionalList<TypeList<>, Components...>::Type;

		explicit Node(Memory::ObjectPtr<typename RemoveOptional<Components>::Type>...) {}
		virtual ~Node() = default;
	};

	class CameraNode : public Node<TransformComponent, CameraComponent>
	{
		GENERATE(CameraNode);

	public:
		Memory::ObjectPtr<TransformComponent> transform;
		Memory::ObjectPtr<CameraComponent> camera;

	public:
		CameraNode(Memory::ObjectPtr<TransformComponent> transform, Memory::ObjectPtr<CameraComponent> camera)
			: Node(transform, camera)
			, transform(transform)
			, camera(camera)
		{
		}

		virtual ~CameraNode() = default;

	public:
		const fmat4 GetViewMatrix() const;
		const fmat4 GetProjectionMatrix() const;
	};
};

#endif // __WTR_NODE_H__