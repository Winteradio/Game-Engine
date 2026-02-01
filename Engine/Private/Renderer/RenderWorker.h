#ifndef __WTR_RENDERWORKER_H__
#define __WTR_RENDERWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Worker.h>

namespace wtr
{
	class FrameContext;
	class Renderer;
	class RHICommandExecutor;
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
		void SetRenderer(const Memory::RefPtr<Renderer> renderer);
		void SetExecutor(const Memory::RefPtr<RHICommandExecutor> executor);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		Memory::RefPtr<FrameContext> m_refFrameContext;
		Memory::RefPtr<Renderer> m_refRenderer;
		Memory::RefPtr<RHICommandExecutor> m_refExecutor;
	};
};

#endif // __WTR_RENDERWORKER_H__