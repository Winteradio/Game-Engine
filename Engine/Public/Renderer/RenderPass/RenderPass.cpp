#include <Renderer/RenderPass/RenderPass.h>

#include <RHI/RHICommandList.h>
#include <RHI/RHIResources.h>
#include <Renderer/ShaderState.h>
#include <Renderer/GlobalRenderer.h>

namespace wtr
{
	ComputePass::ComputePass(const uint32_t groupX, const uint32_t groupY, const uint32_t groupZ)
		: m_groupX(groupX)
		, m_groupY(groupY)
		, m_groupZ(groupZ)
	{
	}

	Memory::RefPtr<const RHIPipeLine> ComputePass::GetPipeLine(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_computeShader)
		{
			return {};
		}

		RHIPipeLineCreateDesc desc;
		desc.computeShader = m_computeShader->GetShader();
		Memory::RefPtr<const RHIPipeLine> pipeline = GlobalResource::GetPipeLine(cmdList, desc);
		if (!pipeline || pipeline->GetState() != eResourceState::eReady)
		{
			return {};
		}

		return pipeline;
	}
}