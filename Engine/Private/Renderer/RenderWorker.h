#ifndef __WTR_RENDERWORKER_H__
#define __WTR_RENDERWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Worker.h>

namespace wtr
{
	class FrameContext;
	class RenderGraph;
	class RHIExecutor;
};

namespace wtr
{
	class RenderWorker : public Worker
	{
	public :
		RenderWorker();
		~RenderWorker();

	public :
		void SetFrameContext(const Memory::RefPtr<FrameContext> frameContext);
		void SetGraph(const Memory::RefPtr<RenderGraph> graph);
		void SetExecutor(const Memory::RefPtr<RHIExecutor> executor);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		Memory::RefPtr<FrameContext> m_refFrameContext;
		Memory::RefPtr<RenderGraph> m_refGraph;
		Memory::RefPtr<RHIExecutor> m_refExecutor;
	};
};

#endif // __WTR_RENDERWORKER_H__