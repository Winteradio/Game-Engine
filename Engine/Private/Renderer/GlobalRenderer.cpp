#include <Renderer/GlobalRenderer.h>

#include <Asset/AssetSystem.h>
#include <Asset/AssetTypes.h>
#include <Renderer/RenderView.h>
#include <Renderer/RenderResource.h>
#include <Renderer/Proxy/LightProxy.h>
#include <Renderer/Proxy/MaterialProxy.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIDescriptions.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	namespace GlobalResource
	{
		CameraResource::CameraResource()
			: m_cameraData()
			, m_buffer()
		{
			m_cameraData = Memory::MakeRef<ScalarData<CameraData>>();
		}

		eResourceState CameraResource::GetResourceState() const
		{
			return m_buffer ? m_buffer->GetState() : eResourceState::eNone;
		}

		void CameraResource::Upload(Memory::RefPtr<RHICommandList> cmdList)
		{
			if (!cmdList || !m_cameraData)
			{
				return;
			}

			RHIBufferCreateDesc desc;
			desc.bufferType = eBufferType::eConst;
			desc.accessType = eDataAccess::eDynamic;
			desc.componentType = eDataType::eNone;
			desc.numComponents = 0;
			desc.count = 1;
			desc.stride = sizeof(CameraResource::CameraData);
			desc.size = sizeof(CameraResource::CameraData);
			desc.dataRanges.PushBack({ 0, m_cameraData });

			Memory::RefPtr<RHIBuffer> cameraBuffer = cmdList->CreateBuffer(desc);
			if (!cameraBuffer)
			{
				LOGWARN() << "[CameraResource] Failed to create the camera's rhi buffer";
				return;
			}
			else
			{
				LOGINFO() << "[CameraResource] Succeed to create the camera's rhi buffer";

				m_buffer = cameraBuffer;
			}
		}

		void CameraResource::Unload(Memory::RefPtr<RHICommandList> cmdList)
		{
			if (!cmdList)
			{
				return;
			}

			cmdList->RemoveBuffer(m_buffer);

			m_buffer = nullptr;
			m_cameraData = nullptr;

			LOGINFO() << "[CameraResource] Unload the camera's rhi buffer";
		}

		void CameraResource::Sync(Memory::RefPtr<RHICommandList> cmdList)
		{
			if (!cmdList || !m_cameraData)
			{
				return;
			}

			Memory::RefPtr<ScalarData<CameraData>> updateData = Memory::MakeRef<ScalarData<CameraData>>();
			if (!updateData)
			{
				return;
			}

			updateData->data = m_cameraData->data;

			RHIBufferUpdateDesc updateDesc;
			updateDesc.bufferType = eBufferType::eConst;
			updateDesc.accessType = eDataAccess::eDynamic;
			updateDesc.componentType = eDataType::eNone;
			updateDesc.numComponents = 0;
			updateDesc.count = 1;
			updateDesc.stride = sizeof(CameraResource::CameraData);
			updateDesc.size = sizeof(CameraResource::CameraData);
			updateDesc.dataRanges.PushBack({ 0, updateData });
			updateDesc.mapAccess = eMapAccess::eNone;

			cmdList->UpdateBuffer(updateDesc, m_buffer);
		}

		Memory::RefPtr<const RHIBuffer> CameraResource::GetBuffer() const
		{
			return m_buffer;
		}

		void CameraResource::UpdateView(const fmat4 viewMatrix)
		{
			if (!m_cameraData)
			{
				return;
			}

			auto& cameraData = m_cameraData->data;
			cameraData.viewMatrix = viewMatrix;
		}

		void CameraResource::UpdateProjection(const fmat4 projMatrix)
		{
			if (!m_cameraData)
			{
				return;
			}

			auto& cameraData = m_cameraData->data;
			cameraData.projMatrix = projMatrix;
		}

		void CameraResource::UpdatePosition(const fvec3 position)
		{
			if (!m_cameraData)
			{
				return;
			}

			auto& cameraData = m_cameraData->data;
			cameraData.position = position;
		}

		void CameraResource::UpdateDirection(const fvec3 direction)
		{
			if (!m_cameraData)
			{
				return;
			}

			auto& cameraData = m_cameraData->data;
			cameraData.direction = direction;
		}

		GBufferResource::GBufferResource(const RHITextureDesc& desc)
			: m_texture(nullptr)
			, m_prevDesc(nullptr)
		{
			m_prevDesc = Memory::MakeRef<RHITextureDesc>();
			
			if (m_prevDesc)
			{
				*m_prevDesc = desc;
			}
		}

		eResourceState GBufferResource::GetResourceState() const
		{
			return m_texture ? m_texture->GetState() : eResourceState::eNone;
		}

		void GBufferResource::Upload(Memory::RefPtr<RHICommandList> cmdList)
		{
			if (!cmdList || m_texture || !m_prevDesc)
			{
				return;
			}

			RHITextureCreateDesc desc;
			desc.width = m_prevDesc->width;
			desc.height = m_prevDesc->height;
			desc.depth = m_prevDesc->depth;
			desc.face = m_prevDesc->face;
			desc.mipLevels = m_prevDesc->mipLevels;
			desc.sampleCount = m_prevDesc->sampleCount;
			desc.format = m_prevDesc->format;
			desc.usage = m_prevDesc->usage;
			desc.textureType = m_prevDesc->textureType;
			desc.dataType = m_prevDesc->dataType;
			desc.generateMips = m_prevDesc->generateMips;
			desc.compressed = m_prevDesc->compressed;

			Memory::RefPtr<RHITexture> texture = cmdList->CreateTexture(desc);
			if (!texture)
			{
				LOGWARN() << "[GBuffer] Failed to create the texture's rhi buffer";
			}
			else
			{
				LOGINFO() << "[GBuffer] Succeed to create the texture's rhi buffer";

				m_texture = texture;
			}
		}

		void GBufferResource::Unload(Memory::RefPtr<RHICommandList> cmdList)
		{
			if (!cmdList || !m_texture)
			{
				return;
			}

			cmdList->RemoveTexture(m_texture);
			m_texture.Reset();

			LOGINFO() << "[GBuffer] Unload the texture's rhi buffer";
		}

		void GBufferResource::Sync(Memory::RefPtr<RHICommandList> cmdList)
		{
			if (!cmdList || !m_texture)
			{
				return;
			}

			if (m_prevDesc->width == m_texture->GetWidth() &&
				m_prevDesc->height == m_texture->GetHeight() &&
				m_prevDesc->depth == m_texture->GetDepth())
			{
				return;
			}

			RHITextureCreateDesc desc;
			desc.width = m_prevDesc->width;
			desc.height = m_prevDesc->height;
			desc.depth = m_prevDesc->depth;
			desc.face = m_prevDesc->face;
			desc.mipLevels = m_prevDesc->mipLevels;
			desc.sampleCount = m_prevDesc->sampleCount;
			desc.format = m_prevDesc->format;
			desc.usage = m_prevDesc->usage;
			desc.textureType = m_prevDesc->textureType;
			desc.dataType = m_prevDesc->dataType;
			desc.generateMips = m_prevDesc->generateMips;
			desc.compressed = m_prevDesc->compressed;

			cmdList->ResizeTexture(desc, m_texture);
		}

		Memory::RefPtr<const RHITexture> GBufferResource::GetTexture() const
		{
			return m_texture;
		}

		void GBufferResource::Resize(const uint32_t width, const uint32_t height, const uint32_t depth)
		{
			if (!m_prevDesc)
			{
				return;
			}

			m_prevDesc->width = width;
			m_prevDesc->height = height;
			m_prevDesc->depth = depth;
		}

		QuadResource::QuadResource()
			: m_position()
			, m_uv()
			, m_index()
			, m_layout()
		{
		}

		eResourceState QuadResource::GetResourceState() const
		{
			if (!m_position)
			{
				return eResourceState::eNone;
			}
			
			if (!m_uv)
			{
				return eResourceState::eNone;
			}

			if (!m_index)
			{
				return eResourceState::eNone;
			}

			if (!m_layout)
			{
				return eResourceState::eNone;
			}

			return m_position->GetState() & m_uv->GetState() & m_index->GetState() & m_layout->GetState();
		}

		void QuadResource::Upload(Memory::RefPtr<RHICommandList> cmdList)
		{
			if (!cmdList)
			{
				return;
			}

			constexpr size_t vertexCount = 4;
			constexpr size_t indexCount = 6;

			Memory::RefPtr<ArrayData<fvec2>> positionData = Memory::MakeRef<ArrayData<fvec2>>();
			Memory::RefPtr<ArrayData<fvec2>> uvData = Memory::MakeRef<ArrayData<fvec2>>();
			Memory::RefPtr<ArrayData<uint32_t>> indexData = Memory::MakeRef<ArrayData<uint32_t>>();
			if (!positionData || !uvData || !indexData)
			{
				return;
			}

			positionData->data.Resize(vertexCount);
			uvData->data.Resize(vertexCount);
			indexData->data.Resize(indexCount);

			positionData->data[0] = fvec2(-1.0, -1.0);
			positionData->data[1] = fvec2(1.0, -1.0);
			positionData->data[2] = fvec2(1.0, 1.0);
			positionData->data[3] = fvec2(-1.0, 1.0);

			uvData->data[0] = fvec2(0.0, 0.0);
			uvData->data[1] = fvec2(1.0, 0.0);
			uvData->data[2] = fvec2(1.0, 1.0);
			uvData->data[3] = fvec2(0.0, 1.0);

			indexData->data[0] = 0;
			indexData->data[1] = 1;
			indexData->data[2] = 2;
			indexData->data[3] = 0;
			indexData->data[4] = 2;
			indexData->data[5] = 3;

			RHIBufferCreateDesc positionDesc;
			positionDesc.bufferType = eBufferType::eVertex;
			positionDesc.accessType = eDataAccess::eStatic;
			positionDesc.componentType = eDataType::eFloat;
			positionDesc.numComponents = 2;
			positionDesc.count = vertexCount;
			positionDesc.stride = sizeof(fvec2);
			positionDesc.size = sizeof(fvec2) * vertexCount;
			positionDesc.dataRanges.PushBack({0, positionData});

			RHIBufferCreateDesc uvDesc;
			uvDesc.bufferType = eBufferType::eVertex;
			uvDesc.accessType = eDataAccess::eStatic;
			uvDesc.componentType = eDataType::eFloat;
			uvDesc.numComponents = 2;
			uvDesc.count = vertexCount;
			uvDesc.stride = sizeof(fvec2);
			uvDesc.size = sizeof(fvec2) * vertexCount;
			uvDesc.dataRanges.PushBack({ 0, uvData });

			RHIBufferCreateDesc indexDesc;
			indexDesc.bufferType = eBufferType::eIndex;
			indexDesc.accessType = eDataAccess::eStatic;
			indexDesc.componentType = eDataType::eUInt;
			indexDesc.numComponents = 1;
			indexDesc.count = indexCount;
			indexDesc.stride = sizeof(uint32_t);
			indexDesc.size = sizeof(uint32_t) * indexCount;
			indexDesc.dataRanges.PushBack({ 0, indexData });

			Memory::RefPtr<RHIBuffer> positionBuffer = cmdList->CreateBuffer(positionDesc);
			Memory::RefPtr<RHIBuffer> uvBuffer = cmdList->CreateBuffer(uvDesc);
			Memory::RefPtr<RHIBuffer> indexBuffer = cmdList->CreateBuffer(indexDesc);
			if (!positionBuffer || !uvBuffer || !indexBuffer)
			{
				return;
			}

			RHIVertexLayoutCreateDesc layoutDesc;

			VertexKey positionKey{ eVertexSemantic::ePosition, 0 };
			RHIVertexStream positionStream;
			positionStream.buffer = positionBuffer;
			positionStream.attribute.semantic = eVertexSemantic::ePosition;
			positionStream.attribute.semanticIndex = 0;
			positionStream.attribute.componentType = eDataType::eFloat;
			positionStream.attribute.numComponents = 2;
			positionStream.attribute.normalized = false;
			positionStream.attribute.integer = false;
			positionStream.attribute.divisor = 0;
			positionStream.attribute.offset = 0;

			VertexKey uvKey{ eVertexSemantic::eTexCoord, 0 };
			RHIVertexStream uvStream;
			uvStream.buffer = uvBuffer;
			uvStream.attribute.semantic = eVertexSemantic::eTexCoord;
			uvStream.attribute.semanticIndex = 0;
			uvStream.attribute.componentType = eDataType::eFloat;
			uvStream.attribute.numComponents = 2;
			uvStream.attribute.normalized = false;
			uvStream.attribute.integer = false;
			uvStream.attribute.divisor = 0;
			uvStream.attribute.offset = 0;

			layoutDesc.vertexStreams.Insert(std::make_pair(positionKey, positionStream));
			layoutDesc.vertexStreams.Insert(std::make_pair(uvKey, uvStream));
			layoutDesc.indexBuffer = indexBuffer;

			Memory::RefPtr<RHIVertexLayout> vertexLayout = cmdList->CreateVertexLayout(layoutDesc);
			if (!vertexLayout)
			{
				return;
			}

			m_position = positionBuffer;
			m_uv = uvBuffer;
			m_index = indexBuffer;
			m_layout = vertexLayout;
		}

		void QuadResource::Unload(Memory::RefPtr<RHICommandList> cmdList)
		{
			if (!cmdList)
			{
				return;
			}

			if (m_position)
			{
				cmdList->RemoveBuffer(m_position);
				m_position.Reset();
			}

			if (m_uv)
			{
				cmdList->RemoveBuffer(m_uv);
				m_uv.Reset();
			}

			if (m_index)
			{
				cmdList->RemoveBuffer(m_index);
				m_index.Reset();
			}

			if (m_layout)
			{
				cmdList->RemoveVertexLayout(m_layout);
				m_layout.Reset();
			}
		}

		Memory::RefPtr<const RHIVertexLayout> QuadResource::GetLayout() const
		{
			return m_layout;
		}
	}

	namespace GlobalResource
	{
		struct ResourceData
		{
			struct PipeLineHasher
			{
				size_t operator()(const RHIPipeLineCreateDesc& desc)
				{
					size_t hash = 0;
				
					auto HashCombine = [](size_t& seed, const void* pointer)
						{
							seed ^= std::hash<const void*>{}(pointer)+0x9e3779b9 + (seed << 6) + (seed >> 2);

							return seed;
						};

					HashCombine(hash, desc.clear.Get());
					HashCombine(hash, desc.color.Get());
					HashCombine(hash, desc.depth.Get());
					HashCombine(hash, desc.stencil.Get());
					HashCombine(hash, desc.blend.Get());
					HashCombine(hash, desc.rasterizer.Get());
					HashCombine(hash, desc.vertexShader.Get());
					HashCombine(hash, desc.geometryShader.Get());
					HashCombine(hash, desc.hullShader.Get());
					HashCombine(hash, desc.domainShader.Get());
					HashCombine(hash, desc.computeShader.Get());
					HashCombine(hash, desc.pixelShader.Get());

					return hash;
				}
			};

			struct SamplerHasher
			{
				size_t operator()(const RHISamplerCreateDesc& desc)
				{
					size_t hash = 0;
					hash = (hash << 2) | static_cast<size_t>(desc.minFilter);
					hash = (hash << 2) | static_cast<size_t>(desc.magFilter);
					hash = (hash << 2) | static_cast<size_t>(desc.mipFilter);
					hash = (hash << 3) | static_cast<size_t>(desc.wrapS);
					hash = (hash << 3) | static_cast<size_t>(desc.wrapR);
					hash = (hash << 3) | static_cast<size_t>(desc.wrapT);

					return hash;
				}
			};

			using GBufferContainer = wtr::HashMap<eGBufferSlot, Memory::RefPtr<GBufferResource>>;
			using PipeLineContainer = wtr::HashMap<RHIPipeLineCreateDesc, Memory::RefPtr<RHIPipeLine>, PipeLineHasher>;
			using SamplerContainer = wtr::HashMap<RHISamplerCreateDesc, Memory::RefPtr<RHISampler>, SamplerHasher>;

			CameraResource camera;
			QuadResource quad;

			GBufferContainer gBuffers;
			PipeLineContainer pipelines;
			SamplerContainer samplers;

			Memory::RefPtr<MaterialProxy> material;
			Memory::RefPtr<LightProxy> light;
		};

		ResourceData& GetData()
		{
			static ResourceData data;
			return data;
		}

		bool InitGBuffer(Memory::RefPtr<RHICommandList> cmdList)
		{
			if (!cmdList)
			{
				return false;
			}

			auto& data = GetData();

			for (uint8_t slotIndex = static_cast<uint8_t>(eGBufferSlot::ePosition); slotIndex <= static_cast<uint8_t>(eGBufferSlot::eDepth); slotIndex++)
			{
				eGBufferSlot slot = static_cast<eGBufferSlot>(slotIndex);

				RHITextureDesc desc;
				if (slot == eGBufferSlot::ePosition)
				{
					desc.width = 1;
					desc.height = 1;
					desc.depth = 1;
					desc.face = 1;
					desc.mipLevels = 1;
					desc.sampleCount = 1;
					desc.format = ePixelFormat::eR32G32B32A32_Float;
					desc.usage = eTextureUsage::eRenderTarget;
					desc.textureType = eTextureType::eTexture2D;
					desc.dataType = eDataType::eFloat;
					desc.generateMips = false;
					desc.compressed = false;
				}
				else if (slot == eGBufferSlot::eNormal)
				{
					desc.width = 1;
					desc.height = 1;
					desc.depth = 1;
					desc.face = 1;
					desc.mipLevels = 1;
					desc.sampleCount = 1;
					desc.format = ePixelFormat::eR16G16B16A16_Float;
					desc.usage = eTextureUsage::eRenderTarget;
					desc.textureType = eTextureType::eTexture2D;
					desc.dataType = eDataType::eFloat;
					desc.generateMips = false;
					desc.compressed = false;
				}
				else if (slot == eGBufferSlot::eAlbedo)
				{
					desc.width = 1;
					desc.height = 1;
					desc.depth = 1;
					desc.face = 1;
					desc.mipLevels = 1;
					desc.sampleCount = 1;
					desc.format = ePixelFormat::eR8G8B8A8_UNorm;
					desc.usage = eTextureUsage::eRenderTarget;
					desc.textureType = eTextureType::eTexture2D;
					desc.dataType = eDataType::eFloat;
					desc.generateMips = false;
					desc.compressed = false;
				}
				else if (slot == eGBufferSlot::eAlpha)
				{
					desc.width = 1;
					desc.height = 1;
					desc.depth = 1;
					desc.face = 1;
					desc.mipLevels = 1;
					desc.sampleCount = 1;
					desc.format = ePixelFormat::eR8G8B8A8_UNorm;
					desc.usage = eTextureUsage::eRenderTarget;
					desc.textureType = eTextureType::eTexture2D;
					desc.dataType = eDataType::eUByte;
					desc.generateMips = false;
					desc.compressed = false;
				}
				else if (slot == eGBufferSlot::eDepth)
				{
					desc.width = 1;
					desc.height = 1;
					desc.depth = 1;
					desc.face = 1;
					desc.mipLevels = 1;
					desc.sampleCount = 1;
					desc.format = ePixelFormat::eD24_S8;
					desc.usage = eTextureUsage::eDepthStencil;
					desc.textureType = eTextureType::eTexture2D;
					desc.dataType = eDataType::eFloat;
					desc.generateMips = false;
					desc.compressed = false;
				}
				else
				{
					LOGWARN() << "[GlobalResource] Failed to create the GBuffer, the slot(" << static_cast<size_t>(slot) << "isn't supported";
					continue;
				}

				Memory::RefPtr<GBufferResource> gBuffer = Memory::MakeRef<GBufferResource>(desc);
				if (gBuffer)
				{
					gBuffer->Upload(cmdList);

					data.gBuffers[slot] = gBuffer;
				}
			}

			return !data.gBuffers.Empty();
		}

		bool Init(Memory::RefPtr<RHICommandList> cmdList)
		{
			auto& data = GetData();
			data.camera.Upload(cmdList);
			data.quad.Upload(cmdList);

			Memory::RefPtr<MaterialAsset> defaultMaterial = Memory::Cast<MaterialAsset>(AssetSystem::Create("default", eAsset::eMaterial));
			data.material = Memory::MakeRef<MaterialProxy>(ECS::UUID::Null());
			data.material->Upload(cmdList);
			data.material->SetMaterialAsset(defaultMaterial);
			GlobalShaderSelector::SetShader(data.material);

			data.light = Memory::MakeRef<DirectionalLightProxy>(ECS::UUID::Null());
			data.light->Upload(cmdList);
			GlobalShaderSelector::SetShader(data.light);

			return InitGBuffer(cmdList);
		}

		void Release(Memory::RefPtr<RHICommandList> cmdList)
		{
			auto& data = GetData();
			data.camera.Unload(cmdList);
			data.quad.Unload(cmdList);
			data.material->Unload(cmdList);
			data.light->Unload(cmdList);

			for (auto& [slot, gBuffer] : data.gBuffers)
			{
				if (gBuffer)
				{
					gBuffer->Unload(cmdList);
					gBuffer.Reset();
				}
			}

			for (auto& [desc, pipeline] : data.pipelines)
			{
				if (pipeline)
				{
					cmdList->RemovePipeLine(pipeline);
					pipeline.Reset();
				}
			}

			for (auto& [desc, sampler] : data.samplers)
			{
				if (sampler)
				{
					cmdList->RemoveSampler(sampler);
					sampler.Reset();
				}
			}
		}

		void Update(Memory::RefPtr<RHICommandList> cmdList, const RenderView& renderView)
		{
			if (!cmdList)
			{
				return;
			}

			auto& data = GetData();
			{
				if (data.camera.GetResourceState() != eResourceState::eReady)
				{
					return;
				}

				data.camera.UpdateDirection(renderView.camera.direction);
				data.camera.UpdatePosition(renderView.camera.position);
				data.camera.UpdateView(renderView.camera.viewMatrix);
				data.camera.UpdateProjection(renderView.camera.projMatrix);

				data.camera.Sync(cmdList);
			}

			{
				for (auto& [slot, gBuffer] : data.gBuffers)
				{
					if (gBuffer)
					{
						gBuffer->Resize(renderView.viewport.width, renderView.viewport.height);
						gBuffer->Sync(cmdList);
					}
				}
			}
		}

		Memory::RefPtr<const RHIBuffer> GetCamera()
		{
			auto& data = GetData();
			if (data.camera.GetResourceState() != eResourceState::eReady)
			{
				return {};
			}

			return data.camera.GetBuffer();
		}

		Memory::RefPtr<const RHITexture> GetGBuffer(const eGBufferSlot slot)
		{
			if (slot == eGBufferSlot::eNone)
			{
				return {};
			}

			auto& data = GetData();
			auto& gBuffer = data.gBuffers[slot];

			if (gBuffer && gBuffer->GetResourceState() == eResourceState::eReady)
			{
				return gBuffer->GetTexture();
			}
			else
			{
				return {};
			}
		}

		Memory::RefPtr<const RHIVertexLayout> GetQuad()
		{
			auto& data = GetData();
			if (data.quad.GetResourceState() == eResourceState::eReady)
			{
				return data.quad.GetLayout();
			}
			else
			{
				return {};
			}
		}

		Memory::RefPtr<const RHIPipeLine> GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, const RHIPipeLineCreateDesc desc)
		{
			if (!cmdList)
			{
				return {};
			}

			if (desc.vertexShader && desc.vertexShader->GetState() != eResourceState::eReady)
			{
				return {};
			}

			if (desc.geometryShader && desc.geometryShader->GetState() != eResourceState::eReady)
			{
				return {};
			}

			if (desc.hullShader && desc.hullShader->GetState() != eResourceState::eReady)
			{
				return {};
			}

			if (desc.domainShader && desc.domainShader->GetState() != eResourceState::eReady)
			{
				return {};
			}

			if (desc.computeShader && desc.computeShader->GetState() != eResourceState::eReady)
			{
				return {};
			}

			if (desc.pixelShader && desc.pixelShader->GetState() != eResourceState::eReady)
			{
				return {};
			}

			auto& data = GetData();
			Memory::RefPtr<RHIPipeLine>& pipeline = data.pipelines[desc];
			if (!pipeline)
			{
				pipeline = cmdList->CreatePipeLine(desc);
			}

			if (pipeline && pipeline->GetState() == eResourceState::eReady)
			{
				return pipeline;
			}
			else
			{
				return {};
			}
		}

		Memory::RefPtr<const RHISampler> GetSampler(Memory::RefPtr<RHICommandList> cmdList, const eResourceSlot slot)
		{
			if (!cmdList)
			{
				return {};
			}

			if (slot == eResourceSlot::eVector ||
				slot == eResourceSlot::eScalar ||
				slot == eResourceSlot::eNone)
			{
				return nullptr;
			}

			RHISamplerCreateDesc desc;
			desc.minFilter = eFilterMode::eLinear;
			desc.magFilter = eFilterMode::eLinear;
			desc.mipFilter = eFilterMode::eLinear;

			switch (slot)
			{
			case eResourceSlot::eOpacity:
				desc.wrapS = eWrapMode::eClampToEdge;
				desc.wrapT = eWrapMode::eClampToEdge;
				desc.wrapR = eWrapMode::eClampToEdge;
				break;

			default:
				desc.wrapS = eWrapMode::eRepeat;
				desc.wrapT = eWrapMode::eRepeat;
				desc.wrapR = eWrapMode::eRepeat;
				break;
			}

			return GetSampler(cmdList, desc);
		}

		Memory::RefPtr<const RHISampler> GetSampler(Memory::RefPtr<RHICommandList> cmdList, const RHISamplerCreateDesc desc)
		{
			if (!cmdList)
			{
				return {};
			}

			auto& data = GetData();
			Memory::RefPtr<RHISampler>& sampler = data.samplers[desc];
			if (!sampler)
			{
				sampler = cmdList->CreateSampler(desc);
			}

			if (sampler && sampler->GetState() == eResourceState::eReady)
			{
				return sampler;
			}
			else
			{
				return {};
			}
		}

		Memory::RefPtr<const MaterialProxy> GetDefaultMaterial()
		{
			auto& data = GetData();
			
			return data.material;
		}

		Memory::RefPtr<const LightProxy> GetDefaultLight()
		{
			auto& data = GetData();

			return data.light;
		}
	}

	namespace GlobalShaderSelector
	{
		void SetShader(Memory::RefPtr<MaterialProxy> material)
		{
			if (!material)
			{
				return;
			}

			const auto& materialDesc = material->GetMaterialDesc();

			Memory::RefPtr<const ShaderAsset> geometryVS;
			Memory::RefPtr<const ShaderAsset> geometryGS;
			Memory::RefPtr<const ShaderAsset> geometryHS;
			Memory::RefPtr<const ShaderAsset> geometryDS;
			Memory::RefPtr<const ShaderAsset> geometryPS;

			if (materialDesc.shadingModel == eShadingModel::eUnlit)
			{
				if (materialDesc.hasDiffuseMap)
				{
					geometryVS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/unlit_texture.vs.glsl"));
					geometryPS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/unlit_texture.ps.glsl"));
				}
				else
				{
					geometryVS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/unlit_color.vs.glsl"));
					geometryPS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/unlit_color.ps.glsl"));
				}
			}
			else if (materialDesc.shadingModel == eShadingModel::eLit)
			{
				if (materialDesc.blendMode == eBlendMode::eOpaque)
				{
					if (materialDesc.isPBR)
					{
						geometryVS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/geometry_pbr.vs.glsl"));
						geometryPS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/geometry_pbr.ps.glsl"));
					}
					else
					{
						geometryVS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/geometry_phong.vs.glsl"));
						geometryPS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/geometry_phong.ps.glsl"));
					}
				}

				// TODO : eTranslucent (Forward)
			}

			// TODO : Height Map

			if (geometryVS) material->AddShader(Memory::MakeRef<GeometryVSState>(geometryVS));
			if (geometryGS) material->AddShader(Memory::MakeRef<GeometryGSState>(geometryGS));
			if (geometryHS) material->AddShader(Memory::MakeRef<GeometryHSState>(geometryHS));
			if (geometryDS) material->AddShader(Memory::MakeRef<GeometryDSState>(geometryDS));
			if (geometryPS) material->AddShader(Memory::MakeRef<GeometryPSState>(geometryPS));

			//Memory::RefPtr<const ShaderAsset> computeCS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/frustum_culling.cs.glsl"));
			//if (computeCS) material->AddShader(Memory::MakeRef<ComputeCSState>(computeCS));

		}

		void SetShader(Memory::RefPtr<LightProxy> light)
		{
			if (!light)
			{
				return;
			}

			if (auto directional = Memory::Cast<DirectionalLightProxy>(light))
			{
				Memory::RefPtr<const ShaderAsset> shadowVS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/shadow_directional.vs.glsl"));
				Memory::RefPtr<const ShaderAsset> shadowPS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/shadow_directional.ps.glsl"));

				Memory::RefPtr<const ShaderAsset> lightVS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/lighting_directional.vs.glsl"));
				Memory::RefPtr<const ShaderAsset> lightPS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/lighting_directional.ps.glsl"));

				if (shadowVS) light->AddShader(Memory::MakeRef<ShadowVSState>(shadowVS));
				if (shadowPS) light->AddShader(Memory::MakeRef<ShadowPSState>(shadowPS));
				if (lightVS)  light->AddShader(Memory::MakeRef<LightVSState>(lightVS));
				if (lightPS)  light->AddShader(Memory::MakeRef<LightPSState>(lightPS));

			}
			else if (auto spot = Memory::Cast<SpotLightProxy>(light))
			{
				Memory::RefPtr<const ShaderAsset> shadowVS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/shadow_spot.vs.glsl"));
				Memory::RefPtr<const ShaderAsset> shadowPS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/shadow_spot.ps.glsl"));

				Memory::RefPtr<const ShaderAsset> lightVS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/lighting_spot.vs.glsl"));
				Memory::RefPtr<const ShaderAsset> lightPS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/lighting_spot.ps.glsl"));

				if (shadowVS) light->AddShader(Memory::MakeRef<ShadowVSState>(shadowVS));
				if (shadowPS) light->AddShader(Memory::MakeRef<ShadowPSState>(shadowPS));
				if (lightVS)  light->AddShader(Memory::MakeRef<LightVSState>(lightVS));
				if (lightPS)  light->AddShader(Memory::MakeRef<LightPSState>(lightPS));

			}
			else if (auto point = Memory::Cast<PointLightProxy>(light))
			{
				Memory::RefPtr<const ShaderAsset> shadowVS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/shadow_point.vs.glsl"));
				Memory::RefPtr<const ShaderAsset> shadowGS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/shadow_point.gs.glsl"));
				Memory::RefPtr<const ShaderAsset> shadowPS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/shadow_point.ps.glsl"));

				Memory::RefPtr<const ShaderAsset> lightVS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/lighting_point.vs.glsl"));
				Memory::RefPtr<const ShaderAsset> lightPS = Memory::Cast<ShaderAsset>(AssetSystem::Load("asset/shader/lighting_point.ps.glsl"));

				if (shadowVS) light->AddShader(Memory::MakeRef<ShadowVSState>(shadowVS));
				if (shadowGS) light->AddShader(Memory::MakeRef<GeometryGSState>(shadowGS));
				if (shadowPS) light->AddShader(Memory::MakeRef<ShadowPSState>(shadowPS));
				if (lightVS)  light->AddShader(Memory::MakeRef<LightVSState>(lightVS));
				if (lightPS)  light->AddShader(Memory::MakeRef<LightPSState>(lightPS));
			}
			else
			{
				LOGWARN() << "[GlobalShaderSelector] Failed to set the shader to the light proxy, the light type isn't supported";
			}
		}
	}
}