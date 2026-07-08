#include <RHI/RHISystem.h>

#include <RHI/RHIUtils.h>
#include <RHI/RHIResources.h>

#include <Log/include/Log.h>

namespace wtr
{
	RHISystem::RHISystem(const eRenderType type)
		: m_pendingBuffers()
		, m_pendingVertexLayouts()
		, m_pendingTextures()
		, m_pendingSamplers()
		, m_pendingShaders()
		, m_pendingPipeLines()
		, m_pendingRenderTargets()
	{
		RHIUtils::SetRenderType(type);
	}

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

	void RHISystem::RemoveRenderTarget(Memory::RefPtr<RHIRenderTarget> target)
	{
		if (target)
		{
			m_pendingRenderTargets.Insert(target);
		}
	}

	bool RHISystem::CheckShaderVariable(Memory::RefPtr<RHIShader> shader)
	{
		if (!shader)
		{
			return false;
		}

		const auto& inputs = shader->GetInputs();
		for (const auto& [name, attribute] : inputs)
		{
			if (name.substr(0, 3) == "gl_")
			{
				continue;
			}

			const VertexKey vertexKey = GetVertexKey(name);
			if (vertexKey.semantic == eVertexSemantic::eNone || vertexKey.semanticIndex == 0xFF)
			{
				LOGWARN() << "[RHI] Unrecognized vertex attribute: " << name;
				continue;
			}

			const int32_t expectedLocation = GetVertexLocation(vertexKey);
			const int32_t actualLocation = attribute.location;

			if (actualLocation != expectedLocation)
			{
				LOGWARN() << "[RHI] Vertex attribute location mismatch: " << name
					<< ", expected location : " << expectedLocation
					<< ", actual location : " << actualLocation;

				return false;
			}
		}

		return true;
	}
}