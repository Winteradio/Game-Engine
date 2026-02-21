#ifndef __WTR_RENDERGRAPH_H__
#define __WTR_RENDERGRAPH_H__

#include <Renderer/RenderTypes.h>
#include <Renderer/PipeLine/PipeLineGraph.h>

namespace wtr
{
	class FrameView;
};

namespace wtr
{
	struct RenderDesc
	{
		eRenderType Type = eRenderType::eNone;
	};

	class RenderGraph
	{
	public :
		RenderGraph();
		~RenderGraph();

	public :
		void Add(Memory::RefPtr<PipeLine> pipeline);
		void Remove(Memory::RefPtr<PipeLine> pipeline);
		void Prepare();
		void Draw(const FrameView& frame);

	private :
		PipeLineGraph m_graph;
	};
};

#endif // __WTR_RENDERGRAPH_H__