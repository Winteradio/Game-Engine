#include <RHI/RHIPendingPool.h>

#include <RHI/RHIContext.h>
#include <RHI/RHIResources.h>
#include <Memory/include/Core.h>

#include <cassert>

namespace wtr
{
	void RHIPendingPool::Add(Memory::RefPtr<RHIResource> resource)
	{
		if (resource)
		{
			m_resources.Insert(resource);
		}
	}

	void RHIPendingPool::Flush(RHIContext& context)
	{
		auto itr = m_resources.Begin();
		while (itr != m_resources.End())
		{
			auto& resource = *itr;
			if (!resource)
			{
				itr = m_resources.Erase(itr);
				continue;
			}

			Memory::RefCounted* refCounted = resource.GetRefData();
			if (nullptr == refCounted)
			{
				itr = m_resources.Erase(itr);

				assert("[RHIPendingPool] The resource hasn't ref counted, the memory leak occurred!" && false);

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
				released = context.ReleaseBuffer(buffer);
			}
			else if (type == eResourceType::eLayout)
			{
				Memory::RefPtr<RHIVertexLayout> layout = Memory::Cast<RHIVertexLayout>(resource);
				released = context.ReleaseVertexLayout(layout);
			}
			else if (type == eResourceType::eTexture)
			{
				Memory::RefPtr<RHITexture> texture = Memory::Cast<RHITexture>(resource);
				released = context.ReleaseTexture(texture);
			}
			else if (type == eResourceType::eSampler)
			{
				Memory::RefPtr<RHISampler> sampler = Memory::Cast<RHISampler>(resource);
				released = context.ReleaseSampler(sampler);
			}
			else if (type == eResourceType::eShader)
			{
				Memory::RefPtr<RHIShader> shader = Memory::Cast<RHIShader>(resource);
				released = context.ReleaseShader(shader);
			}
			else if (type == eResourceType::ePipeLine)
			{
				Memory::RefPtr<RHIPipeLine> pipeline = Memory::Cast<RHIPipeLine>(resource);
				released = context.ReleasePipeLine(pipeline);
			}
			else if (type == eResourceType::ePass)
			{
				Memory::RefPtr<RHIRenderPass> pass = Memory::Cast<RHIRenderPass>(resource);
				released = context.ReleaseRenderPass(pass);
			}
			else
			{
				assert("[RHIPendingPool] Unknown resource type!" && false);
				itr++;

				continue;
			}

			if (released)
			{
				itr = m_resources.Erase(itr);
			}
			else
			{
				assert("[RHIPendingPool] Failed to release the resource!" && false);

				itr++;
			}
		}
	}

	bool RHIPendingPool::Empty() const
	{
		return m_resources.Empty();
	}
}
