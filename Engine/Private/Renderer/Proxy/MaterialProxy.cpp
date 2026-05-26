#include <Renderer/Proxy/MaterialProxy.h>

#include <Asset/AssetTypes.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIDescriptions.h>
#include <RHI/RHICommandList.h>

#include <Memory/include/Core.h>

namespace wtr
{
	MaterialProxy::MaterialProxy(const ECS::UUID& id)
		: ECS::Object(id)
		, m_materialAsset(nullptr)
		, m_materialBuffer(nullptr)
	{}

	eResourceState MaterialProxy::GetResourceState() const
	{
		eResourceState allState = m_materialAsset ? m_materialAsset->GetResourceState() : eResourceState::eNone;
		allState &= m_materialBuffer ? m_materialBuffer->GetState() : eResourceState::eNone;

		return allState;
	}

	void MaterialProxy::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_materialAsset || m_materialAsset->GetResourceState() != eResourceState::eReady)
		{
			return;
		}

		if (m_materialBuffer)
		{
			return;
		}

		const auto& vectorValues = m_materialAsset->vectorValues;
		const auto& scalarValues = m_materialAsset->scalarValues;

		Memory::RefPtr<RawData> materialParams = nullptr;
		if (m_materialAsset->isPBR)
		{
			const bool hasBaseColor = vectorValues.Find(eVectorSlot::eBaseColor) != vectorValues.End();
			const bool hasRoughness = scalarValues.Find(eScalarSlot::eRoughness) != scalarValues.End();
			const bool hasMetallic = scalarValues.Find(eScalarSlot::eMetallic) != scalarValues.End();
			const bool hasAmbientOcclusion = scalarValues.Find(eScalarSlot::eAmbientOcclusion) != scalarValues.End();
			const bool hasOpacity = scalarValues.Find(eScalarSlot::eOpacity) != scalarValues.End();

			Memory::RefPtr<ScalarData<PBRParams>> params = Memory::MakeRef<ScalarData<PBRParams>>();
			params->data.baseColor = hasBaseColor ? vectorValues[eVectorSlot::eBaseColor] : fvec3(1.f);
			params->data.roughness = hasRoughness ? scalarValues[eScalarSlot::eRoughness] : 0.5f;
			params->data.metallic = hasMetallic ? scalarValues[eScalarSlot::eMetallic] : 0.f;
			params->data.ambientOcclusion = hasAmbientOcclusion ? scalarValues[eScalarSlot::eAmbientOcclusion] : 1.f;
			params->data.opacity = hasOpacity ? scalarValues[eScalarSlot::eOpacity] : 1.f;

			materialParams = params;
		}
		else
		{
			const bool hasDiffuseColor = vectorValues.Find(eVectorSlot::eBaseColor) != vectorValues.End();
			const bool hasSpecularColor = vectorValues.Find(eVectorSlot::eSpecularColor) != vectorValues.End();
			const bool hasEmissiveColor = vectorValues.Find(eVectorSlot::eEmissiveColor) != vectorValues.End();
			const bool hasShininess = scalarValues.Find(eScalarSlot::eShininess) != scalarValues.End();
			const bool hasOpacity = scalarValues.Find(eScalarSlot::eOpacity) != scalarValues.End();

			Memory::RefPtr<ScalarData<PhongParams>> params = Memory::MakeRef<ScalarData<PhongParams>>();
			params->data.diffuse = hasDiffuseColor ? vectorValues[eVectorSlot::eBaseColor] : fvec3(1.f);
			params->data.specular = hasSpecularColor ? vectorValues[eVectorSlot::eSpecularColor] : fvec3(1.f);
			params->data.emissive = hasEmissiveColor ? vectorValues[eVectorSlot::eEmissiveColor] : fvec3(0.f);
			params->data.shininess = hasShininess ? scalarValues[eScalarSlot::eShininess] : 32.f;
			params->data.opacity = hasOpacity ? scalarValues[eScalarSlot::eOpacity] : 1.f;

			materialParams = params;
		}

		RHIBufferCreateDesc desc;
		desc.bufferType = eBufferType::eStorage;
		desc.accessType = eDataAccess::eDynamic;
		desc.componentType = eDataType::eNone;
		desc.numComponents = 1;
		desc.count = 1;
		desc.size = materialParams->GetSize();
		desc.stride = materialParams->GetSize();
		desc.dataRanges.PushBack({ 0, materialParams });

		Memory::RefPtr<RHIBuffer> buffer = cmdList->CreateBuffer(desc);
		if (buffer)
		{
			m_materialBuffer = buffer;
		}
	}

	void MaterialProxy::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_materialBuffer)
		{
			cmdList->RemoveBuffer(m_materialBuffer);
			m_materialBuffer.Reset();
		}
	}

	void MaterialProxy::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_materialBuffer)
		{
			return;
		}
		
		const auto& vectorValues = m_materialAsset->vectorValues;
		const auto& scalarValues = m_materialAsset->scalarValues;

		Memory::RefPtr<RawData> materialParams = nullptr;
		if (m_materialAsset->isPBR)
		{
			const bool hasBaseColor = vectorValues.Find(eVectorSlot::eBaseColor) != vectorValues.End();
			const bool hasRoughness = scalarValues.Find(eScalarSlot::eRoughness) != scalarValues.End();
			const bool hasMetallic = scalarValues.Find(eScalarSlot::eMetallic) != scalarValues.End();
			const bool hasAmbientOcclusion = scalarValues.Find(eScalarSlot::eAmbientOcclusion) != scalarValues.End();
			const bool hasOpacity = scalarValues.Find(eScalarSlot::eOpacity) != scalarValues.End();

			Memory::RefPtr<ScalarData<PBRParams>> params = Memory::MakeRef<ScalarData<PBRParams>>();
			params->data.baseColor = hasBaseColor ? vectorValues[eVectorSlot::eBaseColor] : fvec3(1.f);
			params->data.roughness = hasRoughness ? scalarValues[eScalarSlot::eRoughness] : 0.5f;
			params->data.metallic = hasMetallic ? scalarValues[eScalarSlot::eMetallic] : 0.f;
			params->data.ambientOcclusion = hasAmbientOcclusion ? scalarValues[eScalarSlot::eAmbientOcclusion] : 1.f;
			params->data.opacity = hasOpacity ? scalarValues[eScalarSlot::eOpacity] : 1.f;

			materialParams = params;
		}
		else
		{
			const bool hasDiffuseColor = vectorValues.Find(eVectorSlot::eBaseColor) != vectorValues.End();
			const bool hasSpecularColor = vectorValues.Find(eVectorSlot::eSpecularColor) != vectorValues.End();
			const bool hasEmissiveColor = vectorValues.Find(eVectorSlot::eEmissiveColor) != vectorValues.End();
			const bool hasShininess = scalarValues.Find(eScalarSlot::eShininess) != scalarValues.End();
			const bool hasOpacity = scalarValues.Find(eScalarSlot::eOpacity) != scalarValues.End();

			Memory::RefPtr<ScalarData<PhongParams>> params = Memory::MakeRef<ScalarData<PhongParams>>();
			params->data.diffuse = hasDiffuseColor ? vectorValues[eVectorSlot::eBaseColor] : fvec3(1.f);
			params->data.specular = hasSpecularColor ? vectorValues[eVectorSlot::eSpecularColor] : fvec3(1.f);
			params->data.emissive = hasEmissiveColor ? vectorValues[eVectorSlot::eEmissiveColor] : fvec3(0.f);
			params->data.shininess = hasShininess ? scalarValues[eScalarSlot::eShininess] : 32.f;
			params->data.opacity = hasOpacity ? scalarValues[eScalarSlot::eOpacity] : 1.f;

			materialParams = params;
		}

		RHIBufferUpdateDesc desc;
		desc.bufferType = eBufferType::eStorage;
		desc.accessType = eDataAccess::eDynamic;
		desc.componentType = eDataType::eNone;
		desc.numComponents = 1;
		desc.count = 1;
		desc.size = materialParams->GetSize();
		desc.stride = materialParams->GetSize();
		desc.dataRanges.PushBack({ 0, materialParams });

		cmdList->UpdateBuffer(desc, m_materialBuffer);
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

		m_materialDesc.hasTransparent = m_materialAsset->textures.Contains(eResourceSlot::eOpacity) && 
			m_materialAsset->scalarValues.Contains(eScalarSlot::eOpacity) && 
			m_materialAsset->scalarValues[eScalarSlot::eOpacity] < 1.0f;
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

	Memory::RefPtr<const RHIBuffer> MaterialProxy::GetMaterialBuffer() const
	{
		return m_materialBuffer;
	}
}