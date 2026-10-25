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
			struct ShaderBuffer
			{
				Memory::RefPtr<const RHIBuffer> buffer = nullptr;
				uint32_t offset = 0;
				uint32_t range = 0;
			};

			struct VertexBuffer
			{
				Memory::RefPtr<const RHIBuffer> buffer = nullptr;
				uint32_t offset = 0;
			};

			struct IndexBuffer
			{
				Memory::RefPtr<const RHIBuffer> buffer = nullptr;
				uint32_t offset = 0;
			};

			wtr::HashMap<uint32_t, ShaderBuffer> shaderBuffers = {};
			wtr::HashMap<uint32_t, VertexBuffer> vertexBuffers = {};
			IndexBuffer indexBuffer = {};

			wtr::HashMap<uint32_t, RHIConstDesc> consts = {};
			wtr::HashMap<uint32_t, Memory::RefPtr<const RHITexture>> textures = {};
			wtr::HashMap<uint32_t, Memory::RefPtr<const RHISampler>> samplers = {};

			Memory::RefPtr<const RHIVertexLayout> vertexLayout = nullptr;
			Memory::RefPtr<const RHIPipeLine> pipeline = nullptr;
			Memory::RefPtr<const RHIRenderPass> renderPass = nullptr;
		};

		struct RHICore
		{
			using RHIContextPtr = Memory::RefPtr<RHIContext>;
			using RHIResourceSet = wtr::HashSet<Memory::RefPtr<RHIResource>>;

			RHIContextPtr context = nullptr;
			RHIResourceSet pendings = {};
			RHITransitionCache transition = {};
			RHICache cache = {};

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

			// 캐시가 들고 있는 참조를 먼저 놓아야 Pending 리소스가 해제될 수 있음
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
					else if (refCounted->GetRefCount() > 2) // pendings와 로컬 resource가 각각 하나씩 잡고 있음
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
				core.context->InitializeVertexLayout(info, layout);
			}
		}

		void InitializeBuffer(Memory::RefPtr<RHIBuffer> buffer, const RHIBufferCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeBuffer(info, buffer);
			}
		}

		void InitializeTexture(Memory::RefPtr<RHITexture> texture, const RHITextureCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeTexture(info, texture);
			}
		}

		void InitializeSampler(Memory::RefPtr<RHISampler> sampler, const RHISamplerCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeSampler(info, sampler);
			}
		}

		void InitializeShader(Memory::RefPtr<RHIShader> shader, const RHIShaderCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeShader(info, shader);
			}
		}

		void InitializePipeLine(Memory::RefPtr<RHIPipeLine> pipeline, const RHIPipeLineCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializePipeLine(info, pipeline);
			}
		}

		void InitializeRenderPass(Memory::RefPtr<RHIRenderPass> pass, const RHIRenderPassCreateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->InitializeRenderPass(info, pass);
			}
		}

		void UpdateBuffer(Memory::RefPtr<RHIBuffer> buffer, const RHIBufferUpdateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->UpdateBuffer(info, buffer);
			}
		}

		void UpdateTexture(Memory::RefPtr<RHITexture> texture, const RHITextureUpdateDesc& info)
		{
			RHICore& core = GetCore();
			if (core.context)
			{
				core.context->UpdateTexture(info, texture);
			}
		}

		void SetConstant(const RHIConstDesc& info, const Name& binding)
		{
			// TODO : RHIRingBuffer를 통해서, Uniform Buffer를 생성하고 사용할 수 있도록 수정 필요
		}

		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the vertex layout, the context is invalid";
				return;
			}

			auto& cache = core.cache.vertexLayout;
			const bool alreadySet = (cache == layout);
			if (!alreadySet)
			{
				core.context->SetVertexLayout(layout);
				cache = layout;
			}
		}

		void SetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const VertexKey& key, const uint32_t offset)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the vertex buffer, the context is invalid";
				return;
			}

			if (!core.cache.pipeline)
			{
				LOGWARN() << "[RHI] Failed to set the vertex buffer, the pipeline is not set";
				return;
			}

			auto& pipeline = core.cache.pipeline;

			const RHIShaderLayout& layout = pipeline->GetAttrib(key);
			auto& cache = core.cache.vertexBuffers[layout.location];
			const bool alreadySet = (cache.buffer == buffer) && (cache.offset == offset);
			if (!alreadySet)
			{
				core.context->SetVertexBuffer(buffer, layout., offset);
				cache.buffer = buffer;
				cache.offset = offset;
			}
		}

		void SetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the index buffer, the context is invalid";
				return;
			}

			auto& cache = core.cache.indexBuffer;
			const bool alreadySet = (cache.buffer == buffer) && (cache.offset == offset);
			if (!alreadySet)
			{
				core.context->SetIndexBuffer(buffer, offset);
				cache.buffer = buffer;
				cache.offset = offset;
			}
		}

		void SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const Name& binding, const uint32_t offset, const uint32_t range)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the shader buffer, the context is invalid";
				return;
			}

			const uint32_t bindingIndex = ; // TODO : RHIPipeline으로부터 추출
			auto& cache = core.cache.shaderBuffers[bindingIndex];
			const bool alreadySet = (cache.buffer == buffer) && (cache.offset == offset) && (cache.range == range);
			if (!alreadySet)
			{
				core.context->SetBuffer(buffer, bindingIndex, offset, range);
				cache.buffer = buffer;
				cache.offset = offset;
				cache.range = range;
			}
		}

		void SetTexture(Memory::RefPtr<const RHITexture> texture, const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to set the texture, the context is invalid";
				return;
			}

			const uint32_t bindingIndex = ; // TODO : RHIPipeline에서 추출
			auto& cache = core.cache.textures[bindingIndex];
			const bool alreadySet = (cache == texture);
			if (!alreadySet)
			{
				core.context->SetTexture(texture, bindingIndex);
				cache = texture;
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

			const uint32_t bindingIndex = ; // TODO : RHIPipeline에서 추출
			auto& cache = core.cache.samplers[bindingIndex];
			const bool alreadySet = (cache == sampler);
			if (!alreadySet)
			{
				core.context->SetSampler(sampler, bindingIndex);
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

			RHITransitionDesc desc = GetTransitionDesc();
			ClearTransition();

			core.context->Transition(desc);

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

		void UnsetVertexBuffer(const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the vertex buffer, the context is invalid";
				return;
			}

			const uint32_t bindingIndex = ; // TODO : RHIPipeline으로부터 추출
			core.context->UnsetVertexBuffer(bindingIndex);
			core.cache.vertexBuffers.Erase(bindingIndex);
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

		void UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the shader buffer, the context is invalid";
				return;
			}

			const uint32_t bindingIndex = ; // TODO : RHIPipeline으로부터 추출
			core.context->UnsetBuffer(buffer, bindingIndex);
			core.cache.shaderBuffers.Erase(bindingIndex);
		}

		void UnsetTexture(Memory::RefPtr<const RHITexture> texture, const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the texture, the context is invalid";
				return;
			}

			const uint32_t bindingIndex = ; // TODO : RHIPipeline으로부터 추출
			core.context->UnsetTexture(texture, bindingIndex);
			core.cache.textures.Erase(bindingIndex);
		}

		void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const Name& binding)
		{
			RHICore& core = GetCore();
			if (!core.context)
			{
				LOGWARN() << "[RHI] Failed to unset the sampler, the context is invalid";
				return;
			}

			const uint32_t bindingIndex = ; // TODO : RHIPipeline으로부터 추출
			core.context->UnsetSampler(sampler, bindingIndex);
			core.cache.samplers.Erase(bindingIndex);
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

		void Transition(Memory::RefPtr<const RHIBuffer> buffer, const RHIBufferState& after)
		{
			RHICore& core = GetCore();
			if (!core.context || !buffer)
			{
				LOGWARN() << "[RHI] Failed to transition the buffer, the context or buffer is invalid";
				return;
			}

			const RHIBufferState& before = buffer->GetBufferState();
			if (before == after)
			{
				return;
			}

			RHIBufferTransition transition;
			transition.buffer = buffer;
			transition.before = buffer->GetBufferState();
			transition.after = after;

			core.transition.buffers.Insert(transition);
		}

		void Transition(Memory::RefPtr<const RHITexture> texture, const RHITextureState& after)
		{
			RHICore& core = GetCore();
			if (!core.context || !texture)
			{
				LOGWARN() << "[RHI] Failed to transition the texture, the context or texture is invalid";
				return;
			}

			const RHITextureState& before = texture->GetTextureState();
			if (before == after)
			{
				return;
			}

			RHITextureTransition transition;
			transition.texture = texture;
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

			core.context->DrawIndirect(desc);
		}
	}
}
