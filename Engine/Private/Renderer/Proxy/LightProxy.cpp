#include <Renderer/Proxy/LightProxy.h>

#include <RHI/RHIResources.h>
#include <Renderer/ShaderSet.h>

namespace wtr
{
	LightProxy::LightProxy(const ECS::UUID& id)
		: SceneProxy(id)
		, m_lightDesc()
		, m_lightBuffer()
		, m_shadowMap()
		, m_shaderSet()
		, m_color(fvec3(1.0f))
		, m_direction(fvec3(0.f))
		, m_intensity(1.0f)
	{}

	eResourceState LightProxy::GetResourceState() const
	{
		if (!m_lightBuffer || m_lightBuffer->GetState() != eResourceState::eReady)
		{
			return eResourceState::eNone;
		}

		if (m_shadowMap && m_shadowMap->GetState() != eResourceState::eReady)
		{
			return eResourceState::eNone;
		}

		if (!m_shaderSet || m_shaderSet->GetResourceState() != eResourceState::eReady)
		{
			return eResourceState::eNone;
		}

		return eResourceState::eReady;
	}

	bool LightProxy::IsUploadable() const
	{
		// TODO
		return false;
	}

	bool LightProxy::IsSyncable() const
	{
		// TODO
		return false;
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

	Memory::RefPtr<ShaderSet> LightProxy::GetShaderSet()
	{
		return m_shaderSet;
	}

	Memory::RefPtr<const ShaderSet> LightProxy::GetShaderSet() const
	{
		return m_shaderSet;
	}

	const LightDesc& LightProxy::GetLightDesc() const
	{
		return m_lightDesc;
	}

	DirectionalLightProxy::DirectionalLightProxy(const ECS::UUID& id)
		: LightProxy(id)
	{
	}

	void DirectionalLightProxy::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		// TODO
	}

	void DirectionalLightProxy::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		// TODO
	}

	void DirectionalLightProxy::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		// TODO
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
		return fmat4(1.0f);

		// TODO
	}

	PointLightProxy::PointLightProxy(const ECS::UUID& id)
		: LightProxy(id)
		, m_range(0.f)
	{
	}

	void PointLightProxy::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		// TODO
	}

	void PointLightProxy::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		// TODO
	}

	void PointLightProxy::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		// TODO
	}

	const fmat4 PointLightProxy::GetViewMatrix() const
	{
		return fmat4(1.0f);

		// TODO
	}

	const fmat4 PointLightProxy::GetProjectionMatrix() const
	{
		return  fmat4(1.0f);

		// TODO
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

	void SpotLightProxy::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		// TODO
	}

	void SpotLightProxy::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		 // TODO
	}

	void SpotLightProxy::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
	}

	const fmat4 SpotLightProxy::GetViewMatrix() const
	{
		return fmat4(1.0f);

		// TODO
	}

	const fmat4 SpotLightProxy::GetProjectionMatrix() const
	{
		return fmat4(1.0f);

		// TODO
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