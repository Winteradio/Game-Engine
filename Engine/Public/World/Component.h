#ifndef __WTR_COMPONENT_H__
#define __WTR_COMPONENT_H__

#include <ECS/include/Object/Data.h>
#include <Framework/Math/MathTypes.h>
#include <Asset/AssetTypes.h>

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

	class SceneComponent : public BaseComponent
	{
		GENERATE(SceneComponent);

	public:
		using BaseComponent::BaseComponent;

		SceneComponent();
		virtual ~SceneComponent() = default;

	public:
		void OnAttached(Scene* scene);
		void OnDetached();

		void UpdatePosition(const fvec3& position);
		void UpdateRotation(const fvec3& rotation);
		void UpdateRotation(const fquat& rotation);
		void UpdateScale(const fvec3& scale);

		const fvec3 GetPosition() const;
		const fquat GetRotation() const;
		const fvec3 GetScale() const;

	private :
		void Update();

	private :
		fvec3 m_position;
		fquat m_rotation;
		fvec3 m_scale;

		Scene* m_scene;
	};

	class ColorComponent : public BaseComponent
	{
		GENERATE(ColorComponent);

	public:
		using BaseComponent::BaseComponent;

		virtual ~ColorComponent() = default;

		float red = 1.f;
		float blue = 1.f;
		float green = 1.f;
		float alpha = 1.f;
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

	class LightComponent : public BaseComponent
	{
		GENERATE(LightComponent);

	public:
		using BaseComponent::BaseComponent;

		virtual ~LightComponent() = default;

		fvec3 direction = fvec3(0.f);
	};

	class MeshComponent : public BaseComponent
	{
		GENERATE(MeshComponent);

	public :
		using BaseComponent::BaseComponent;

		MeshComponent() = default;
		MeshComponent(Memory::RefPtr<Asset> refAsset);
		virtual ~MeshComponent() = default;

		Memory::RefPtr<MeshAsset> meshAsset;
	};
	
	class MaterialComponent : public BaseComponent
	{
		GENERATE(MaterialComponent);

	public :
		using BaseComponent::BaseComponent;

		MaterialComponent() = default;
		MaterialComponent(Memory::RefPtr<Asset> refAsset);
		virtual ~MaterialComponent() = default;

		Memory::RefPtr<MaterialAsset> materialAsset;
	};
};

#endif // __WTR_COMPONENT_H__