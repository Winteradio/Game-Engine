#include <Renderer/Proxy/LightProxy.h>

#include <RHI/RHICommandList.h>
#include <RHI/RHIResources.h>
#include <Framework/Math/MathTypes.h>
#include <Memory/include/Core.h>

namespace wtr
{
	const fvec3 CUBE[] = {
		fvec3(-1.f, -1.f, -1.f), fvec3(1.f, -1.f, -1.f), fvec3(-1.f, 1.f, -1.f), fvec3(1.f, 1.f, -1.f),
		fvec3(-1.f, -1.f, 1.f), fvec3(1.f, -1.f, 1.f), fvec3(-1.f, 1.f, 1.f), fvec3(1.f, 1.f, 1.f) 
	};

	LightProxy::LightProxy(const ECS::UUID& id)
		: SceneProxy(id)
		, m_lightDesc()
		, m_lightView()
		, m_lightProj()
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

	Memory::RefPtr<const ScalarData<fmat4>> LightProxy::GetLightView() const
	{
		return m_lightView;
	}

	Memory::RefPtr<const ScalarData<fmat4>> LightProxy::GetLightProj() const
	{
		return m_lightProj;
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

		if (!m_shadowMap)
		{
			RHITextureCreateDesc desc;
			desc.width = 1;
			desc.height = 1;
			desc.depth = 1;
			desc.face = 1;
			desc.mipLevels = 1;
			desc.sampleCount = 1;
			desc.format = ePixelFormat::eD32;
			desc.usage = eTextureUsage::eDepthStencil;
			desc.textureType = eTextureType::eTexture2D;
			desc.dataType = eDataType::eFloat;
			desc.generateMips = false;
			desc.compressed = false;

			Memory::RefPtr<RHITexture> texture = cmdList->CreateTexture(desc);
			if (texture)
			{
				m_shadowMap = texture;
			}
		}

		if (!m_shadowTarget)
		{
			RHIRenderTargetCreateDesc desc;

			RHIDepthStencilAttachment depth;
			depth.type = eAttachment::eDepth;
			depth.texture = m_shadowMap;
			if (!m_shadowMap || m_shadowMap->GetState() != eResourceState::eReady)
			{
				return;
			}

			desc.depthStencil = depth;

			Memory::RefPtr<RHIRenderTarget> target = cmdList->CreateRenderTarget(desc);
			if (target)
			{
				m_shadowTarget = target;
			}
		}
	}

	void DirectionalLightProxy::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_lightBuffer && m_directional)
		{
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

		if (m_shadowMap)
		{
			const auto& prevDesc = m_shadowMap->GetDesc();

			RHITextureUpdateDesc desc = m_shadowMap->GetDesc();
			desc.width = 1;
			desc.height = 1;
			desc.depth = 1;
			desc.face = 1;
			desc.mipLevels = 1;
			desc.sampleCount = 1;
			desc.format = ePixelFormat::eD32;
			desc.usage = eTextureUsage::eDepthStencil;
			desc.textureType = eTextureType::eTexture2D;
			desc.dataType = eDataType::eFloat;
			desc.generateMips = false;
			desc.compressed = false;
		}
	}

	void DirectionalLightProxy::UpdateView(const RenderView& renderView)
	{
		const fvec3 lightDir = GetRotation() * BASE_DIR;
		const fvec3 lightPos = GetPosition();
	}

	const float DirectionalLightProxy::GetViewWidth() const
	{
		if (!m_lightProj)
		{
			return 1.f;
		}

		const float nearRatio = m_lightProj->data[0][0];
		if (std::abs(nearRatio) <= std::numeric_limits<float>::epsilon())
		{
			return 1.f;
		}

		const float width = 2.f / nearRatio;

		return width;
	}

	const float DirectionalLightProxy::GetViewHeight() const
	{
		if (!m_lightProj)
		{
			return 1.f;
		}

		const float nearRatio = m_lightProj->data[1][1];
		if (std::abs(nearRatio) <= std::numeric_limits<float>::epsilon())
		{
			return 1.f;
		}

		const float height = 2.f / nearRatio;

		return height;
	}

	const fvec2 DirectionalLightProxy::GetViewDepth() const
	{
		if (!m_lightProj)
		{
			return fvec2(0.0, 1.f);
		}

		const float zRatio = m_lightProj->data[2][2];
		const float wRatio = m_lightProj->data[3][2];
		if (std::abs(zRatio - wRatio) <= std::numeric_limits<float>::epsilon() || 
			std::abs(zRatio - 0.f) <= std::numeric_limits<float>::epsilon())
		{
			return fvec2(0.f, 1.f);
		}

		const fvec2 depthRange = fvec2((wRatio + 1.f) / zRatio, (wRatio - 1.f) / zRatio);

		return fvec2(0.f, 1.f);
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

	void PointLightProxy::UpdateView(const RenderView& renderView)
	{
		// TODO : Shadow
	}

	const float PointLightProxy::GetViewWidth() const
	{
		// TODO : View's near width
		return 0.f;
	}

	const float PointLightProxy::GetViewHeight() const
	{
		// TODO : View's near height
		return 0.f;
	}

	const fvec2 PointLightProxy::GetViewDepth() const
	{
		// TODO : Near, Far
		return fvec2(0.f);
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

	void SpotLightProxy::UpdateView(const RenderView& renderView)
	{
		// TODO : Shadow
	}

	const float SpotLightProxy::GetViewWidth() const
	{
		// TODO : View's near width
		return 0.f;
	}

	const float SpotLightProxy::GetViewHeight() const
	{
		// TODO : View's near height
		return 0.f;
	}

	const fvec2 SpotLightProxy::GetViewDepth() const
	{
		// TODO : Near, Far
		return fvec2(0.f);
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