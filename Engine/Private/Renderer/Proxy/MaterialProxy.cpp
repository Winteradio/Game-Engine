#include <Renderer/Proxy/MaterialProxy.h>

#include <Asset/AssetTypes.h>
#include <RHI/RHIResources.h>

namespace wtr
{
	MaterialProxy::MaterialProxy(const ECS::UUID& id)
		: ECS::Object(id)
		, m_materialAsset(nullptr)
	{}

	eResourceState MaterialProxy::GetResourceState() const
	{
		eResourceState allState = ShaderProxy::GetResourceState();

		// TODO : Override Material
		allState &= m_materialAsset ? m_materialAsset->GetResourceState() : eResourceState::eReady;

		return allState;
	}

	void MaterialProxy::SetMaterialAsset(Memory::RefPtr<const MaterialAsset> material)
	{
		if (!material)
		{
			return;
		}

		m_materialAsset = material;

		// Update the material description
		m_materialDesc.shadingModel = material->shadingModel;
		m_materialDesc.blendMode = material->blendMode;
		m_materialDesc.isDoubleSided = material->isDoubleSided;
		m_materialDesc.isPBR = material->isPBR;

		m_materialDesc.hasAmbientMap = material->textures.Contains(eTextureSlot::eAmbient);
		m_materialDesc.hasDiffuseMap = material->textures.Contains(eTextureSlot::eDiffuse);
		m_materialDesc.hasSpecularMap = material->textures.Contains(eTextureSlot::eSpecular);
		m_materialDesc.hasEmissiveMap = material->textures.Contains(eTextureSlot::eEmissive);
		m_materialDesc.hasOpacityMap = material->textures.Contains(eTextureSlot::eOpacity);
		m_materialDesc.hasBumpMap = material->textures.Contains(eTextureSlot::eBump);
		m_materialDesc.hasNormalMap = material->textures.Contains(eTextureSlot::eNormal);
		m_materialDesc.hasRoughnessMap = material->textures.Contains(eTextureSlot::eRoughness);
		m_materialDesc.hasMetallicMap = material->textures.Contains(eTextureSlot::eMetallic);
		m_materialDesc.hasAmbientOcclusionMap = material->textures.Contains(eTextureSlot::eAmbientOcclusion);
		m_materialDesc.hasSheenMap = material->textures.Contains(eTextureSlot::eSheen);
	}

	Memory::RefPtr<const TextureAsset> MaterialProxy::GetTexture(const eTextureSlot slot) const
	{
		if (!m_materialAsset)
		{
			return nullptr;
		}

		auto itr = m_materialAsset->textures.Find(slot);
		if (itr != m_materialAsset->textures.End())
		{
			return itr->second;
		}

		return nullptr;
	}

	const MaterialDesc& MaterialProxy::GetMaterialDesc() const
	{
		return m_materialDesc;
	}

	const fvec3 MaterialProxy::GetVector(const eVectorSlot slot) const
	{
		if (!m_materialAsset)
		{
			return fvec3(0.0f);
		}

		auto itr = m_materialAsset->vectorValues.Find(slot);
		if (itr != m_materialAsset->vectorValues.End())
		{
			return itr->second;
		}

		return fvec3(0.0f);
	}

	const float MaterialProxy::GetScalar(const eScalarSlot slot) const
	{
		if (!m_materialAsset)
		{
			return 0.0f;
		}

		auto itr = m_materialAsset->scalarValues.Find(slot);
		if (itr != m_materialAsset->scalarValues.End())
		{
			return itr->second;
		}

		return 0.0f;
	}
}