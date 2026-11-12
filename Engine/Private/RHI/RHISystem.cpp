#include <RHI/RHISystem.h>

#include <RHI/OpenGL/GLContext.h>
#include <RHI/Vulkan/VulkanContext.h>

#include <RHI/RHIDescriptions.h>
#include <RHI/RHIPendingPool.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIRingBuffer.h>

#include <Log/include/Log.h>
#include <Memory/include/Core.h>

namespace wtr
{
	namespace RHISystem
	{
		struct RHITransitionCache
		{
			wtr::HashSet<RHIBufferTransition> buffers = {};
			wtr::HashSet<RHITextureTransition> textures = {};
		};

		struct RHICache
		{
			Memory::RefPtr<const RHIVertexLayout> vertexLayout = nullptr;
			Memory::RefPtr<const RHIPipeLine> pipeline = nullptr;
			Memory::RefPtr<const RHIRenderPass> renderPass = nullptr;

			wtr::HashMap<Name, Memory::RefPtr<const RHITextureView>> textures = {};
			wtr::HashMap<Name, Memory::RefPtr<const RHISampler>> samplers = {};

			wtr::HashMap<Name, Memory::RefPtr<const RHIBufferView>> shaderBuffers = {};
			wtr::HashMap<VertexKey, Memory::RefPtr<const RHIBufferView>> vertexBuffers = {};
			Memory::RefPtr<const RHIBufferView> indexBuffer = nullptr;

			wtr::HashMap<Name, RHIConstDesc> consts = {};
			bool constChanged = false;
		};

		struct RHICore
		{
			using RHIContextPtr = Memory::RefPtr<RHIContext>;
			using RHIResourceSet = wtr::HashSet<Memory::RefPtr<RHIResource>>;

			RHIContextPtr context = nullptr;
			RHIResourceSet pendings = {};
			RHITransitionCache transition = {};
			RHICache cache = {};

			size_t frame;
			eRenderType type = eRenderType::eNone;
		};

		RHICore& GetCore()
		{
			static RHICore core = {};
			return core;
		}

		RHITransitionDesc GetTransitionDesc()
		{
			RHICore& core = GetCore();
			
			RHITransitionDesc desc;
			desc.buffers.Reserve(core.transition.buffers.Size());
			desc.textures.Reserve(core.transition.textures.Size());

			for (const auto& transition : core.transition.buffers)
			{
				desc.buffers.PushBack(transition);
			}

			for (const auto& transition : core.transition.textures)
			{
				desc.textures.PushBack(transition);
			}

			return desc;
		}

		void ClearTransition()
		{
			RHICore& core = GetCore();
			core.transition.buffers.Clear();
			core.transition.textures.Clear();
		}

		void FlushTransition()
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHISystem] Failed to flush transition : the context is not initialized.";
			}

			RHITransitionDesc desc = GetTransitionDesc();
			if (!desc.buffers.Empty() || !desc.textures.Empty())
			{
				core.context->Transition(desc);
				ClearTransition();
			}
		}

		void FlushConst()
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHISystem] Failed to flush const : the context is not initialized.";
			}

			if (!core.cache.constChanged)
			{
				return;
			}

			if (!core.cache.pipeline)
			{
				LOGWARN() << "[RHISystem] Failed to flush const : the pipeline is not set.";
				return;
			}

			// TODO : Implement const flush logic here		
		}

		bool Init(const eRenderType type, const size_t frameDiff, void* nativeHandle)
		{
			if (!nativeHandle)
			{
				LOGERROR() << "[RHISystem] Invalid native handle.";
				return false;
			}

			RHICore& core = GetCore();
			if (core.context)
			{
				LOGWARN() << "[RHISystem] RHIContext is already initialized.";
				return true;
			}

			switch (type)
			{
			case eRenderType::eOpenGL:
				core.context = Memory::MakeRef<GLContext>();
				core.type = eRenderType::eOpenGL;
				break;

			case eRenderType::eVulkan:
				core.context = Memory::MakeRef<VulkanContext>();
				core.type = eRenderType::eVulkan;
				break;

			default:
				LOGERROR() << "[RHISystem] Unsupported render type. : " << static_cast<int>(type);
				return false;
			}

			if (!core.context->Init(nativeHandle))
			{
				LOGERROR() << "[RHISystem] Failed to initialize RHIContext.";
				core.context.Reset();
				core.type = eRenderType::eNone;
				return false;
			}

			if (!core.context->InitResources(frameDiff))
			{
				LOGERROR() << "[RHISystem] Failed to initialize RHIContext resources.";
				core.context.Reset();
				core.type = eRenderType::eNone;
				return false;
			}
			
			return true;
		}

		void Release()
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHISystem] Failed to release RHIContext : the context is not initialized.";
				return;
			}

			core.cache = {};
			FlushPending(false);

			core.pendings.Clear();
			core.context.Reset();
			core.type = eRenderType::eNone;
		}

		bool Ready()
		{
			return GetCore().context != nullptr;
		}

		void BeginContext()
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->BeginContext();
			}
		}

		void EndContext()
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->EndContext();
			}
		}

		void BeginFrame()
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->BeginFrame();
				core.context->ResetRingBuffer(core.frame++);
			}
		}

		void EndFrame()
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->EndFrame();
			}
		}

		void Resize(const uint32_t width, const uint32_t height)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->Resize(width, height);
			}
		}

		void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->Resize(posX, posY, width, height);
			}
		}

		Memory::RefPtr<RHIVertexLayout> CreateVertexLayout(const RHIVertexLayoutDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateVertexLayout(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateBuffer(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateTexture(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateSampler(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreateVertexShader(const RHIShaderDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateVertexShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreateGeometryShader(const RHIShaderDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateGeometryShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreateHullShader(const RHIShaderDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateHullShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreateDomainShader(const RHIShaderDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateDomainShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreatePixelShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateComputeShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreatePipeLine(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIRenderPass> CreateRenderPass(const RHIRenderPassDesc& desc)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateRenderPass(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIBufferView> CreateBufferView(Memory::RefPtr<RHIBuffer> buffer)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateBufferView(buffer);
			}

			return nullptr;
		}

		Memory::RefPtr<RHITextureView> CreateTextureView(Memory::RefPtr<RHITexture> texture)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				return core.context->CreateTextureView(texture);
			}

			return nullptr;
		}

		void RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout)
		{
			RHICore& core = GetCore();
			if (layout)
			{
				core.pendings.Insert(layout);
			}
		}

		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer)
		{
			RHICore& core = GetCore();
			if (buffer)
			{
				core.pendings.Insert(buffer);
			}
		}
		
		void RemoveTexture(Memory::RefPtr<RHITexture> texture)
		{
			RHICore& core = GetCore();
			if (texture)
			{
				core.pendings.Insert(texture);
			}
		}

		void RemoveSampler(Memory::RefPtr<RHISampler> sampler)
		{
			RHICore& core = GetCore();
			if (sampler)
			{
				core.pendings.Insert(sampler);
			}
		}

		void RemoveShader(Memory::RefPtr<RHIShader> shader)
		{
			RHICore& core = GetCore();
			if (shader)
			{
				core.pendings.Insert(shader);
			}
		}

		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline)
		{
			RHICore& core = GetCore();
			if (pipeline)
			{
				core.pendings.Insert(pipeline);
			}
		}

		void RemoveRenderPass(Memory::RefPtr<RHIRenderPass> renderPass)
		{
			RHICore& core = GetCore();
			if (renderPass)
			{
				core.pendings.Insert(renderPass);
			}
		}

		void RemoveBufferView(Memory::RefPtr<RHIBufferView> bufferView)
		{
			RHICore& core = GetCore();
			if (bufferView)
			{
				core.pendings.Insert(bufferView);
			}
		}

		void RemoveTextureView(Memory::RefPtr<RHITextureView> textureView)
		{
			RHICore& core = GetCore();
			if (textureView)
			{
				core.pendings.Insert(textureView);
			}
		}

		void FlushPending(const bool wait)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHISystem] RHIContext is not valid. Pending resources will not be flushed.";
				return;
			}

			auto itr = core.pendings.begin();
			while (itr != core.pendings.end())
			{
				Memory::RefPtr<RHIResource> resource = *itr;
				if (!resource)
				{
					itr = core.pendings.Erase(itr);
					continue;
				}

				if (wait)
				{
					Memory::RefCounted* refCounted = resource.GetRefData();
					if (!refCounted)
					{
						itr = core.pendings.Erase(itr);
						continue;
					}
					else if (refCounted->GetRefCount() > 2)
					{
						++itr;
						continue;
					}
					else
					{
						// nothing
					}
				}

				if (auto layout = Memory::Cast<RHIVertexLayout>(resource))
				{
					core.context->ReleaseVertexLayout(layout);
				}
				else if (auto buffer = Memory::Cast<RHIBuffer>(resource))
				{
					core.context->ReleaseBuffer(buffer);
				}
				else if (auto texture = Memory::Cast<RHITexture>(resource))
				{
					core.context->ReleaseTexture(texture);
				}
				else if (auto sampler = Memory::Cast<RHISampler>(resource))
				{
					core.context->ReleaseSampler(sampler);
				}
				else if (auto shader = Memory::Cast<RHIShader>(resource))
				{
					core.context->ReleaseShader(shader);
				}
				else if (auto pipeline = Memory::Cast<RHIPipeLine>(resource))
				{
					core.context->ReleasePipeLine(pipeline);
				}
				else if (auto renderPass = Memory::Cast<RHIRenderPass>(resource))
				{
					core.context->ReleaseRenderPass(renderPass);
				}
				else if (auto bufferView = Memory::Cast<RHIBufferView>(resource))
				{
					core.context->ReleaseBufferView(bufferView);
				}
				else if (auto textureView = Memory::Cast<RHITextureView>(resource))
				{
					core.context->ReleaseTextureView(textureView);
				}
				else
				{
					LOGWARN() << "[RHISystem] Unknown resource type. Resource will not be flushed.";
				}

				itr = core.pendings.Erase(itr);
			}
		}

		void InitializeVertexLayout(Memory::RefPtr<RHIVertexLayout> layout, const RHIVertexLayoutCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeVertexLayout(layout, info);
			}
		}

		void InitializeBuffer(Memory::RefPtr<RHIBuffer> buffer, const RHIBufferCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeBuffer(buffer, info);
			}
		}

		void InitializeTexture(Memory::RefPtr<RHITexture> texture, const RHITextureCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeTexture(texture, info);
			}
		}

		void InitializeSampler(Memory::RefPtr<RHISampler> sampler, const RHISamplerCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeSampler(sampler, info);
			}
		}

		void InitializeShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeShader(shader, info);
			}
		}

		void InitializePipeLine(Memory::RefPtr<RHIPipeLine> pipeline, const RHIPipeLineCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializePipeLine(pipeline, info);
			}
		}

		void InitializeRenderPass(Memory::RefPtr<RHIRenderPass> pass, const RHIRenderPassCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeRenderPass(pass, info);
			}
		}

		void InitializeBufferView(Memory::RefPtr<RHIBufferView> bufferView, Memory::RefPtr<RHIBuffer> buffer)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeBufferView(bufferView, buffer);
			}
		}

		void InitializeTextureView(Memory::RefPtr<RHITextureView> textureView, Memory::RefPtr<RHITexture> texture)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeTextureView(textureView, texture);
			}
		}

		void UpdateBuffer(Memory::RefPtr<RHIBufferView> bufferView, const RHIBufferUpdateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->UpdateBuffer(bufferView, info);
			}
		}

		void UpdateTexture(Memory::RefPtr<RHITextureView> textureView, const RHITextureUpdateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->UpdateTexture(textureView, info);
			}
		}

		void SetConstant(const RHIConstDesc& info, const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.cache.pipeline)
			{
				LOGWARN() << "[RHI] Failed to set the constant, the pipeline is not set";
				return;
			}

			auto& pipeline = core.cache.pipeline;
			const RHIStageConst& stage = pipeline->GetConst(binding);
			if (stage.binding == INVALID_BINDING)
			{
				LOGWARN() << "[RHI] Failed to set the constant, the binding is not found in the pipeline";
				return;
			}

			auto& cacheConst = core.cache.consts[binding];
			const bool alreadySet = (cacheConst == info);
			if (!alreadySet)
			{
				cacheConst = info;
				core.cache.constChanged = true;
			}
		}

		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the vertex layout, the context is invalid";
				return;
			}

			auto& cacheVertexLayout = core.cache.vertexLayout;
			const bool alreadySet = (cacheVertexLayout == layout);
			if (!alreadySet)
			{
				core.context->SetVertexLayout(layout);
				cacheVertexLayout = layout;
			}
		}

		void SetVertexBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const VertexKey& key)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the vertex buffer, the context is invalid";
				return;
			}

			auto& pipeline = core.cache.pipeline;
			if (!pipeline)
			{
				LOGWARN() << "[RHI] Failed to set the vertex buffer, the pipeline is not set";
				return;
			}

			const RHIStageAttrib& stage = pipeline->GetAttrib(key);
			if (stage.binding == INVALID_BINDING)
			{
				LOGWARN() << "[RHI] Failed to set the vertex buffer, the binding is not found in the pipeline";
				return;
			}

			auto& cache = core.cache.vertexBuffers[key];
			const bool alreadySet = (cache == bufferView);
			if (!alreadySet)
			{
				core.context->SetVertexBuffer(bufferView, stage.binding);
				cache = bufferView;
			}
		}

		void SetIndexBuffer(Memory::RefPtr<const RHIBufferView> bufferView)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the index buffer, the context is invalid";
				return;
			}

			auto& cache = core.cache.indexBuffer;
			const bool alreadySet = (cache == bufferView);
			if (!alreadySet)
			{
				core.context->SetIndexBuffer(bufferView);
				cache = bufferView;
			}
		}

		void SetBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the shader buffer, the context is invalid";
				return;
			}

			auto& pipeline = core.cache.pipeline;
			if (!pipeline)
			{
				LOGWARN() << "[RHI] Failed to set the buffer, the pipeline is not set";
				return;
			}

			const RHIStageBuffer& stage = pipeline->GetBuffer(binding);
			if (stage.binding == INVALID_BINDING)
			{
				LOGWARN() << "[RHI] Failed to set the shader buffer, the binding is not found in the pipeline";
				return;
			}

			auto& cacheBuffer = core.cache.shaderBuffers[binding];
			const bool alreadySet = (cacheBuffer == bufferView);
			if (!alreadySet)
			{
				core.context->SetBuffer(bufferView, stage.binding);
				cacheBuffer = bufferView;
			}
		}

		void SetTexture(Memory::RefPtr<const RHITextureView> textureView, const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the texture, the context is invalid";
				return;
			}

			auto& pipeline = core.cache.pipeline;
			if (!pipeline)
			{
				LOGWARN() << "[RHI] Failed to set the buffer, the pipeline is not set";
				return;
			}

			const RHIStageTexture& stage = pipeline->GetTexture(binding);
			if (stage.binding == INVALID_BINDING)
			{
				LOGWARN() << "[RHI] Failed to set the texture, the binding is not found in the pipeline";
				return;
			}

			auto& cacheTexture = core.cache.textures[binding];
			const bool alreadySet = (cacheTexture == textureView);
			if (!alreadySet)
			{
				core.context->SetTexture(textureView, stage.binding);
				cacheTexture = textureView;
			}
		}

		void SetSampler(Memory::RefPtr<const RHISampler> sampler, const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the sampler, the context is invalid";
				return;
			}

			auto& pipeline = core.cache.pipeline;
			if (!pipeline)
			{
				LOGWARN() << "[RHI] Failed to set the sampler, the pipeline is not set";
				return;
			}

			const RHIStageSampler& stage = pipeline->GetSampler(binding);
			if (stage.binding == INVALID_BINDING)
			{
				LOGWARN() << "[RHI] Failed to set the sampler, the binding is not found in the pipeline";
				return;
			}

			auto& cache = core.cache.samplers[binding];
			const bool alreadySet = (cache == sampler);
			if (!alreadySet)
			{
				core.context->SetSampler(sampler, stage.binding);
				cache = sampler;
			}
		}

		void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the pipeline, the context is invalid";
				return;
			}

			auto& cache = core.cache.pipeline;
			const bool alreadySet = (cache == pipeline);
			if (!alreadySet)
			{
				core.context->SetPipeLine(pipeline);
				cache = pipeline;
			}
		}

		void SetRenderPass(Memory::RefPtr<const RHIRenderPass> renderPass)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the render pass, the context is invalid";
				return;
			}

			FlushTransition();

			auto& cache = core.cache.renderPass;
			const bool alreadySet = (cache == renderPass);
			if (!alreadySet)
			{
				core.context->SetRenderPass(renderPass);
				cache = renderPass;
			}
		}

		void UnsetVertexLayout()
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the vertex layout, the context is invalid";
				return;
			}

			core.context->UnsetVertexLayout();
			core.cache.vertexLayout = nullptr;
		}

		void UnsetVertexBuffer(const VertexKey& key)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the vertex buffer, the context is invalid";
				return;
			}

			auto& pipeline = core.cache.pipeline;
			if (!pipeline)
			{
				LOGWARN() << "[RHI] Failed to unset the vertex buffer, the pipeline is not set";
				return;
			}

			const RHIStageAttrib& stage = pipeline->GetAttrib(key);
			if (stage.binding == INVALID_BINDING)
			{
				LOGWARN() << "[RHI] Failed to unset the vertex buffer, the binding is not found in the pipeline";
				return;
			}

			core.context->UnsetVertexBuffer(stage.binding);
			core.cache.vertexBuffers.Erase(key);
		}

		void UnsetIndexBuffer()
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the index buffer, the context is invalid";
				return;
			}

			core.context->UnsetIndexBuffer();
			core.cache.indexBuffer = {};
		}

		void UnsetBuffer(Memory::RefPtr<const RHIBufferView> bufferView, const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the shader buffer, the context is invalid";
				return;
			}

			auto& pipeline = core.cache.pipeline;
			if (!pipeline)
			{
				LOGWARN() << "[RHI] Failed to unset the shader buffer, the pipeline is not set";
				return;
			}

			const RHIStageBuffer& stage = pipeline->GetBuffer(binding);
			if (stage.binding == INVALID_BINDING)
			{
				LOGWARN() << "[RHI] Failed to unset the shader buffer, the binding is not found in the pipeline";
				return;
			}

			core.context->UnsetBuffer(bufferView, stage.binding);
			core.cache.shaderBuffers.Erase(binding);
		}

		void UnsetTexture(Memory::RefPtr<const RHITextureView> textureView, const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the texture, the context is invalid";
				return;
			}

			auto& pipeline = core.cache.pipeline;
			if (!pipeline)
			{
				LOGWARN() << "[RHI] Failed to unset the texture, the pipeline is not set";
				return;
			}

			const RHIStageTexture& stage = pipeline->GetTexture(binding);
			if (stage.binding == INVALID_BINDING)
			{
				LOGWARN() << "[RHI] Failed to unset the texture, the binding is not found in the pipeline";
				return;
			}

			core.context->UnsetTexture(textureView, stage.binding);
			core.cache.textures.Erase(binding);
		}

		void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the sampler, the context is invalid";
				return;
			}

			auto& pipeline = core.cache.pipeline;
			if (!pipeline)
			{
				LOGWARN() << "[RHI] Failed to unset the sampler, the pipeline is not set";
				return;
			}

			const RHIStageSampler& stage = pipeline->GetSampler(binding);
			if (stage.binding == INVALID_BINDING)
			{
				LOGWARN() << "[RHI] Failed to unset the sampler, the binding is not found in the pipeline";
				return;
			}

			core.context->UnsetSampler(sampler, stage.binding);
			core.cache.samplers.Erase(binding);
		}

		void UnsetPipeLine()
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the pipeline, the context is invalid";
				return;
			}

			core.context->UnsetPipeLine();
			core.cache.pipeline = nullptr;
		}

		void UnsetRenderPass()
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the render pass, the context is invalid";
				return;
			}

			core.context->UnsetRenderPass();
			core.cache.renderPass = nullptr;
		}

		void Transition(Memory::RefPtr<const RHIBufferView> bufferView, const RHIBufferState& after)
		{
			RHICore& core = GetCore();
			if (!core.context || !bufferView)
			{
				LOGWARN() << "[RHI] Failed to transition the buffer, the context or buffer is invalid";
				return;
			}

			const RHIBufferState& before = bufferView->GetBufferState();
			if (before == after)
			{
				return;
			}

			RHIBufferTransition transition;
			transition.buffer = bufferView;
			transition.before = bufferView->GetBufferState();
			transition.after = after;

			core.transition.buffers.Insert(transition);
		}

		void Transition(Memory::RefPtr<const RHITextureView> textureView, const RHITextureState& after)
		{
			RHICore& core = GetCore();
			if (!core.context || !textureView)
			{
				LOGWARN() << "[RHI] Failed to transition the texture, the context or texture is invalid";
				return;
			}

			const RHITextureState& before = textureView->GetTextureState();
			if (before == after)
			{
				return;
			}

			RHITextureTransition transition;
			transition.texture = textureView;
			transition.before = before;
			transition.after = after;

			core.transition.textures.Insert(transition);
		}

		void DispatchCompute(const RHIDispatchDesc& desc)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to dispatch for compute, the context is invalid";
				return;
			}

			FlushConst();

			core.context->DispatchCompute(desc);
		}

		void DrawDirect(const RHIDrawDirectDesc& desc)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to draw the direct call, the context is invalid";
				return;
			}

			FlushConst();

			core.context->DrawDirect(desc);
		}

		void DrawIndirect(const RHIDrawIndirectDesc& desc)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to draw the indirect call, the context is invalid";
				return;
			}

			FlushConst();

			core.context->DrawIndirect(desc);
		}
	}
}
