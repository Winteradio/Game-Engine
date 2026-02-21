#ifndef __WTR_PIPELINEGRAPH_H__
#define __WTR_PIPELINEGRAPH_H__

#include <ECS/include/Graph/Graph.h>
#include <ECS/include/UUID/UUID.h>
#include <Renderer/PipeLine/PipeLine.h>

namespace wtr
{
	class PipeLineGraph : public ECS::Graph<ECS::UUID, ECS::UUIDString>
	{
		public :
			PipeLineGraph() = default;
			~PipeLineGraph() = default;
	};
};

#endif // __WTR_PIPELINEGRAPH_H__