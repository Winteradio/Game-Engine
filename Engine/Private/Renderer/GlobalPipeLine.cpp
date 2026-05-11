#include <Renderer/GlobalPipeLine.h>

#include <Container/include/HashSet.h>

#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <RHI/RHICommandList.h>

#include <Log/include/Log.h>

namespace wtr
{
	namespace GlobalPipeLine
	{
		struct PipeLineHasher
		{
			size_t operator()(const RHIPipeLineCreateDesc& desc) const
			{
				size_t seed = 0;
				auto combine = [](size_t& seed, const auto& value)
				{
					std::hash<std::decay_t<decltype(value)>> hasher;
					seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
				};

				combine(seed, desc.clear.Get());
				combine(seed, desc.color.Get());
				combine(seed, desc.depth.Get());
				combine(seed, desc.stencil.Get());
				combine(seed, desc.blend.Get());
				combine(seed, desc.rasterizer.Get());
				combine(seed, desc.vertexShader.Get());
				combine(seed, desc.geometryShader.Get());
				combine(seed, desc.hullShader.Get());
				combine(seed, desc.domainShader.Get());
				combine(seed, desc.computeShader.Get());
				combine(seed, desc.pixelShader.Get());

				return seed;
			}
		};

		wtr::HashMap<const RHIPipeLineCreateDesc, Memory::RefPtr<RHIPipeLine>, PipeLineHasher> PIPELINE_CACHE;

		Memory::RefPtr<RHIPipeLine> CreatePipeLine(Memory::RefPtr<RHICommandList> cmdList, const RHIPipeLineCreateDesc desc)
		{
			auto& vertexShader = desc.vertexShader;
			if (vertexShader && vertexShader->GetState() != eResourceState::eReady)
			{
				return nullptr;
			}

			auto& geometryShader = desc.geometryShader;
			if (geometryShader && geometryShader->GetState() != eResourceState::eReady)
			{
				return nullptr;
			}

			auto& hullShader = desc.hullShader;
			if (hullShader && hullShader->GetState() != eResourceState::eReady)
			{
				return nullptr;
			}

			auto& domainShader = desc.domainShader;
			if (domainShader && domainShader->GetState() != eResourceState::eReady)
			{
				return nullptr;
			}

			auto& pixelShader = desc.pixelShader;
			if (pixelShader && pixelShader->GetState() != eResourceState::eReady)
			{
				return nullptr;
			}

			auto& computeShader = desc.computeShader;
			if (computeShader && computeShader->GetState() != eResourceState::eReady)
			{
				return nullptr;
			}

			Memory::RefPtr<RHIPipeLine> pipeline = cmdList->CreatePipeLine(desc);
			if (!pipeline)
			{
				LOGINFO() << "[GlobalPipeLine] Failed to create pipeline.";
				return nullptr;
			}

			return pipeline;
		}

		Memory::RefPtr<RHIPipeLine> GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, const RHIPipeLineCreateDesc desc)
		{
			if (!cmdList)
			{
				return nullptr;
			}

			auto itr = PIPELINE_CACHE.Find(desc);
			if (itr != PIPELINE_CACHE.End())
			{
				return itr->second;
			}
			else
			{
				Memory::RefPtr<RHIPipeLine> pipeline = CreatePipeLine(cmdList, desc);
				if (pipeline)
				{
					PIPELINE_CACHE.Emplace(desc, pipeline);
				}

				return pipeline;
			}
		}

		void RemoveAll(Memory::RefPtr<RHICommandList> cmdList)
		{
			for (auto& [desc, pipeline] : PIPELINE_CACHE)
			{
				if (pipeline)
				{
					cmdList->RemovePipeLine(pipeline);
				}
				else
				{
					LOGINFO() << "[GlobalPipeLine] Failed to remove pipeline.";
				}
			}

			PIPELINE_CACHE.Clear();

			return;
		}
	}
}