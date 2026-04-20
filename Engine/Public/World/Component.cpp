#include <World/Component.h>

#include <World/Scene.h>
#include <Memory/include/Core.h>

namespace wtr
{
	SceneComponent::SceneComponent()
		: m_position(0.f, 0.f, 0.f)
		, m_rotation(1.f, 0.f, 0.f, 0.f)
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
		if ((std::abs(position.x - this->m_position.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(position.y - this->m_position.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(position.z - this->m_position.z) >= std::numeric_limits<float>::epsilon()))
		{
			this->m_position = position;
			Update();
		}
	}

	void SceneComponent::UpdateRotation(const fquat& rotation)
	{
		if ((std::abs(rotation.x - this->m_rotation.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.y - this->m_rotation.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.z - this->m_rotation.z) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.w - this->m_rotation.w) >= std::numeric_limits<float>::epsilon()))
		{
			this->m_rotation = glm::quat(rotation);
			Update();
		}
	}

	void SceneComponent::UpdateRotation(const fvec3& rotation)
	{
		const fvec3 eulerAngles = glm::eulerAngles(this->m_rotation);
		if ((std::abs(rotation.x - eulerAngles.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.y - eulerAngles.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.z - eulerAngles.z) >= std::numeric_limits<float>::epsilon()))
		{
			this->m_rotation = glm::quat(rotation);
			Update();
		}
	}

	void SceneComponent::UpdateScale(const fvec3& scale)
	{
		if ((std::abs(scale.x - this->m_scale.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(scale.y - this->m_scale.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(scale.z - this->m_scale.z) >= std::numeric_limits<float>::epsilon()))
		{
			this->m_scale = scale;
			Update();
		}
	}

	const fvec3 SceneComponent::GetPosition() const
	{
		return m_position;
	}

	const fquat SceneComponent::GetRotation() const
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