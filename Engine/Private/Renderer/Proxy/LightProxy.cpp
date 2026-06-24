#include <Renderer/Proxy/LightProxy.h>

#include <RHI/RHICommandList.h>
#include <RHI/RHIResources.h>
#include <Renderer/RenderView.h>
#include <Framework/Math/MathTypes.h>
#include <Memory/include/Core.h>
#include <Container/include/StaticArray.h>

namespace wtr
{
	constexpr size_t NUM_CUBE_VERTEX = 8;

	const StaticArray<fvec3, NUM_CUBE_VERTEX> NDC_CUBE = {
		fvec3(1.f, -1.f, -1.f), // Near Right Bottom
		fvec3(1.f, 1.f, -1.f), // Near Right Top
		fvec3(-1.f, 1.f, -1.f), // Near Left Top
		fvec3(-1.f, -1.f, -1.f), // Near Left Bottom
		fvec3(1.f, -1.f, 1.f), // Right Right Bottom
		fvec3(1.f, 1.f, 1.f), // Right Right Top
		fvec3(-1.f, 1.f, 1.f), // Right Left Top
		fvec3(-1.f, -1.f, 1.f) // Right Left Bottom
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
		data.direction = GetRotation() * util::GetForwardDirection();
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
			desc.width = 1024;
			desc.height = 1024;
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
			data.direction = GetRotation() * util::GetForwardDirection();
			data.pos = GetPosition();

			const RHIBufferDesc prevDesc = m_lightBuffer->GetDesc();

			RHIBufferUpdateDesc desc;
			desc = prevDesc;
			desc.dataRanges.PushBack({ 0, m_directional });

			cmdList->UpdateBuffer(desc, m_lightBuffer);
		}
	}

	void DirectionalLightProxy::UpdateView(const RenderView& renderView)
	{
		if (!m_lightProj)
		{
			m_lightProj = Memory::MakeRef<ScalarData<fmat4>>();
			m_lightProj->data = glm::fmat4(1.f);
		}

		if (!m_lightView)
		{
			m_lightView = Memory::MakeRef<ScalarData<fmat4>>();
			m_lightView->data = glm::fmat4(1.f);
		}

		if (!m_lightProj || !m_lightView ||
			glm::determinant(renderView.camera.viewMatrix) == 0 ||
			glm::determinant(renderView.camera.projMatrix) == 0)
		{
			return;
		}

		//m_lightView->data = util::GetLookAt(GetPosition(), GetRotation());
		//m_lightProj->data = util::GetOrthographicMatrix(20.f, 20.f, -7.5f, 7.5f);

		// Get the world camera' frustum position
		const fmat4 invCameraView = glm::inverse(renderView.camera.viewMatrix);
		const fmat4 invCameraProj = glm::inverse(renderView.camera.projMatrix);

		StaticArray<fvec3, NUM_CUBE_VERTEX> frustum = {};

		for (size_t index = 0; index < NDC_CUBE.Size(); index++)
		{
			const fvec3& ndcPos = NDC_CUBE[index];
			fvec4 viewPos = invCameraProj * fvec4(ndcPos, 1.f);
			
			if (std::abs(viewPos.w) > std::numeric_limits<float>::epsilon())
			{
				viewPos /= viewPos.w;
			}

			fvec4 worldPos = invCameraView * viewPos;

			frustum[index] = fvec3(worldPos);
		}

		// Get the shadow distance's direction in the world space
		fvec4 nearCenter = fvec4(0.f, 0.f, -1.f, 1.f);
		nearCenter = invCameraProj * nearCenter;
		nearCenter /= std::abs(nearCenter.w) > std::numeric_limits<float>::epsilon() ? nearCenter.w : 1.f;
		nearCenter = invCameraView * nearCenter;

		fvec4 farCenter = fvec4(0.f, 0.f, 1.f, 1.f);
		farCenter = invCameraProj * farCenter;
		farCenter /= std::abs(farCenter.w) > std::numeric_limits<float>::epsilon() ? farCenter.w : 1.f;
		farCenter = invCameraView * farCenter;

		const float frustumDepth = glm::length(fvec3(nearCenter) - fvec3(farCenter));
		const float shadowDistance = 10.f;

		const float depthLength = glm::length(farCenter - nearCenter);
		const float shadowRatio = std::abs(depthLength) > std::numeric_limits<float>::epsilon() ? shadowDistance / depthLength : 0.f;

		// Get the Shadow Frustum in the world space
		constexpr size_t pairIndex = 4;
		for (size_t index = 0; index < NDC_CUBE.Size() / 2; index++)
		{
			const fvec3& nearPos = frustum[index];
			const fvec3& farPos = frustum[index + pairIndex];

			const fvec3 shadowPos = nearPos + (farPos - nearPos) * shadowRatio;
			frustum[index + pairIndex] = shadowPos;
		}

		// Get the AABB on the light space
		const fmat4 lightRotation = glm::toMat4(GetRotation());
		const fmat4 invLightRotation = glm::transpose(lightRotation);
		const fmat4 invLightTranslation = glm::translate(fmat4(1.f), -1.f * GetPosition());

		const fmat4 lightView = invLightRotation * invLightTranslation;

		glm::fvec3 minPos(0.f);
		glm::fvec3 maxPos(0.f);

		for (size_t index = 0; index < frustum.Size(); index++)
		{
			fvec4 frustumVertex = fvec4(frustum[index], 1.f);
			frustumVertex = lightView * frustumVertex;

			frustum[index] = fvec3(frustumVertex);

			if (index == 0)
			{
				minPos = frustum[index];
				maxPos = frustum[index];
			}
			else
			{
				minPos.x = std::min(minPos.x, frustum[index].x);
				minPos.y = std::min(minPos.y, frustum[index].y);
				minPos.z = std::min(minPos.z, frustum[index].z);

				maxPos.x = std::max(maxPos.x, frustum[index].x);
				maxPos.y = std::max(maxPos.y, frustum[index].y);
				maxPos.z = std::max(maxPos.z, frustum[index].z);
			}
		}

		const fvec3 lightCenter = (minPos + maxPos) / 2.f;
		const float width = std::abs(maxPos.x - minPos.x);
		const float height = std::abs(maxPos.y - minPos.y);
		const float depth = std::abs(maxPos.z - minPos.z);

		const float frustumSize = std::max(width, std::max(height, depth));

		const float prevWidth = GetViewWidth();
		const float prevHeight = GetViewHeight();
		const fvec2 prevDepth = GetViewDepth();

		const fmat4 lightOffset = glm::translate(fmat4(1.f), -1.f * lightCenter);
		m_lightView->data = lightOffset * lightView;
		m_lightProj->data = util::GetOrthographicMatrix(frustumSize, frustumSize, -0.5f * frustumSize, 0.5f * frustumSize);

		OnUpdate();
	}

	float DirectionalLightProxy::GetViewWidth() const
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

	float DirectionalLightProxy::GetViewHeight() const
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

	fvec2 DirectionalLightProxy::GetViewDepth() const
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

		return depthRange;
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

		{
			const RHIBufferDesc prevDesc = m_lightBuffer->GetDesc();

			RHIBufferUpdateDesc desc;
			desc = prevDesc;
			desc.dataRanges.PushBack({ 0, m_point });

			cmdList->UpdateBuffer(desc, m_lightBuffer);
		}
	}

	void PointLightProxy::UpdateView(const RenderView& renderView)
	{
		// TODO : Shadow
	}

	float PointLightProxy::GetViewWidth() const
	{
		// TODO : View's near width
		return 0.f;
	}

	float PointLightProxy::GetViewHeight() const
	{
		// TODO : View's near height
		return 0.f;
	}

	fvec2 PointLightProxy::GetViewDepth() const
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
		data.direction = GetRotation() * util::GetForwardDirection();
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
		data.direction = GetRotation() * util::GetForwardDirection();
		data.pos = GetPosition();

		{
			const RHIBufferDesc prevDesc = m_lightBuffer->GetDesc();

			RHIBufferUpdateDesc desc;
			desc = prevDesc;
			desc.dataRanges.PushBack({ 0, m_spot });

			cmdList->UpdateBuffer(desc, m_lightBuffer);
		}

		{
			const RHITextureDesc prevDesc = m_shadowMap->GetDesc();

			RHITextureUpdateDesc desc;
			desc = prevDesc;
			desc.width = GetViewWidth();
			desc.height = GetViewHeight();

			cmdList->ResizeTexture(desc, m_shadowMap);
		}
	}

	void SpotLightProxy::UpdateView(const RenderView& renderView)
	{
		// TODO : Shadow
	}

	float SpotLightProxy::GetViewWidth() const
	{
		// TODO : View's near width
		return 0.f;
	}

	float SpotLightProxy::GetViewHeight() const
	{
		// TODO : View's near height
		return 0.f;
	}

	fvec2 SpotLightProxy::GetViewDepth() const
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