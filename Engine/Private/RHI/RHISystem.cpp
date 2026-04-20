#include <RHI/RHISystem.h>

namespace wtr
{
	RHISystem::RHISystem()
		: m_pendingBuffers()
		, m_pendingVertexLayouts()
		, m_pendingTextures()
		, m_pendingSamplers()
		, m_pendingShaders()
		, m_pendingPipeLines()
	{}

	RHISystem::~RHISystem()
	{}

	void RHISystem::RemoveBuffer(Memory::RefPtr<RHIBuffer> buffer)
	{
		if (buffer)
		{
			m_pendingBuffers.Insert(buffer);
		}
	}

	void RHISystem::RemoveVertexLayout(Memory::RefPtr<RHIVertexLayout> layout)
	{
		if (layout)
		{
			m_pendingVertexLayouts.Insert(layout);
		}
	}

	void RHISystem::RemoveTexture(Memory::RefPtr<RHITexture> texture)
	{
		if (texture)
		{
			m_pendingTextures.Insert(texture);
		}
	}

	void RHISystem::RemoveSampler(Memory::RefPtr<RHISampler> sampler)
	{
		if (sampler)
		{
			m_pendingSamplers.Insert(sampler);
		}
	}

	void RHISystem::RemoveShader(Memory::RefPtr<RHIShader> shader)
	{
		if (shader)
		{
			m_pendingShaders.Insert(shader);
		}
	}

	void RHISystem::RemovePipeLine(Memory::RefPtr<RHIPipeLine> pipeline)
	{
		if (pipeline)
		{
			m_pendingPipeLines.Insert(pipeline);
		}
	}
}