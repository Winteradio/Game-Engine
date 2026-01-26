#ifndef __WTR_PIPELINEGRAPH_H__
#define __WTR_PIPELINEGRAPH_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <string>

namespace wtr
{
	class SceneView;
	class RHIPipeLine;
	class RHIShader;
};

namespace wtr
{
	class PipeLine
	{
		public :
			PipeLine();
			~PipeLine();

		public :
			void Draw(const SceneView& scene);

			static std::string toString(const PipeLine& pipeline);
		
		private :
			std::string m_name;
	};
};

#endif // __WTR_PIPELINEGRAPH_H