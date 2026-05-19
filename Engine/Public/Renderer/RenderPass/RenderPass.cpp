#include <Renderer/RenderPass/RenderPass.h>

namespace wtr
{
	ComputePass::ComputePass(const uint32_t groupX, const uint32_t groupY, const uint32_t groupZ)
		: m_groupX(groupX)
		, m_groupY(groupY)
		, m_groupZ(groupZ)
	{
	}
}