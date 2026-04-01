#ifndef __WTR_NODE_H__
#define __WTR_NODE_H__

#include <Container/include/TypeTraits.h>
#include <Memory/include/Pointer/ObjectPtr.h>
#include <Reflection/include/Type/TypeMacro.h>

#include <World/Component.h>

namespace wtr
{
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
		using Required = TypeList<Components...>;

		explicit Node(Memory::ObjectPtr<Components>...) {}
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

	class MeshNode : public Node<SceneComponent, MeshComponent>
	{
		GENERATE(MeshNode);

	public :
		Memory::ObjectPtr<SceneComponent> transform;
		Memory::ObjectPtr<MeshComponent> mesh;

	public :
		MeshNode(Memory::ObjectPtr<SceneComponent> transform, Memory::ObjectPtr<MeshComponent> mesh)
			: Node(transform, mesh)
			, transform(transform)
			, mesh(mesh)
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