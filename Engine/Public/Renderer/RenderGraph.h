#ifndef __WTR_RENDERGRAPH_H__
#define __WTR_RENDERGRAPH_H__

#include <Renderer/RenderTypes.h>
#include <Renderer/PipeLine/PipeLineGraph.h>

namespace wtr
{
	class RenderView;
	class RHICommandList;
};

namespace wtr
{
	class RenderGraph
	{
	public :
		RenderGraph();
		~RenderGraph();

	public :
		bool Init();
		void Add(Memory::RefPtr<PipeLine> pipeline);
		void Remove(Memory::RefPtr<PipeLine> pipeline);

		void Execute(Memory::RefPtr<RHICommandList> cmdList, const RenderView& renderView);

	private :
		PipeLineGraph m_graph;
	};
};

#endif // __WTR_RENDERGRAPH_H__