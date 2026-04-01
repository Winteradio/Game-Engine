#ifndef __WTR_RENDERGRAPH_H__
#define __WTR_RENDERGRAPH_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <ECS/include/Graph/Graph.h>
#include <Renderer/RenderTypes.h>
#include <Renderer/PipeLine/PipeLine.h>

namespace wtr
{
	class RenderScene;
	class RenderView;
	class RHICommandList;
};

namespace wtr
{
	struct PipeLineString
	{
		std::string operator()(const Memory::RefPtr<PipeLine>& pipeLine) const
		{
			if (!pipeLine)
			{
				return {};
			}

			const Reflection::TypeInfo* typeInfo = pipeLine->GetTypeInfo();
			return typeInfo->GetTypeName();
		}
	};

	struct PipeLineHasher
	{
		size_t operator()(const Memory::RefPtr<PipeLine>& pipeLine) const
		{
			if (pipeLine)
			{
				const Reflection::TypeInfo* typeInfo = pipeLine->GetTypeInfo();
				return typeInfo->GetTypeHash();
			}
			else
			{
				return 0;
			}
		}
	};

	class RenderGraph
	{
	public :
		using GraphType = ECS::Graph<Memory::RefPtr<PipeLine>, PipeLineString, PipeLineHasher>;

		RenderGraph();
		~RenderGraph();

	public :
		bool Init();
		void Add(Memory::RefPtr<PipeLine> pipeline);
		void Remove(Memory::RefPtr<PipeLine> pipeline);

		void Execute(Memory::RefPtr<RHICommandList> cmdList, Memory::RefPtr<RenderScene>& renderScene, const RenderView& renderView);

	private :
		GraphType m_graph;
	};
};

#endif // __WTR_RENDERGRAPH_H__