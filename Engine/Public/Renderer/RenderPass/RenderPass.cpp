#include <Renderer/RenderPass/RenderPass.h>

#include <Renderer/MeshDrawCommand.h>
#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>

namespace wtr
{
	RenderPass::RenderPass()
	{
	}

	const RHIDrawIndexDesc RenderPass::GetDrawIndexDesc(Memory::RefPtr<MeshDrawCommand> drawCommand)
	{
		if (!drawCommand || !drawCommand->vertexLayout)
		{
			return {};
		}

		RHIDrawIndexDesc drawDesc;
		drawDesc.drawMode = drawCommand->drawMode;
		drawDesc.indexType = drawCommand->indexType;
		drawDesc.indexCount = drawCommand->indexCount;
		drawDesc.indexOffset = drawCommand->indexOffset * GetDataTypeSize(drawCommand->indexType);
		drawDesc.baseVertex = drawCommand->minVertexIndex;
		drawDesc.indirectDraw = drawCommand->indirectDraw;
		drawDesc.instanceCount = !drawCommand->indirectDraw ? drawCommand->instanceCount : drawDesc.instanceCount;

		return drawDesc;
	}
}