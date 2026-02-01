#ifndef __WTR_PIPELINE_H__
#define __WTR_PIPELINE_H__

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class SceneView;
	class RHISystem;
	class RHIPipeLine;

	struct RHIPipeLineDesc;
};

namespace wtr
{
	namespace PipeLine
	{
		class Base
		{
		public :
			Base(Memory::RefPtr<RHISystem> system);
			virtual ~Base();

		public :
			virtual void Draw(const SceneView& scene) = 0;
			virtual void Init() = 0;

		protected :
			Memory::RefPtr<RHIPipeLine> m_pipeLine;
			Memory::RefPtr<RHISystem> m_system;
		};
	};
};

#endif // __WTR_PIPELINE_H__