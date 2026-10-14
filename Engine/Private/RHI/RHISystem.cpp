#include <RHI/RHISystem.h>

#include <RHI/RHIUtils.h>
#include <RHI/RHIResources.h>
#include <Memory/include/Core.h>

namespace wtr
{
	RHISystem::RHISystem(const eRenderType type)
		: m_pendingResources()
		, m_stateTracker()
	{
		RHIUtils::SetRenderType(type);
	}

	RHISystem::~RHISystem()
	{}

	void RHISystem::RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer)
	{
		if (buffer)
		{
			m_pendingResources.Insert(buffer);
		}
	}

	void RHISystem::RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout)
	{
		if (layout)
		{
			m_pendingResources.Insert(layout);
		}
	}

	void RHISystem::RemoveTexture(Memory::RefPtr<RHITexture> texture)
	{
		if (texture)
		{
			m_pendingResources.Insert(texture);
		}
	}

	void RHISystem::RemoveSampler(Memory::RefPtr<RHISampler> sampler)
	{
		if (sampler)
		{
			m_pendingResources.Insert(sampler);
		}
	}

	void RHISystem::RemoveShader(Memory::RefPtr<RHIShader> shader)
	{
		if (shader)
		{
			m_pendingResources.Insert(shader);
		}
	}

	void RHISystem::RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline)
	{
		if (pipeline)
		{
			m_pendingResources.Insert(pipeline);
		}
	}

	void RHISystem::RemoveRenderPass(Memory::RefPtr<RHIRenderPass> pass)
	{
		if (pass)
		{
			m_pendingResources.Insert(pass);
		}
	}

	void RHISystem::FlushPending()
	{
		auto itr = m_pendingResources.Begin();
		while (itr != m_pendingResources.End())
		{
			auto& resource = *itr;
			if (!resource)
			{
				itr = m_pendingResources.Erase(itr);
				continue;
			}

			Memory::RefCounted* refCounted = resource.GetRefData();
			if (nullptr == refCounted)
			{
				itr = m_pendingResources.Erase(itr);

				assert("[RHISystem] The resource hasn't ref counted, the memory leak occurred!" && false);

				continue;
			}

			if (refCounted->GetRefCount() > 1)
			{
				itr++;
				continue;
			}

			bool released = false;
			const eResourceType type = resource->GetResourceType();
			if (type == eResourceType::eBuffer)
			{
				Memory::RefPtr<RHIBuffer> buffer = Memory::Cast<RHIBuffer>(resource);
				released = ReleaseBuffer(buffer);
			}
			else if (type == eResourceType::eLayout)
			{
				Memory::RefPtr<RHIVertexLayout> layout = Memory::Cast<RHIVertexLayout>(resource);
				released = ReleaseVertexLayout(layout);
			}
			else if (type == eResourceType::eTexture)
			{
				Memory::RefPtr<RHITexture> texture = Memory::Cast<RHITexture>(resource);
				released = ReleaseTexture(texture);
			}
			else if (type == eResourceType::eSampler)
			{
				Memory::RefPtr<RHISampler> sampler = Memory::Cast<RHISampler>(resource);
				released = ReleaseSampler(sampler);
			}
			else if (type == eResourceType::eShader)
			{
				Memory::RefPtr<RHIShader> shader = Memory::Cast<RHIShader>(resource);
				released = ReleaseShader(shader);
			}
			else if (type == eResourceType::ePipeLine)
			{
				Memory::RefPtr<RHIPipeLine> pipeline = Memory::Cast<RHIPipeLine>(resource);
				released = ReleasePipeLine(pipeline);
			}
			else if (type == eResourceType::ePass)
			{
				Memory::RefPtr<RHIRenderPass> pass = Memory::Cast<RHIRenderPass>(resource);
				released = ReleaseRenderPass(pass);
			}
			else
			{
				assert("[RHISystem] Unknown resource type!" && false);
				itr++;

				continue;
			}

			if (released)
			{
				itr = m_pendingResources.Erase(itr);
			}
			else
			{
				assert("[RHISystem] Failed to release the resource!" && false);

				itr++;
			}
		}
	}
}