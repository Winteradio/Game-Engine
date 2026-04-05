#ifndef __WTR_NODE_H__
#define __WTR_NODE_H__

#include <Container/include/TypeTraits.h>
#include <Memory/include/Pointer/ObjectPtr.h>
#include <Reflection/include/Type/TypeMacro.h>

#include <World/Component.h>

namespace wtr
{
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
	class Node : public BaseNode
	{
		GENERATE(Node);
	public :
		using RequiredType = typename RequiredList<TypeList<>, Components...>::Type;
		using OptionalType = typename OptionalList<TypeList<>, Components...>::Type;

		explicit Node(Memory::ObjectPtr<typename RemoveOptional<Components>::Type>...) {}
		virtual ~Node() = default;
	};

	class LightNode : public Node<SceneComponent, LightComponent>
	{
		GENERATE(LightNode);

	public :
		Memory::ObjectPtr<SceneComponent> transform;
		Memory::ObjectPtr<LightComponent> light;

	public :
		LightNode(Memory::ObjectPtr<SceneComponent> transform, Memory::ObjectPtr<LightComponent> light)
			: Node(transform, light)
			, transform(transform)
			, light(light)
		{}

		virtual ~LightNode() = default;
	};

	class MeshNode : public Node<SceneComponent, MeshComponent, Optional<MaterialComponent>>
	{
		GENERATE(MeshNode);

	public :
		Memory::ObjectPtr<SceneComponent> transform;
		Memory::ObjectPtr<MeshComponent> mesh;
		Memory::ObjectPtr<MaterialComponent> overrideMaterial;

	public :
		MeshNode(Memory::ObjectPtr<SceneComponent> transform, Memory::ObjectPtr<MeshComponent> mesh, Memory::ObjectPtr<MaterialComponent> material = nullptr)
			: Node(transform, mesh, material)
			, transform(transform)
			, mesh(mesh)
			, overrideMaterial(material)
		{}
		virtual ~MeshNode() = default;
	};

	class CameraNode : public Node<SceneComponent, CameraComponent>
	{
		GENERATE(CameraNode);

	public :
		Memory::ObjectPtr<SceneComponent> transform;
		Memory::ObjectPtr<CameraComponent> camera;

	public :
		CameraNode(Memory::ObjectPtr<SceneComponent> transform, Memory::ObjectPtr<CameraComponent> camera)
			: Node(transform, camera)
			, transform(transform)
			, camera(camera)
		{}
		virtual ~CameraNode() = default;
	};
};

#endif // __WTR_NODE_H__