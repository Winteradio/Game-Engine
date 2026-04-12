#include <Renderer/PipeLine/PipeLine.h>

#include <RHI/RHIResources.h>

namespace wtr
{
	PipeLine::PipeLine()
		: m_pipeLine(nullptr)
		, m_prepared(false)
	{}

	PipeLine::~PipeLine()
	{}

	void PipeLine::Execute(const RenderView& renderView, const MeshDrawCommands& meshDrawCommands, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!m_prepared)
		{
			Prepare();
		}
		else
		{
			Draw(renderView, meshDrawCommands, cmdList);
		}
	}
}