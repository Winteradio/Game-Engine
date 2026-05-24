#include <Renderer/Proxy/LightProxy.h>

#include <RHI/RHICommandList.h>
#include <RHI/RHIResources.h>
#include <Memory/include/Core.h>

namespace wtr
{
	LightProxy::LightProxy(const ECS::UUID& id)
		: SceneProxy(id)
		, m_lightDesc()
		, m_lightBuffer()
		, m_shadowMap()
		, m_shadowTarget()
	{}

	eResourceState LightProxy::GetResourceState() const
	{
		eResourceState allState = m_lightBuffer ? m_lightBuffer->GetState() : eResourceState::eNone;

		allState &= m_shadowMap ? m_shadowMap->GetState() : eResourceState::eNone;
		allState &= m_shadowTarget ? m_shadowTarget->GetState() : eResourceState::eNone;

		return allState;
	}

	bool LightProxy::IsUploadable() const
	{
		return true;
	}

	void LightProxy::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_lightBuffer)
		{
			cmdList->RemoveBuffer(m_lightBuffer);
			m_lightBuffer.Reset();
		}

		if (m_shadowMap)
		{
			cmdList->RemoveTexture(m_shadowMap);
			m_shadowMap.Reset();
		}

		if (m_shadowTarget)
		{
			cmdList->RemoveRenderTarget(m_shadowTarget);
			m_shadowTarget.Reset();
		}
	}

	void LightProxy::SetLightType(const eLightType lightType)
	{
		if (m_lightDesc.lightType != lightType)
		{
			m_lightDesc.lightType = lightType;

			OnUpdate();
		}
	}

	void LightProxy::SetShadowType(const eShadowType shadowType)
	{
		if (m_lightDesc.shadowType != shadowType)
		{
			m_lightDesc.shadowType = shadowType;

			OnUpdate();

			// TODO : Shadow
			// GlobalShaderSelector�� ���� ShadowRenderTarget ����
		}
	}

	void LightProxy::SetColor(const fvec3 color)
	{
		auto lightData = GetLightData();
		if (lightData == nullptr)
		{
			return;
		}

		if ((std::abs(color.r - lightData->color.r) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(color.g - lightData->color.g) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(color.b - lightData->color.b) >= std::numeric_limits<float>::epsilon()))
		{
			lightData->color = color;

			OnUpdate();
		}
	}

	void LightProxy::SetIntensity(const float intensity)
	{
		auto lightData = GetLightData();
		if (lightData == nullptr)
		{
			return;
		}

		if (std::abs(intensity - lightData->intensity) >= std::numeric_limits<float>::epsilon())
		{
			lightData->intensity = intensity;

			OnUpdate();
		}
	}

	Memory::RefPtr<const RHIBuffer> LightProxy::GetLightBuffer() const
	{
		return m_lightBuffer;
	}

	Memory::RefPtr<const RHITexture> LightProxy::GetShadowMap() const
	{
		return m_shadowMap;
	}

	Memory::RefPtr<const RHIRenderTarget> LightProxy::GetShadowTarget() const
	{
		return m_shadowTarget;
	}

	const LightDesc& LightProxy::GetLightDesc() const
	{
		return m_lightDesc;
	}

	DirectionalLightProxy::DirectionalLightProxy(const ECS::UUID& id)
		: LightProxy(id)
		, m_directional(Memory::MakeRef<ScalarData<DirectionalLight>>())
	{
	}

	bool DirectionalLightProxy::IsSyncable() const
	{
		return m_directional;
	}

	void DirectionalLightProxy::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_directional)
		{
			return;
		}

		auto& data = m_directional->data;
		data.direction = GetRotation() * BASE_DIR;
		data.pos = GetPosition();

		if (!m_lightBuffer)
		{
			RHIBufferCreateDesc desc;
			desc.bufferType = eBufferType::eStorage;
			desc.accessType = eDataAccess::eDynamic;
			desc.componentType = eDataType::eNone;
			desc.numComponents = 1;
			desc.count = 1;
			desc.size = sizeof(DirectionalLight);
			desc.stride = sizeof(DirectionalLight);
			desc.dataRanges.PushBack({ 0, m_directional });

			Memory::RefPtr<RHIBuffer> buffer = cmdList->CreateBuffer(desc);
			if (buffer)
			{
				m_lightBuffer = buffer;
			}
		}

		// TODO : Shadow
	}

	void DirectionalLightProxy::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_lightBuffer || !m_directional)
		{
			return;
		}

		auto& data = m_directional->data;
		data.direction = GetRotation() * BASE_DIR;
		data.pos = GetPosition();

		RHIBufferUpdateDesc desc;
		desc.bufferType = eBufferType::eStorage;
		desc.accessType = eDataAccess::eDynamic;
		desc.componentType = eDataType::eNone;
		desc.numComponents = 1;
		desc.count = 1;
		desc.size = sizeof(DirectionalLight);
		desc.stride = sizeof(DirectionalLight);
		desc.dataRanges.PushBack({ 0, m_directional });

		cmdList->UpdateBuffer(desc, m_lightBuffer);
	}

	const fmat4 DirectionalLightProxy::GetViewMatrix() const
	{
		fmat4 viewMatrix = fmat4(1.0f);

		fmat4 rotation = glm::toMat4(GetRotation());

		fmat4 inverseRotation = glm::transpose(rotation);
		fmat4 inverseTranslate = glm::translate(fmat4(1.0f), -1.0f * GetPosition());
		
		viewMatrix *= inverseRotation * inverseTranslate;

		return viewMatrix;
	}

	const fmat4 DirectionalLightProxy::GetProjectionMatrix() const
	{
		// TODO : Shadow

		return fmat4(1.0f);
	}

	LightProxy::Light* DirectionalLightProxy::GetLightData()
	{
		if (m_directional)
		{
			return &m_directional->data;
		}
		else
		{
			return nullptr;
		}
	}

	PointLightProxy::PointLightProxy(const ECS::UUID& id)
		: LightProxy(id)
		, m_point(Memory::MakeRef<ScalarData<PointLight>>())
	{
	}

	bool PointLightProxy::IsSyncable() const
	{
		return m_point;
	}

	void PointLightProxy::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_point)
		{
			return;
		}

		auto& data = m_point->data;
		data.pos = GetPosition();

		if (!m_lightBuffer)
		{
			RHIBufferCreateDesc desc;
			desc.bufferType = eBufferType::eStorage;
			desc.accessType = eDataAccess::eDynamic;
			desc.componentType = eDataType::eNone;
			desc.numComponents = 1;
			desc.count = 1;
			desc.size = sizeof(PointLight);
			desc.stride = sizeof(PointLight);
			desc.dataRanges.PushBack({ 0, m_point });

			Memory::RefPtr<RHIBuffer> buffer = cmdList->CreateBuffer(desc);
			if (buffer)
			{
				m_lightBuffer = buffer;
			}
		}

		// TODO : Shadow
	}

	void PointLightProxy::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_lightBuffer || !m_point)
		{
			return;
		}

		auto& data = m_point->data;
		data.pos = GetPosition();

		RHIBufferUpdateDesc desc;
		desc.bufferType = eBufferType::eStorage;
		desc.accessType = eDataAccess::eDynamic;
		desc.componentType = eDataType::eNone;
		desc.numComponents = 1;
		desc.count = 1;
		desc.size = sizeof(PointLight);
		desc.stride = sizeof(PointLight);
		desc.dataRanges.PushBack({ 0, m_point });

		cmdList->UpdateBuffer(desc, m_lightBuffer);
	}

	const fmat4 PointLightProxy::GetViewMatrix() const
	{
		// TODO : Shadow

		return fmat4(1.0f);
	}

	const fmat4 PointLightProxy::GetProjectionMatrix() const
	{
		// TODO : Shadow

		return  fmat4(1.0f);
	}

	LightProxy::Light* PointLightProxy::GetLightData()
	{
		if (m_point)
		{
			return &m_point->data;
		}
		else
		{
			return nullptr;
		}
	}

	void PointLightProxy::SetRange(const float range)
	{
		if (!m_point)
		{
			return;
		}

		auto& light = m_point->data;
		if (std::abs(range - light.range) >= std::numeric_limits<float>::epsilon())
		{
			light.range = range;

			OnUpdate();
		}
	}

	SpotLightProxy::SpotLightProxy(const ECS::UUID& id)
		: LightProxy(id)
		, m_spot(Memory::MakeRef<ScalarData<SpotLight>>())
	{
	}

	bool SpotLightProxy::IsSyncable() const
	{
		return m_spot;
	}

	void SpotLightProxy::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_spot)
		{
			return;
		}

		auto& data = m_spot->data;
		data.direction = GetRotation() * BASE_DIR;
		data.pos = GetPosition();

		if (!m_lightBuffer)
		{
			RHIBufferCreateDesc desc;
			desc.bufferType = eBufferType::eStorage;
			desc.accessType = eDataAccess::eDynamic;
			desc.componentType = eDataType::eNone;
			desc.numComponents = 1;
			desc.count = 1;
			desc.size = sizeof(SpotLight);
			desc.stride = sizeof(SpotLight);
			desc.dataRanges.PushBack({ 0, m_spot });

			Memory::RefPtr<RHIBuffer> buffer = cmdList->CreateBuffer(desc);
			if (buffer)
			{
				m_lightBuffer = buffer;
			}
		}

		// TODO : Shadow
	}

	void SpotLightProxy::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_lightBuffer || !m_spot)
		{
			return;
		}

		auto& data = m_spot->data;
		data.direction = GetRotation() * BASE_DIR;
		data.pos = GetPosition();

		RHIBufferUpdateDesc desc;
		desc.bufferType = eBufferType::eStorage;
		desc.accessType = eDataAccess::eDynamic;
		desc.componentType = eDataType::eNone;
		desc.numComponents = 1;
		desc.count = 1;
		desc.size = sizeof(SpotLight);
		desc.stride = sizeof(SpotLight);
		desc.dataRanges.PushBack({ 0, m_spot });

		cmdList->UpdateBuffer(desc, m_lightBuffer);
	}

	const fmat4 SpotLightProxy::GetViewMatrix() const
	{
		// TODO : Shadow

		return fmat4(1.0f);
	}

	const fmat4 SpotLightProxy::GetProjectionMatrix() const
	{
		// TODO : Shadow

		return fmat4(1.0f);
	}

	LightProxy::Light* SpotLightProxy::GetLightData()
	{
		if (m_spot)
		{
			return &m_spot->data;
		}
		else
		{
			return nullptr;
		}
	}

	void SpotLightProxy::SetRange(const float range)
	{
		if (!m_spot)
		{
			return;
		}

		auto& lightData = m_spot->data;
		if (std::abs(range - lightData.range) >= std::numeric_limits<float>::epsilon())
		{
			lightData.range = range;

			OnUpdate();
		}
	}

	void SpotLightProxy::SetInnerAngle(const float innerAngle)
	{
		if (!m_spot)
		{
			return;
		}

		auto& lightData = m_spot->data;
		if (std::abs(innerAngle - lightData.innerAngle) >= std::numeric_limits<float>::epsilon())
		{
			lightData.innerAngle = innerAngle;

			OnUpdate();
		}
	}

	void SpotLightProxy::SetOuterAngle(const float outerAngle)
	{
		if (!m_spot)
		{
			return;
		}

		auto& lightData = m_spot->data;
		if (std::abs(outerAngle - lightData.outerAngle) >= std::numeric_limits<float>::epsilon())
		{
			lightData.outerAngle = outerAngle;

			OnUpdate();
		}
	}
}