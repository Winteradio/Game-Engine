#ifndef __WTR_PIPELINE_H__
#define __WTR_PIPELINE_H__

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class SceneView;
	class RHIPipeLine;
	class RHICommandList;
};

namespace wtr
{
	class PipeLine
	{
		public :
			PipeLine();
			virtual ~PipeLine();

		public :
			virtual void Draw(const SceneView& scene, Memory::RefPtr<RHICommandList> commandList) = 0;
			virtual void Init(Memory::RefPtr<RHICommandList> commandList) = 0;

		protected :
			Memory::RefPtr<RHIPipeLine> m_pipeLine;
	};
};

#endif // __WTR_PIPELINE_H__