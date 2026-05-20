#ifndef __WTR_RENDERGRAPH_H__
#define __WTR_RENDERGRAPH_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <ECS/include/Graph/Graph.h>
#include <Renderer/RenderTypes.h>
#include <Renderer/RenderPass/RenderPass.h>

namespace wtr
{
	struct MeshDrawCommand;
	struct RenderView;

	class LightProxy;
	class RenderScene;
	class RHICommandList;
};

namespace wtr
{
	class RenderGraph : public ECS::Graph<RenderPass>
	{
	public :
		using Super = ECS::Graph<RenderPass>;
		using PendingPipeLine = wtr::HashSet<Memory::RefPtr<RenderPass>, RenderPassHasher>;

		RenderGraph();
		~RenderGraph();

	public :
		bool Init(Memory::RefPtr<RHICommandList> cmdList);
		void RemoveAll(Memory::RefPtr<RHICommandList> cmdList);
		void Flush(Memory::RefPtr<RHICommandList> cmdList);

		void Execute(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<RenderScene> renderScene, const RenderView& renderView);

	private :
		void Add(Memory::RefPtr<RenderPass> pipeline);
		void Remove(Memory::RefPtr<RenderPass> pipeline);

		void FlushAddable(Memory::RefPtr<RHICommandList> cmdList);
		void FlushRemovable(Memory::RefPtr<RHICommandList> cmdList);

	private :
		PendingPipeLine m_addable;
		PendingPipeLine m_removable;

		wtr::DynamicArray<Memory::RefPtr<const MeshDrawCommand>> m_drawCommands;
		wtr::DynamicArray<Memory::RefPtr<const LightProxy>> m_lightProxies;
	};
};

#endif // __WTR_RENDERGRAPH_H__