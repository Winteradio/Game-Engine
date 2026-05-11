#ifndef __WTR_MESHDRAWCOMMAND_H__
#define __WTR_MESHDRAWCOMMAND_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Framework/Math/MathTypes.h>
#include <Renderer/RenderTypes.h>

namespace wtr
{
	class RHIVertexLayout;
	class RHIBuffer;
	class MaterialProxy;
};

namespace wtr
{
	struct IndirectDrawCommand
	{
		uint32_t indexCount		= 0;
		uint32_t instanceCount	= 0;

		uint32_t firstIndex		= 0;
		uint32_t vertexOffset	= 0;
		uint32_t firstInstance	= 0;
	};

	struct MeshDrawCommand
	{
		Memory::RefPtr<RHIBuffer> transform				= nullptr;
		Memory::RefPtr<RHIBuffer> visible				= nullptr;
		Memory::RefPtr<RHIBuffer> indirect				= nullptr;
		Memory::RefPtr<RHIVertexLayout> vertexLayout	= nullptr;
		Memory::RefPtr<const MaterialProxy> material	= nullptr;

		size_t minVertexIndex	= 0;
		size_t maxVertexIndex	= 0;
		size_t instanceCount	= 0;
		size_t indexOffset		= 0;
		size_t indexCount		= 0;
		eDataType indexType		= eDataType::eNone;

		bool indirectDraw		= true;
		eDrawMode drawMode		= eDrawMode::eTriangles;
	};
};

#endif // __WTR_MESHDRAWCOMMAND_H__