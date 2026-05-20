#include <Renderer/Proxy/MaterialProxy.h>

#include <Asset/AssetTypes.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIDescriptions.h>
#include <RHI/RHICommandList.h>

namespace wtr
{
	MaterialProxy::MaterialProxy(const ECS::UUID& id)
		: ECS::Object(id)
		, m_materialAsset(nullptr)
	{}

	eResourceState MaterialProxy::GetResourceState() const
	{
		eResourceState allState = m_materialAsset ? m_materialAsset->GetResourceState() : eResourceState::eNone;
		//allState &= m_vectorBuffer ? m_vectorBuffer->GetState() : eResourceState::eNone;
		//allState &= m_scalarBuffer ? m_scalarBuffer->GetState() : eResourceState::eNone;

		return allState;
	}

	void MaterialProxy::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
	}

	void MaterialProxy::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
	}

	void MaterialProxy::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_vectorBuffer)
		{
			cmdList->RemoveBuffer(m_vectorBuffer);
			m_vectorBuffer.Reset();
		}

		if (m_scalarBuffer)
		{
			cmdList->RemoveBuffer(m_scalarBuffer);
			m_scalarBuffer.Reset();
		}
	}

	void MaterialProxy::SetMaterialAsset(Memory::RefPtr<const MaterialAsset> material)
	{
		if (!material)
		{
			return;
		}

		m_materialAsset = material;

		UpdateMaterialDesc();
	}

	void MaterialProxy::UpdateMaterialDesc()
	{
		if (!m_materialAsset)
		{
			return;
		}

		// Update the material description
		m_materialDesc.shadingModel = m_materialAsset->shadingModel;
		m_materialDesc.blendMode = m_materialAsset->blendMode;
		m_materialDesc.isDoubleSided = m_materialAsset->isDoubleSided;
		m_materialDesc.isPBR = m_materialAsset->isPBR;

		m_materialDesc.hasAmbientMap = m_materialAsset->textures.Contains(eResourceSlot::eAmbient);
		m_materialDesc.hasDiffuseMap = m_materialAsset->textures.Contains(eResourceSlot::eDiffuse);
		m_materialDesc.hasSpecularMap = m_materialAsset->textures.Contains(eResourceSlot::eSpecular);
		m_materialDesc.hasEmissiveMap = m_materialAsset->textures.Contains(eResourceSlot::eEmissive);
		m_materialDesc.hasOpacityMap = m_materialAsset->textures.Contains(eResourceSlot::eOpacity);
		m_materialDesc.hasBumpMap = m_materialAsset->textures.Contains(eResourceSlot::eBump);
		m_materialDesc.hasNormalMap = m_materialAsset->textures.Contains(eResourceSlot::eNormal);
		m_materialDesc.hasRoughnessMap = m_materialAsset->textures.Contains(eResourceSlot::eRoughness);
		m_materialDesc.hasMetallicMap = m_materialAsset->textures.Contains(eResourceSlot::eMetallic);
		m_materialDesc.hasAmbientOcclusionMap = m_materialAsset->textures.Contains(eResourceSlot::eAmbientOcclusion);
		m_materialDesc.hasSheenMap = m_materialAsset->textures.Contains(eResourceSlot::eSheen);
	}

	const MaterialDesc& MaterialProxy::GetMaterialDesc() const
	{
		return m_materialDesc;
	}

	Memory::RefPtr<const TextureAsset> MaterialProxy::GetTexture(const eResourceSlot slot) const
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

	Memory::RefPtr<const MaterialAsset> MaterialProxy::GetMaterial() const
	{
		return m_materialAsset;
	}

	Memory::RefPtr<const RHIBuffer> MaterialProxy::GetVectorBuffer() const
	{
		return m_vectorBuffer;
	}

	Memory::RefPtr<const RHIBuffer> MaterialProxy::GetScalarBuffer() const
	{
		return m_scalarBuffer;
	}
}