#include <World/Component.h>

#include <World/Scene.h>
#include <Memory/include/Core.h>
#include <Asset/AssetCommander.h>

namespace wtr
{
	ProxyComponent::ProxyComponent()
		: m_scene(nullptr)
		, m_isDirty(false)
	{}

	void ProxyComponent::OnAttached(Scene* scene)
	{
		if (nullptr != scene)
		{
			m_scene = scene;
		}
	}

	void ProxyComponent::OnDetached()
	{
		if (nullptr != m_scene)
		{
			m_scene = nullptr;
		}
	}

	void ProxyComponent::OnUpdate()
	{
		m_isDirty = true;

		if (m_scene)
		{
			m_scene->Update(GetID());
		}
	}

	void ProxyComponent::ClearDirty()
	{
		m_isDirty = false;
	}

	bool ProxyComponent::IsDirty() const
	{
		return m_isDirty;
	}

	TransformComponent::TransformComponent()
		: ProxyComponent()
		, m_position(0.f, 0.f, 0.f)
		, m_rotation(1.f, 0.f, 0.f, 0.f)
		, m_scale(1.f, 1.f, 1.f)
	{}

	void TransformComponent::UpdatePosition(const fvec3& position)
	{
		if ((std::abs(position.x - this->m_position.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(position.y - this->m_position.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(position.z - this->m_position.z) >= std::numeric_limits<float>::epsilon()))
		{
			this->m_position = position;
			this->OnUpdate();
		}
	}

	void TransformComponent::UpdateRotation(const fquat& rotation)
	{
		if ((std::abs(rotation.x - this->m_rotation.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.y - this->m_rotation.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.z - this->m_rotation.z) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.w - this->m_rotation.w) >= std::numeric_limits<float>::epsilon()))
		{
			this->m_rotation = rotation;
			this->OnUpdate();
		}
	}

	void TransformComponent::UpdateRotation(const fvec3& rotation)
	{
		const fvec3 eulerAngles = glm::eulerAngles(this->m_rotation);
		if ((std::abs(rotation.x - eulerAngles.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.y - eulerAngles.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.z - eulerAngles.z) >= std::numeric_limits<float>::epsilon()))
		{
			this->m_rotation = glm::quat(rotation);
			this->OnUpdate();
		}
	}

	void TransformComponent::UpdateScale(const fvec3& scale)
	{
		if ((std::abs(scale.x - this->m_scale.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(scale.y - this->m_scale.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(scale.z - this->m_scale.z) >= std::numeric_limits<float>::epsilon()))
		{
			this->m_scale = scale;
			this->OnUpdate();
		}
	}

	const fvec3 TransformComponent::GetPosition() const
	{
		return m_position;
	}

	const fquat TransformComponent::GetRotation() const
	{
		return m_rotation;
	}

	const fvec3 TransformComponent::GetScale() const
	{
		return m_scale;
	}

	InstancedTransformComponent::InstancedTransformComponent()
		: TransformComponent()
		, m_instanceTransforms()
	{}

	void InstancedTransformComponent::AddInstance(const fvec3& position, const fquat& rotation, const fvec3& scale)
	{
		m_instanceTransforms.EmplaceBack(position, rotation, scale);
	
		this->OnUpdate();
	}

	void InstancedTransformComponent::RemoveInstance(const size_t instanceIndex)
	{
		if (instanceIndex >= m_instanceTransforms.Size())
		{
			return;
		}

		std::swap(m_instanceTransforms[instanceIndex], m_instanceTransforms.Back());
		m_instanceTransforms.PopBack();

		this->OnUpdate();
	}

	const size_t InstancedTransformComponent::GetInstanceCount() const
	{
		return m_instanceTransforms.Size();
	}

	void InstancedTransformComponent::UpdatePosition(const size_t instanceIndex, const fvec3& position)
	{
		if (instanceIndex >= m_instanceTransforms.Size())
		{
			return;
		}

		auto& transform = m_instanceTransforms[instanceIndex];
		if ((std::abs(position.x - transform.position.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(position.y - transform.position.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(position.z - transform.position.z) >= std::numeric_limits<float>::epsilon()))
		{
			transform.position = position;
			this->OnUpdate();
		}
	}

	void InstancedTransformComponent::UpdateRotation(const size_t instanceIndex, const fvec3& rotation)
	{
		if (instanceIndex >= m_instanceTransforms.Size())
		{
			return;
		}
	
		auto& transform = m_instanceTransforms[instanceIndex];
		const fvec3 eulerAngles = glm::eulerAngles(transform.rotation);
		if ((std::abs(rotation.x - eulerAngles.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.y - eulerAngles.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.z - eulerAngles.z) >= std::numeric_limits<float>::epsilon()))
		{
			transform.rotation = glm::quat(eulerAngles);
		
			this->OnUpdate();
		}
	}

	void InstancedTransformComponent::UpdateRotation(const size_t instanceIndex, const fquat& rotation)
	{
		if (instanceIndex >= m_instanceTransforms.Size())
		{
			return;
		}
	
		auto& transform = m_instanceTransforms[instanceIndex];
		if ((std::abs(rotation.x - transform.rotation.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.y - transform.rotation.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.z - transform.rotation.z) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(rotation.w - transform.rotation.w) >= std::numeric_limits<float>::epsilon()))
		{
			transform.rotation = rotation;
		
			this->OnUpdate();
		}
	}

	void InstancedTransformComponent::UpdateScale(const size_t instanceIndex, const fvec3& scale)
	{
		if (instanceIndex >= m_instanceTransforms.Size())
		{
			return;
		}
	
		auto& transform = m_instanceTransforms[instanceIndex];
		if ((std::abs(scale.x - transform.scale.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(scale.y - transform.scale.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(scale.z - transform.scale.z) >= std::numeric_limits<float>::epsilon()))
		{
			transform.scale = scale;
	
			this->OnUpdate();
		}
	}

	const fvec3 InstancedTransformComponent::GetPosition(const size_t instanceIndex) const
	{
		if (instanceIndex >= m_instanceTransforms.Size())
		{
			return fvec3(0.f, 0.f, 0.f);
		}

		auto& transform = m_instanceTransforms[instanceIndex];
		return transform.position;
	}

	const fquat InstancedTransformComponent::GetRotation(const size_t instanceIndex) const
	{
		if (instanceIndex >= m_instanceTransforms.Size())
		{
			return fquat(1.f, 0.f, 0.f, 0.f);
		}
	
		auto& transform = m_instanceTransforms[instanceIndex];
		return transform.rotation;
	}

	const fvec3 InstancedTransformComponent::GetScale(const size_t instanceIndex) const
	{
		if (instanceIndex >= m_instanceTransforms.Size())
		{
			return fvec3(1.f, 1.f, 1.f);
		}
	
		auto& transform = m_instanceTransforms[instanceIndex];
		return transform.scale;
	}

	const ftransform InstancedTransformComponent::GetTransform(const size_t instanceIndex) const
	{
		if (instanceIndex >= m_instanceTransforms.Size())
		{
			return {};
		}

		return m_instanceTransforms[instanceIndex];
	}

	const wtr::DynamicArray<ftransform>& InstancedTransformComponent::GetTransforms() const
	{
		return m_instanceTransforms;
	}

	LightComponent::LightComponent()
		: ProxyComponent()
		, m_lightType(eLightType::eDirectional)
		, m_shadowType(eShadowType::eSoft)
		, m_color(1.f, 1.f, 1.f)
		, m_direction(0.f, 0.f, 0.f)
		, m_intensity(1.f)
	{}

	void LightComponent::SetLightType(const eLightType lightType)
	{
		if (lightType != m_lightType)
		{
			m_lightType = lightType;
	
			this->OnUpdate();
		}
	}

	void LightComponent::SetShadowType(const eShadowType shadowType)
	{
		if (shadowType != m_shadowType)
		{
			m_shadowType = shadowType;
	
			this->OnUpdate();
		}
	}

	void LightComponent::UpdateColor(const fvec3& color)
	{
		if ((std::abs(color.x - m_color.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(color.y - m_color.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(color.z - m_color.z) >= std::numeric_limits<float>::epsilon()))
		{
			m_color = color;
	
			this->OnUpdate();
		}
	}

	void LightComponent::UpdateDirection(const fvec3& direction)
	{
		if ((std::abs(direction.x - m_direction.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(direction.y - m_direction.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(direction.z - m_direction.z) >= std::numeric_limits<float>::epsilon()))
		{
			m_direction = direction;
	
			this->OnUpdate();
		}
	}

	void LightComponent::UpdateIntensity(const float intensity)
	{
		if (std::abs(intensity - m_intensity) >= std::numeric_limits<float>::epsilon())
		{
			m_intensity = intensity;
	
			this->OnUpdate();
		}
	}

	const eLightType LightComponent::GetLightType() const
	{
		return m_lightType;
	}

	const eShadowType LightComponent::GetShadowType() const
	{
		return m_shadowType;
	}

	const fvec3 LightComponent::GetColor() const
	{
		return m_color;
	}

	const fvec3 LightComponent::GetDirection() const
	{
		return m_direction;
	}

	const float LightComponent::GetIntensity() const
	{
		return m_intensity;
	}

	PointLightComponent::PointLightComponent()
		: LightComponent()
		, m_range(1.f)
	{}

	void PointLightComponent::UpdateRange(const float range)
	{
		if (std::abs(range - m_range) >= std::numeric_limits<float>::epsilon())
		{
			m_range = range;
	
			this->OnUpdate();
		}
	}

	const float PointLightComponent::GetRange() const
	{
		return m_range;
	}

	SpotLightComponent::SpotLightComponent()
		: LightComponent()
		, m_range(1.f)
		, m_innerAngle(0.f)
		, m_outerAngle(0.f)
	{}

	void SpotLightComponent::UpdateRange(const float range)
	{
		if (std::abs(range - m_range) >= std::numeric_limits<float>::epsilon())
		{
			m_range = range;
	
			this->OnUpdate();
		}
	}

	void SpotLightComponent::UpdateInnerAngle(const float innerAngle)
	{
		if (std::abs(innerAngle - m_innerAngle) >= std::numeric_limits<float>::epsilon())
		{
			m_innerAngle = innerAngle;
	
			this->OnUpdate();
		}
	}

	void SpotLightComponent::UpdateOuterAngle(const float outerAngle)
	{
		if (std::abs(outerAngle - m_outerAngle) >= std::numeric_limits<float>::epsilon())
		{
			m_outerAngle = outerAngle;
	
			this->OnUpdate();
		}
	}

	const float SpotLightComponent::GetRange() const
	{
		return m_range;
	}

	const float SpotLightComponent::GetInnerAngle() const
	{
		return m_innerAngle;
	}

	const float SpotLightComponent::GetOuterAngle() const
	{
		return m_outerAngle;
	}

	MeshComponent::MeshComponent()
		: ProxyComponent()
	{
	}

	bool MeshComponent::IsChanged() const
	{
		return m_isChanged;
	}

	void MeshComponent::OnChanged()
	{
		m_isChanged = true;
	}

	void MeshComponent::ClearChanged()
	{
		m_isChanged = false;
	}

	StaticMeshComponent::StaticMeshComponent(Memory::RefPtr<const Asset> refAsset)
		: MeshComponent()
		, m_refMesh(nullptr)
	{
		if (!refAsset)
		{
			return;
		}
	
		auto refMesh = Memory::Cast<const MeshAsset>(refAsset);
		if (refMesh)
		{
			m_refMesh = refMesh;

			this->IsChanged();
		}
	}

	Memory::RefPtr<const MeshAsset> StaticMeshComponent::GetMeshAsset() const
	{
		return m_refMesh;
	}

	void StaticMeshComponent::SetMeshAsset(Memory::RefPtr<const MeshAsset> meshAsset)
	{
		if (!meshAsset || meshAsset.Get() == m_refMesh.Get())
		{
			return;
		}

		m_refMesh = meshAsset;

		this->OnChanged();
		this->OnUpdate();
	}

	DynamicMeshComponent::DynamicMeshComponent(Memory::RefPtr<Asset> refAsset)
		: MeshComponent()
		, m_refMesh(nullptr)
	{
		if (!refAsset)
		{
			return;
		}
	
		auto refMesh = Memory::Cast<MeshAsset>(refAsset);
		if (refMesh)
		{
			m_refMesh = refMesh;

			this->IsChanged();
		}
	}

	Memory::RefPtr<const MeshAsset> DynamicMeshComponent::GetMeshAsset() const
	{
		return m_refMesh;
	}

	void DynamicMeshComponent::UpdateVertex(const VertexKey& vertexKey, const FormattedBuffer& vertexBuffer)
	{
		if (!m_refMesh)
		{
			return;
		}

		// TODO : Update the vertex buffer of the mesh asset and update the mesh component.
	}

	void DynamicMeshComponent::UpdateIndex(const FormattedBuffer& indexBuffer)
	{
		if (!m_refMesh)
		{
			return;
		}

		// TODO : Update the index buffer of the mesh asset and update the mesh component.
	}

	void DynamicMeshComponent::UpdateSection(const MeshSection& section)
	{
		if (!m_refMesh)
		{
			return;
		}

		// TODO : Update the section of the mesh asset and update the mesh component.
	}

	void DynamicMeshComponent::UpdateDrawMode(const eDrawMode drawMode)
	{
		if (!m_refMesh)
		{
			return;
		}

		// TODO : Update the draw mode of the mesh asset and update the mesh component.
	}

	void DynamicMeshComponent::SetMeshAsset(Memory::RefPtr<MeshAsset> meshAsset)
	{
		if (!meshAsset || meshAsset.Get() != m_refMesh.Get())
		{
			return;
		}

		m_refMesh = meshAsset;

		this->OnChanged();
		this->OnUpdate();
	}

	void DynamicMeshComponent::SetMaterialAsset(const std::string& sectionName, Memory::RefPtr<const MaterialAsset> refMaterial)
	{
		if (!m_refMesh)
		{
			return;
		}

		// TODO : Update the material of the mesh asset and update the mesh component.
	}

	MaterialComponent::MaterialComponent(Memory::RefPtr<Asset> refAsset)
		: ProxyComponent()
		, m_refMaterial(nullptr)
	{
		if (!refAsset)
		{
			return;
		}

		auto refMaterial = Memory::Cast<MaterialAsset>(refAsset);
		if (refMaterial)
		{
			m_refMaterial = refMaterial;
		}
	}

	Memory::RefPtr<const MaterialAsset> MaterialComponent::GetMaterialAsset() const
	{
		return m_refMaterial;
	}

	void MaterialComponent::UpdateVector(const eVectorSlot slot, const fvec3& value)
	{
		if (!m_refMaterial)
		{
			return;
		}

		auto itr = m_refMaterial->vectorValues.Find(slot);
		if (itr == m_refMaterial->vectorValues.End())
		{
			m_refMaterial->vectorValues[slot] = value;

			this->OnUpdate();
		}
		else
		{
			auto& prevValue = itr->second;
			if ((std::abs(prevValue.x - value.x) >= std::numeric_limits<float>::epsilon()) ||
				(std::abs(prevValue.y - value.y) >= std::numeric_limits<float>::epsilon()) ||
				(std::abs(prevValue.z - value.z) >= std::numeric_limits<float>::epsilon()))
			{
				prevValue = value;

				this->OnUpdate();
			}
		}
	}

	void MaterialComponent::UpdateScalar(const eScalarSlot slot, const float value)
	{
		if (!m_refMaterial)
		{
			return;
		}

		auto itr = m_refMaterial->scalarValues.Find(slot);
		if (itr == m_refMaterial->scalarValues.End())
		{
			m_refMaterial->scalarValues[slot] = value;

			this->OnUpdate();
		}
		else
		{
			auto& prevValue = itr->second;
			if ((std::abs(prevValue - value) >= std::numeric_limits<float>::epsilon()))
			{
				prevValue = value;

				this->OnUpdate();
			}
		}
	}

	void MaterialComponent::SetShadingModel(const eShadingModel shading)
	{
		if (!m_refMaterial)
		{
			return;
		}

		if (shading != m_refMaterial->shadingModel)
		{
			m_refMaterial->shadingModel = shading;

			this->OnUpdate();
		}
	}

	void MaterialComponent::SetBlendMode(const eBlendMode blend)
	{
		// TODO
	}

	void MaterialComponent::SetDoubleSide(const bool doubleSide)
	{
		// TODO
	}

	void MaterialComponent::SetPBR(const bool pbr)
	{
		// TODO
	}
}