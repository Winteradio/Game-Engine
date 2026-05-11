#ifndef __WTR_COMPONENT_H__
#define __WTR_COMPONENT_H__

#include <ECS/include/Object/Data.h>
#include <Container/include/DynamicArray.h>

#include <Asset/AssetTypes.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
	class Scene;
};

namespace wtr
{
	class BaseComponent : public ECS::Component
	{
		GENERATE(BaseComponent);

	public :
		using ECS::Component::Component;

		virtual ~BaseComponent() = default;
	};

	class CameraComponent : public BaseComponent
	{
		GENERATE(CameraComponent);

	public:
		using BaseComponent::BaseComponent;

		virtual ~CameraComponent() = default;

		float nearPlane = 0.1f;
		float farPlane = 1000.f;

		float fovY = 60.f;

		float width = 0.f;
		float height = 0.f;

		bool perspective = true;
	};

	class ProxyComponent : public BaseComponent
	{
		GENERATE(ProxyComponent);
	public :
		using BaseComponent::BaseComponent;

		ProxyComponent();
		virtual ~ProxyComponent() = default;

	public :
		void OnAttached(Scene* scene);
		void OnDetached();

		void ClearDirty();
		bool IsDirty() const;

	protected : 
		void OnUpdate();

	private :
		Scene* m_scene;

		bool m_isDirty;
	};

	class TransformComponent : public ProxyComponent
	{
		GENERATE(TransformComponent);
	public:
		using ProxyComponent::ProxyComponent;

		TransformComponent();
		virtual ~TransformComponent() = default;

	public :
		void UpdatePosition(const fvec3& position);
		void UpdateRotation(const fvec3& rotation);
		void UpdateRotation(const fquat& rotation);
		void UpdateScale(const fvec3& scale);

		const fvec3 GetPosition() const;
		const fquat GetRotation() const;
		const fvec3 GetScale() const;

	private :
		fvec3 m_position;
		fquat m_rotation;
		fvec3 m_scale;
	};

	class InstancedTransformComponent : public TransformComponent
	{
		GENERATE(InstancedTransformComponent);
	public:
		using TransformComponent::TransformComponent;
		using TransformComponent::UpdatePosition;
		using TransformComponent::UpdateRotation;
		using TransformComponent::UpdateScale;
		using TransformComponent::GetPosition;
		using TransformComponent::GetRotation;
		using TransformComponent::GetScale;

		InstancedTransformComponent();
		virtual ~InstancedTransformComponent() = default;

	public:
		void AddInstance(const fvec3& position, const fquat& rotation, const fvec3& scale);
		void RemoveInstance(const size_t instanceIndex);
		const size_t GetInstanceCount() const;

		void UpdatePosition(const size_t instanceIndex, const fvec3& position);
		void UpdateRotation(const size_t instanceIndex, const fvec3& rotation);
		void UpdateRotation(const size_t instanceIndex, const fquat& rotation);
		void UpdateScale(const size_t instanceIndex, const fvec3& scale);

		const fvec3 GetPosition(const size_t instanceIndex) const;
		const fquat GetRotation(const size_t instanceIndex) const;
		const fvec3 GetScale(const size_t instanceIndex) const;
		const ftransform GetTransform(const size_t instanceIndex) const;

		const wtr::DynamicArray<ftransform>& GetTransforms() const;

	private :
		wtr::DynamicArray<ftransform> m_instanceTransforms;
	};

	class LightComponent : public ProxyComponent
	{
		GENERATE(LightComponent);

	public:
		using ProxyComponent::ProxyComponent;

		LightComponent();
		virtual ~LightComponent() = default;
		
	public :
		void SetLightType(const eLightType lightType);
		void SetShadowType(const eShadowType shadowType);

		void UpdateColor(const fvec3& color);
		void UpdateDirection(const fvec3& direction);
		void UpdateIntensity(const float intensity);

		const eLightType GetLightType() const;
		const eShadowType GetShadowType() const;
		const fvec3 GetColor() const;
		const fvec3 GetDirection() const;
		const float GetIntensity() const;

	private :
		eLightType m_lightType;
		eShadowType m_shadowType;

		fvec3 m_color;
		fvec3 m_direction;
		float m_intensity;
	};

	class DirectionalLightComponent : public LightComponent
	{
		GENERATE(DirectionalLightComponent);

	public :
		using LightComponent::LightComponent;

		virtual ~DirectionalLightComponent() = default;
	};

	class PointLightComponent : public LightComponent
	{
		GENERATE(PointLightComponent);

	public :
		using LightComponent::LightComponent;

		PointLightComponent();
		virtual ~PointLightComponent() = default;

	public :
		void UpdateRange(const float range);

		const float GetRange() const;

	private :
		float m_range;
	};

	class SpotLightComponent : public LightComponent
	{
		GENERATE(SpotLightComponent);

	public :
		using LightComponent::LightComponent;

		SpotLightComponent();
		virtual ~SpotLightComponent() = default;

	public :
		void UpdateRange(const float range);
		void UpdateInnerAngle(const float innerAngle);
		void UpdateOuterAngle(const float outerAngle);

		const float GetRange() const;
		const float GetInnerAngle() const;
		const float GetOuterAngle() const;

	private :
		float m_range;
		float m_innerAngle;
		float m_outerAngle;
	};

	class MeshComponent : public ProxyComponent
	{
		GENERATE(MeshComponent);

	public :
		using ProxyComponent::ProxyComponent;

		MeshComponent() = default;
		virtual ~MeshComponent() = default;

	public :
		virtual Memory::RefPtr<const MeshAsset> GetMeshAsset() const = 0;
	};

	class StaticMeshComponent : public MeshComponent
	{
		GENERATE(StaticMeshComponent);
	public :
		using MeshComponent::MeshComponent;

		StaticMeshComponent() = default;
		StaticMeshComponent(Memory::RefPtr<const Asset> refAsset);
		virtual ~StaticMeshComponent() = default;

	public :
		Memory::RefPtr<const MeshAsset> GetMeshAsset() const override;

	private :
		Memory::RefPtr<const MeshAsset> m_refMesh;
	};

	class DynamicMeshComponent : public MeshComponent
	{
		GENERATE(DynamicMeshComponent);
	public :
		using MeshComponent::MeshComponent;

		DynamicMeshComponent() = default;
		DynamicMeshComponent(Memory::RefPtr<Asset> refAsset);
		virtual ~DynamicMeshComponent() = default;

	public :
		Memory::RefPtr<const MeshAsset> GetMeshAsset() const override;
		
	public :
		void UpdateVertex(const VertexKey& vertexKey, const FormattedBuffer& vertexBuffer);	
		void UpdateIndex(const FormattedBuffer& indexBuffer);
		void UpdateSection(const MeshSection& section);
		void UpdateDrawMode(const eDrawMode drawMode);
		void SetMaterial(const std::string& sectionName, Memory::RefPtr<const MaterialAsset> refMaterial);

	private :
		Memory::RefPtr<MeshAsset> m_refMesh;
	};
	
	class MaterialComponent : public BaseComponent
	{
		GENERATE(MaterialComponent);

	public :
		using BaseComponent::BaseComponent;

		MaterialComponent() = default;
		MaterialComponent(Memory::RefPtr<Asset> refAsset);
		virtual ~MaterialComponent() = default;

	public :
		virtual Memory::RefPtr<const MaterialAsset> GetMaterialAsset() const;

	private :
		Memory::RefPtr<MaterialAsset> m_refMaterial;
	};
};

#endif // __WTR_COMPONENT_H__