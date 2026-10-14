#include <RHI/RHISystem.h>

#include <RHI/OpenGL/GLContext.h>
#include <RHI/Vulkan/VulkanContext.h>

#include <RHI/RHIResources.h>
#include <RHI/RHIPendingPool.h>
#include <RHI/RHIConstantPool.h>

#include <Framework/Name.h>
#include <Log/include/Log.h>
#include <Memory/include/Core.h>

namespace wtr
{
	namespace RHISystem
	{
		namespace
		{
			using RHIContextPtr = Memory::RefPtr<RHIContext>;
			using RHIPendingSet = wtr::HashSet<Memory::RefPtr<RHIResource>>;

			RHIContextPtr s_context = nullptr;
			RHIPendingSet s_pending = {};

			// SetConstant가 블록 이름으로 binding 번호를 찾는 데 쓰는 현재 파이프라인과, 상수 섀도 / 링
			Memory::RefPtr<const RHIPipeLine> s_pipeline = nullptr;
			RHIConstantPool s_constants = {};
			eRenderType s_type = eRenderType::eNone;

		}

		bool Init(const eRenderType type, void* nativeHandle)
		{
			if (!nativeHandle)
			{
				LOGERROR() << "[RHISystem] Invalid native handle.";
				return false;
			}

			if (s_context)
			{
				LOGWARN() << "[RHISystem] RHIContext is already initialized.";
				return true;
			}

			switch (type)
			{
			case eRenderType::eOpenGL:
				s_context = Memory::MakeRef<GLContext>();
				break;

			case eRenderType::eVulkan:
				s_context = Memory::MakeRef<VulkanContext>();
				break;

			default:
				LOGERROR() << "[RHISystem] Unsupported render type. : " << static_cast<int>(type);
				return false;
			}

			if (!s_context->Init(nativeHandle))
			{
				LOGERROR() << "[RHISystem] Failed to initialize RHIContext.";
				s_context.Reset();
				return false;
			}

			if (!s_context->InitResources())
			{
				LOGERROR() << "[RHISystem] Failed to initialize RHIContext resources.";
				s_context.Reset();
				return false;
			}

			// 컨텍스트가 초기화된 뒤에 연결 (정렬 한도는 첫 Flush 때 컨텍스트에서 읽음)
			s_constants.Init(s_context.operator->());

			s_type = type;
			
			return true;
		}

		void Release()
		{
			if (s_context)
			{
				// 링 청크도 지연 해제로 넘겨 컨텍스트가 파괴하게 한다
				auto chunks = s_constants.Release();
				for (size_t index = 0; index < chunks.Size(); ++index)
				{
					s_pending.Insert(chunks[index]);
				}

				FlushPending();

				s_pipeline.Reset();
				s_context.Reset();
			}
		}

		void BeginContext()
		{
			if (s_context)
			{
				s_context->BeginContext();
			}
		}

		void EndContext()
		{
			if (s_context)
			{
				s_context->EndContext();
			}
		}

		void BeginFrame()
		{
			if (s_context)
			{
				s_context->BeginFrame();

				// BeginFrame이 이전 프레임의 fence를 기다린 뒤이므로 상수 링 슬라이스를 되감아도 안전함
				s_constants.Reset();
			}
		}

		void EndFrame()
		{
			if (s_context)
			{
				s_context->EndFrame();
			}
		}

		void Resize(const uint32_t width, const uint32_t height)
		{
			if (s_context)
			{
				s_context->Resize(width, height);
			}
		}

		void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height)
		{
			if (s_context)
			{
				s_context->Resize(posX, posY, width, height);
			}
		}

		Memory::RefPtr<RHIVertexLayout> CreateVertexLayout(const RHIVertexLayoutDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreateVertexLayout(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreateBuffer(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreateTexture(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreateSampler(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreateVertexShader(const RHIShaderDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreateVertexShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreateGeometryShader(const RHIShaderDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreateGeometryShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreateHullShader(const RHIShaderDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreateHullShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreateDomainShader(const RHIShaderDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreateDomainShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreatePixelShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreateComputeShader(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreatePipeLine(desc);
			}

			return nullptr;
		}

		Memory::RefPtr<RHIRenderPass> CreateRenderPass(const RHIRenderPassDesc& desc)
		{
			if (s_context)
			{
				return s_context->CreateRenderPass(desc);
			}

			return nullptr;
		}

		void RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout)
		{
			if (layout)
			{
				s_pending.Insert(layout);
			}
		}

		void RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer)
		{
			if (buffer)
			{
				s_pending.Insert(buffer);
			}
		}
		
		void RemoveTexture(Memory::RefPtr<RHITexture> texture)
		{
			if (texture)
			{
				s_pending.Insert(texture);
			}
		}

		void RemoveSampler(Memory::RefPtr<RHISampler> sampler)
		{
			if (sampler)
			{
				s_pending.Insert(sampler);
			}
		}

		void RemoveShader(Memory::RefPtr<RHIShader> shader)
		{
			if (shader)
			{
				s_pending.Insert(shader);
			}
		}

		void RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline)
		{
			if (pipeline)
			{
				s_pending.Insert(pipeline);
			}
		}

		void RemoveRenderPass(Memory::RefPtr<RHIRenderPass> renderPass)
		{
			if (renderPass)
			{
				s_pending.Insert(renderPass);
			}
		}

		void FlushPending()
		{
			if (!s_context)
			{
				LOGWARN() << "[RHISystem] RHIContext is not valid. Pending resources will not be flushed.";
				return;
			}

			auto itr = s_pending.begin();
			while (itr != s_pending.end())
			{
				Memory::RefPtr<RHIResource> resource = *itr;
				if (!resource)
				{
					itr = s_pending.Erase(itr);
					continue;
				}

				Memory::RefCounted* refCounted = resource.GetRefData();
				if (!refCounted)
				{
					itr = s_pending.Erase(itr);
					continue;
				}
				else if (refCounted->GetRefCount() > 1)
				{
					++itr;
					continue;
				}
				else
				{
					// nothing
				}

				if (auto layout = Memory::Cast<RHIVertexLayout>(resource))
				{
					s_context->ReleaseVertexLayout(layout);
				}
				else if (auto buffer = Memory::Cast<RHIBuffer>(resource))
				{
					s_context->ReleaseBuffer(buffer);
				}
				else if (auto texture = Memory::Cast<RHITexture>(resource))
				{
					s_context->ReleaseTexture(texture);
				}
				else if (auto sampler = Memory::Cast<RHISampler>(resource))
				{
					s_context->ReleaseSampler(sampler);
				}
				else if (auto shader = Memory::Cast<RHIShader>(resource))
				{
					s_context->ReleaseShader(shader);
				}
				else if (auto pipeline = Memory::Cast<RHIPipeLine>(resource))
				{
					s_context->ReleasePipeLine(pipeline);
				}
				else if (auto renderPass = Memory::Cast<RHIRenderPass>(resource))
				{
					s_context->ReleaseRenderPass(renderPass);
				}
				else
				{
					LOGWARN() << "[RHISystem] Unknown resource type. Resource will not be flushed.";
				}

				itr = s_pending.Erase(itr);
			}
		}

		void InitializeVertexLayout(const RHIVertexLayoutCreateDesc& info, Memory::RefPtr<RHIVertexLayout> layout)
		{
			if (s_context)
			{
				s_context->InitializeVertexLayout(info, layout);
			}
		}

		void InitializeBuffer(const RHIBufferCreateDesc& info, Memory::RefPtr<RHIBuffer> buffer)
		{
			if (s_context)
			{
				s_context->InitializeBuffer(info, buffer);
			}
		}

		void InitializeTexture(const RHITextureCreateDesc& info, Memory::RefPtr<RHITexture> texture)
		{
			if (s_context)
			{
				s_context->InitializeTexture(info, texture);
			}
		}

		void InitializeSampler(const RHISamplerCreateDesc& info, Memory::RefPtr<RHISampler> sampler)
		{
			if (s_context)
			{
				s_context->InitializeSampler(info, sampler);
			}
		}

		void InitializeShader(const RHIShaderCreateDesc& info, Memory::RefPtr<RHIShader> shader)
		{
			if (s_context)
			{
				s_context->InitializeShader(info, shader);
			}
		}

		void InitializePipeLine(const RHIPipeLineCreateDesc& info, Memory::RefPtr<RHIPipeLine> pipeline)
		{
			if (s_context)
			{
				s_context->InitializePipeLine(info, pipeline);
			}
		}

		void InitializeRenderPass(const RHIRenderPassCreateDesc& info, Memory::RefPtr<RHIRenderPass> pass)
		{
			if (s_context)
			{
				s_context->InitializeRenderPass(info, pass);
			}
		}

		void UpdateBuffer(const RHIBufferUpdateDesc& info, Memory::RefPtr<RHIBuffer> buffer)
		{
			if (s_context)
			{
				s_context->UpdateBuffer(info, buffer);
			}
		}

		void UpdateTexture(const RHITextureUpdateDesc& info, Memory::RefPtr<RHITexture> texture)
		{
			if (s_context)
			{
				s_context->UpdateTexture(info, texture);
			}
		}

		void UpdateVertexLayout(const RHIVertexLayoutCreateDesc& info, Memory::RefPtr<RHIVertexLayout> layout)
		{
			if (s_context)
			{
				s_context->UpdateVertexLayout(info, layout);
			}
		}

		void SetConstant(const RHIConstDesc& info, const Name& binding)
		{
			if (!s_pipeline)
			{
				LOGWARN() << "[RHISystem] Failed to set the constant, the pipeline is not set : " << binding;
				return;
			}

			// 상수 블록은 블록 이름(uLightPass 등)으로 파이프라인 리플렉션에 등록되어 있음
			const std::string& blockName = binding.str();
			if (!s_pipeline->HasBuffer(blockName))
			{
				LOGWARN() << "[RHISystem] Failed to set the constant, the pipeline has no such block : " << binding;
				return;
			}

			const RHIShaderBuffer& block = s_pipeline->GetBuffer(blockName);
			if (block.binding == UINT32_MAX)
			{
				LOGWARN() << "[RHISystem] Failed to set the constant, the block has no binding : " << binding;
				return;
			}

			s_constants.Set(info, static_cast<int32_t>(block.binding));
		}

		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout)
		{
			if (s_context)
			{
				s_context->SetVertexLayout(layout);
			}
		}

		void SetVertexBuffer(Memory::RefPtr<RHIBuffer> buffer, const uint32_t binding, const uint32_t offset)
		{
			if (s_context)
			{
				s_context->SetVertexBuffer(buffer, binding, offset);
			}
		}

		void SetIndexBuffer(Memory::RefPtr<RHIBuffer> buffer, const uint32_t offset)
		{
			if (s_context)
			{
				s_context->SetIndexBuffer(buffer, offset);
			}
		}

		void SetBuffer(Memory::RefPtr<RHIBuffer> buffer, const uint32_t binding, const uint32_t offset, const uint32_t range)
		{
			if (s_context)
			{
				s_context->SetBuffer(buffer, binding, offset, range);
			}
		}

		void SetTexture(Memory::RefPtr<RHITexture> texture, const uint32_t binding)
		{
			if (s_context)
			{
				s_context->SetTexture(texture, binding);
			}
		}

		void SetSampler(Memory::RefPtr<RHISampler> sampler, const uint32_t binding)
		{
			if (s_context)
			{
				s_context->SetSampler(sampler, binding);
			}
		}

		void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline)
		{
			if (s_context)
			{
				s_context->SetPipeLine(pipeline);

				s_pipeline = pipeline;

				// 파이프라인 레이아웃이 바뀌면 이전 바인딩이 유효하지 않을 수 있어 슬라이스를 다시 바인딩하게 한다
				s_constants.Invalidate();
			}
		}

		void UnsetPipeLine()
		{
			if (s_context)
			{
				s_context->UnsetPipeLine();

				s_pipeline.Reset();
			}
		}

		void SetRenderPass(Memory::RefPtr<const RHIRenderPass> renderPass)
		{
			if (s_context)
			{
				s_context->SetRenderPass(renderPass);
			}
		}

		void Transition(const RHITransitionDesc& desc)
		{
			if (s_context)
			{
				s_context->Transition(desc);
			}
		}

		void DispatchCompute(const RHIDispatchDesc& desc)
		{
			if (s_context)
			{
				// 이번 dispatch가 읽을 상수 슬라이스를 올리고 바인딩
				s_constants.Flush();

				s_context->DispatchCompute(desc);
			}
		}

		void DrawDirect(const RHIDrawDirectDesc& desc)
		{
			if (s_context)
			{
				s_constants.Flush();

				s_context->DrawDirect(desc);
			}
		}

		void DrawIndirect(const RHIDrawIndirectDesc& desc)
		{
			if (s_context)
			{
				s_constants.Flush();

				s_context->DrawIndirect(desc);
			}
		}
	}
}
