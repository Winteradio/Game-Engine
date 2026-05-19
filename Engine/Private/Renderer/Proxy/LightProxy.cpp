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
		, m_color(fvec3(1.0f))
		, m_direction(fvec3(0.f))
		, m_intensity(1.0f)
	{}

	eResourceState LightProxy::GetResourceState() const
	{
		eResourceState allState = ShaderProxy::GetResourceState();

		allState &= m_lightBuffer ? m_lightBuffer->GetState() : eResourceState::eNone;

		// TODO
		//allState &= m_shadowMap ? m_shadowMap->GetState() : eResourceState::eNone;
		//allState &= m_shadowTarget ? m_shadowTarget->GetState() : eResourceState::eNone;

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
			// GlobalShaderSelector를 통한 ShadowRenderTarget 변경
		}
	}

	void LightProxy::SetColor(const fvec3 color)
	{
		if ((std::abs(color.r - this->m_color.r) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(color.g - this->m_color.g) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(color.b - this->m_color.b) >= std::numeric_limits<float>::epsilon()))
		{
			this->m_color = color;

			OnUpdate();
		}
	}

	void LightProxy::SetDirection(const fvec3 direction)
	{
		if ((std::abs(direction.r - this->m_direction.x) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(direction.g - this->m_direction.y) >= std::numeric_limits<float>::epsilon()) ||
			(std::abs(direction.b - this->m_direction.z) >= std::numeric_limits<float>::epsilon()))
		{
			this->m_direction = direction;

			OnUpdate();
		}
	}

	void LightProxy::SetIntensity(const float intensity)
	{
		if (std::abs(intensity - this->m_intensity) >= std::numeric_limits<float>::epsilon())
		{
			this->m_intensity = intensity;

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
	{
	}

	bool DirectionalLightProxy::IsSyncable() const
	{
		return m_directional && !m_directional->IsUsed();
	}

	void DirectionalLightProxy::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (!m_directional)
		{
			m_directional = Memory::MakeRef<ScalarData<DirectionalLight>>();
		}

		auto& data = m_directional->data;
		data.color = m_color;
		data.direction = m_direction;
		data.intensity = m_intensity;
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

	PointLightProxy::PointLightProxy(const ECS::UUID& id)
		: LightProxy(id)
		, m_range(0.f)
	{
	}

	bool PointLightProxy::IsSyncable() const
	{
		return m_point && !m_point->IsUsed();
	}

	void PointLightProxy::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (!m_point)
		{
			m_point = Memory::MakeRef<ScalarData<PointLight>>();
		}

		auto& data = m_point->data;
		data.color = m_color;
		data.direction = m_direction;
		data.intensity = m_intensity;
		data.range = m_range;
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

	void PointLightProxy::SetRange(const float range)
	{
		if (std::abs(range - this->m_range) >= std::numeric_limits<float>::epsilon())
		{
			this->m_range = range;

			OnUpdate();
		}
	}

	SpotLightProxy::SpotLightProxy(const ECS::UUID& id)
		: LightProxy(id)
		, m_range(0.f)
		, m_innerAngle(30.f)
		, m_outerAngle(90.f)
	{
	}

	bool SpotLightProxy::IsSyncable() const
	{
		return m_spot && !m_spot->IsUsed();
	}

	void SpotLightProxy::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (!m_spot)
		{
			m_spot = Memory::MakeRef<ScalarData<SpotLight>>();
		}

		auto& data = m_spot->data;
		data.color = m_color;
		data.direction = m_direction;
		data.intensity = m_intensity;
		data.range = m_range;
		data.innerAngle = m_innerAngle;
		data.outerAngle = m_outerAngle;
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

	void SpotLightProxy::SetRange(const float range)
	{
		if (std::abs(range - this->m_range) >= std::numeric_limits<float>::epsilon())
		{
			this->m_range = range;

			OnUpdate();
		}
	}

	void SpotLightProxy::SetInnerAngle(const float innerAngle)
	{
		if (std::abs(innerAngle - this->m_innerAngle) >= std::numeric_limits<float>::epsilon())
		{
			this->m_innerAngle = innerAngle;

			OnUpdate();
		}
	}

	void SpotLightProxy::SetOuterAngle(const float outerAngle)
	{
		if (std::abs(outerAngle - this->m_outerAngle) >= std::numeric_limits<float>::epsilon())
		{
			this->m_outerAngle = outerAngle;

			OnUpdate();
		}
	}
}