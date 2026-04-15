#ifndef __WTR_RENDERGRAPH_H__
#define __WTR_RENDERGRAPH_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <ECS/include/Graph/Graph.h>
#include <Renderer/RenderTypes.h>
#include <Renderer/PipeLine/PipeLine.h>

namespace wtr
{
	struct MeshDrawCommand;
	struct RenderView;

	class RenderScene;
	class GlobalResource;
	class RHICommandList;
};

namespace wtr
{
	class RenderGraph : public ECS::Graph<PipeLine>
	{
	public :
		using Super = ECS::Graph<PipeLine>;
		using PendingPipeLine = wtr::HashSet<Memory::RefPtr<PipeLine>, PipeLineHasher>;

		RenderGraph();
		~RenderGraph();

	public :
		bool Init(Memory::RefPtr<RHICommandList> cmdList);
		void Flush(Memory::RefPtr<RHICommandList> cmdList);

		void Execute(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<RenderScene> renderScene, const RenderView& renderView);

		PendingPipeLine& GetAddable();
		PendingPipeLine& GetRemovable();

	private :
		bool InitResource(Memory::RefPtr<RHICommandList> cmdList);
		bool InitPipeLine();

		void Add(Memory::RefPtr<PipeLine> pipeline);
		void Remove(Memory::RefPtr<PipeLine> pipeline);

		void FlushAddable(Memory::RefPtr<RHICommandList> cmdList);
		void FlushRemovable(Memory::RefPtr<RHICommandList> cmdList);

	private :
		PendingPipeLine m_addable;
		PendingPipeLine m_removable;

		Memory::RefPtr<GlobalResource> m_globalResource;

		wtr::DynamicArray<Memory::RefPtr<const MeshDrawCommand>> m_drawCommands;
	};
};

#endif // __WTR_RENDERGRAPH_H__