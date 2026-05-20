#include <Renderer/RenderPass/RenderPass.h>

#include <RHI/RHICommandList.h>

namespace wtr
{
	ComputePass::ComputePass(const uint32_t groupX, const uint32_t groupY, const uint32_t groupZ)
		: m_groupX(groupX)
		, m_groupY(groupY)
		, m_groupZ(groupZ)
	{
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

		if (m_clear)
		{
			cmdList->Clear(*m_clear);
		}
	}
}