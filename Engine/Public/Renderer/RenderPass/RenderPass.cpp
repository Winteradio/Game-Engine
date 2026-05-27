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

	void GraphicPass::SetState(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_color)
		{
			cmdList->SetColorState(*m_color);
		}

		if (m_depth)
		{
			cmdList->SetDepthState(*m_depth);
		}

		if (m_stencil)
		{
			cmdList->SetStencilState(*m_stencil);
		}

		if (m_rasterizer)
		{
			cmdList->SetRasterizerState(*m_rasterizer);
		}
	}
}