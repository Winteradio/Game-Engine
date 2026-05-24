#ifndef __WTR_RENDERNODE_H__
#define __WTR_RENDERNODE_H__

#include <World/Node.h>
#include <Container/include/HashSet.h>

namespace wtr
{
	class Scene;
	class SceneProxy;
	struct RenderTask;

	using RenderTaskList = wtr::DynamicArray<RenderTask>;
};

namespace wtr
{
	class ProxyNode : public BaseNode
	{
		GENERATE(ProxyNode);
	public :
		using BaseNode::BaseNode;
		using COMPONENT_SET = wtr::HashSet<const Reflection::TypeInfo*>;

		virtual ~ProxyNode() = default;

	public :
		virtual void OnAttached(Scene* scene) = 0;
		virtual void OnDetached() = 0;

		virtual RenderTask CreateProxy() const = 0;
		virtual RenderTaskList UpdateProxy() const = 0;
		virtual RenderTask RemoveProxy() const;
		virtual void ClearDirty() = 0;

		virtual const COMPONENT_SET& GetComponentSet() const = 0;
	};

	template<typename... Components>
	class RenderNode : public ProxyNode
	{
		GENERATE(RenderNode);
	public :
		using RequiredType = typename RequiredList<TypeList<>, Components...>::Type;
		using OptionalType = typename OptionalList<TypeList<>, Components...>::Type;

		explicit RenderNode(Memory::ObjectPtr<typename RemoveOptional<Components>::Type>...) {}
		virtual ~RenderNode() = default;

	public :
		const COMPONENT_SET& GetComponentSet() const override
		{
			return COMPONENTS;
		}

	private :
		static inline COMPONENT_SET COMPONENTS = { Reflection::TypeInfo::Get<typename RemoveOptional<Components>::Type>()... };
	};

	class StaticMeshNode : public RenderNode<TransformComponent, StaticMeshComponent, Optional<MaterialComponent>>
	{
		GENERATE(StaticMeshNode);
	public :
		Memory::ObjectPtr<TransformComponent> transform;
		Memory::ObjectPtr<StaticMeshComponent> mesh;
		Memory::ObjectPtr<MaterialComponent> overrideMaterial;

	public :
		StaticMeshNode(Memory::ObjectPtr<TransformComponent> transform, 
			Memory::ObjectPtr<StaticMeshComponent> mesh, 
			Memory::ObjectPtr<MaterialComponent> material = nullptr)
			: RenderNode(transform, mesh, material)
			, transform(transform)
			, mesh(mesh)
			, overrideMaterial(material)
		{
		}

		virtual ~StaticMeshNode() = default;

	public :
		void OnAttached(Scene* scene) override;
		void OnDetached() override;

		RenderTask CreateProxy() const override;
		RenderTaskList UpdateProxy() const override;
		void ClearDirty() override;
	};

	class InstancedStaticMeshNode : public RenderNode<InstancedTransformComponent, StaticMeshComponent, Optional<MaterialComponent>>
	{
		GENERATE(InstancedStaticMeshNode);
	public :
		Memory::ObjectPtr<InstancedTransformComponent> transform;
		Memory::ObjectPtr<StaticMeshComponent> mesh;
		Memory::ObjectPtr<MaterialComponent> overrideMaterial;

	public :
		InstancedStaticMeshNode(Memory::ObjectPtr<InstancedTransformComponent> transform, 
			Memory::ObjectPtr<StaticMeshComponent> mesh, 
			Memory::ObjectPtr<MaterialComponent> material = nullptr)
			: RenderNode(transform, mesh, material)
			, transform(transform)
			, mesh(mesh)
			, overrideMaterial(material)
		{
		}

		virtual ~InstancedStaticMeshNode() = default;

	public :
		void OnAttached(Scene* scene) override;
		void OnDetached() override;

		RenderTask CreateProxy() const override;
		RenderTaskList UpdateProxy() const override;
		void ClearDirty() override;
	};

	class DynamicMeshNode : public RenderNode<TransformComponent, DynamicMeshComponent, Optional<MaterialComponent>>
	{
		GENERATE(DynamicMeshNode);
	public :
		Memory::ObjectPtr<TransformComponent> transform;
		Memory::ObjectPtr<DynamicMeshComponent> mesh;
		Memory::ObjectPtr<MaterialComponent> overrideMaterial;

	public :
		DynamicMeshNode(Memory::ObjectPtr<TransformComponent> transform, 
			Memory::ObjectPtr<DynamicMeshComponent> mesh, 
			Memory::ObjectPtr<MaterialComponent> material = nullptr)
			: RenderNode(transform, mesh, material)
			, transform(transform)
			, mesh(mesh)
			, overrideMaterial(material)
		{
		}

		virtual ~DynamicMeshNode() = default;

	public :
		void OnAttached(Scene* scene) override;
		void OnDetached() override;

		RenderTask CreateProxy() const override;
		RenderTaskList UpdateProxy() const override;
		void ClearDirty() override;
	};

	class DirectionalLightNode : public RenderNode<TransformComponent, DirectionalLightComponent>
	{
		GENERATE(DirectionalLightNode);
	public :
		Memory::ObjectPtr<TransformComponent> transform;
		Memory::ObjectPtr<DirectionalLightComponent> light;

	public :
		DirectionalLightNode(Memory::ObjectPtr<TransformComponent> transform, 
			Memory::ObjectPtr<DirectionalLightComponent> light)
			: RenderNode(transform, light)
			, transform(transform)
			, light(light)
		{
		}
		virtual ~DirectionalLightNode() = default;

	public :
		void OnAttached(Scene* scene) override;
		void OnDetached() override;

		RenderTask CreateProxy() const override;
		RenderTaskList UpdateProxy() const override;
		void ClearDirty() override;
	};

	class PointLightNode : public RenderNode<TransformComponent, PointLightComponent>
	{
		GENERATE(PointLightNode);
	public :
		Memory::ObjectPtr<TransformComponent> transform;
		Memory::ObjectPtr<PointLightComponent> light;

	public :
		PointLightNode(Memory::ObjectPtr<TransformComponent> transform, 
			Memory::ObjectPtr<PointLightComponent> light)
			: RenderNode(transform, light)
			, transform(transform)
			, light(light)
		{
		}
		virtual ~PointLightNode() = default;

	public :
		void OnAttached(Scene* scene) override;
		void OnDetached() override;

		RenderTask CreateProxy() const override;
		RenderTaskList UpdateProxy() const override;
		void ClearDirty() override;
	};

	class SpotLightNode : public RenderNode<TransformComponent, SpotLightComponent>
	{
		GENERATE(SpotLightNode);
	public :
		Memory::ObjectPtr<TransformComponent> transform;
		Memory::ObjectPtr<SpotLightComponent> light;

	public :
		SpotLightNode(Memory::ObjectPtr<TransformComponent> transform, 
			Memory::ObjectPtr<SpotLightComponent> light)
			: RenderNode(transform, light)
			, transform(transform)
			, light(light)
		{
		}
		virtual ~SpotLightNode() = default;

	public :
		void OnAttached(Scene* scene) override;
		void OnDetached() override;

		RenderTask CreateProxy() const override;
		RenderTaskList UpdateProxy() const override;
		void ClearDirty() override;
	};
};

#endif // __WTR_RENDERNODE_H__