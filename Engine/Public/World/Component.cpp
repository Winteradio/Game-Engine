#include <World/Component.h>

#include <World/Scene.h>
#include <Memory/include/Core.h>

namespace wtr
{
	SceneComponent::SceneComponent()
		: m_position(0.f, 0.f, 0.f)
		, m_rotation(0.f, 0.f, 0.f)
		, m_scale(1.f, 1.f, 1.f)
		, m_scene(nullptr)
	{}

	void SceneComponent::OnAttached(Scene* scene)
	{
		m_scene = scene;
	}

	void SceneComponent::OnDetached()
	{
		m_scene = nullptr;
	}

	void SceneComponent::UpdatePosition(const fvec3& position)
	{
		if (position != this->m_position)
		{
			this->m_position = position;
			Update();
		}
	}

	void SceneComponent::UpdateRotation(const fvec3& rotation)
	{
		if (rotation != this->m_rotation)
		{
			this->m_rotation = rotation;
			Update();
		}
	}

	void SceneComponent::UpdateScale(const fvec3& scale)
	{
		if (scale != this->m_scale)
		{
			this->m_scale = scale;
			Update();
		}
	}

	const fvec3 SceneComponent::GetPosition() const
	{
		return m_position;
	}

	const fvec3 SceneComponent::GetRotation() const
	{
		return m_rotation;
	}

	const fvec3 SceneComponent::GetScale() const
	{
		return m_scale;
	}

	void SceneComponent::Update()
	{
		if (m_scene)
		{
			m_scene->Update(GetID());
		}
	}

	MeshComponent::MeshComponent(Memory::RefPtr<Asset> refAsset)
		: meshAsset(nullptr)
	{
		if (!refAsset)
		{
			return;
		}

		auto refMesh = Memory::Cast<MeshAsset>(refAsset);
		if (refMesh)
		{
			meshAsset = refMesh;
		}
	}

	MaterialComponent::MaterialComponent(Memory::RefPtr<Asset> refAsset)
		: materialAsset(nullptr)
	{
		if (!refAsset)
		{
			return;
		}

		auto refMaterial = Memory::Cast<MaterialAsset>(refAsset);
		if (refMaterial)
		{
			materialAsset = refMaterial;
		}
	}
}