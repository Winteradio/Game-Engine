#ifndef __WTR_RENDERGRAPH_H__
#define __WTR_RENDERGRAPH_H__

#include <Renderer/RenderTypes.h>
#include <Renderer/PipeLine/PipeLineGraph.h>

namespace wtr
{
	class FrameView;
	class RHICommandList;
};

namespace wtr
{
	struct RenderDesc
	{
		eRenderType Type = eRenderType::eNone;
		size_t FrameCount = 3;
	};

	class RenderGraph
	{
	public :
		RenderGraph();
		~RenderGraph();

	public :
		bool Init();
		void Add(Memory::RefPtr<PipeLine> pipeline);
		void Remove(Memory::RefPtr<PipeLine> pipeline);

		void PreDraw(Memory::RefPtr<RHICommandList> cmdList);
		void Draw(const FrameView& frame, Memory::RefPtr<RHICommandList> cmdList);
		void PostDraw(Memory::RefPtr<RHICommandList> cmdList);

	private :
		PipeLineGraph m_graph;
	};
};

#endif // __WTR_RENDERGRAPH_H__