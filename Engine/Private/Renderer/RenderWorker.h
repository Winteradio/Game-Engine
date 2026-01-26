#ifndef __WTR_RENDERWORKER_H__
#define __WTR_RENDERWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Worker.h>
#include <Framework/FrameContext.h>

namespace wtr
{

	class RenderWorker : public Worker
	{
	public :
		using RenderFunc = std::function<void(FrameView&)>;

		RenderWorker();
		~RenderWorker();

	public :
		void SetFrameContext(const Memory::RefPtr<FrameContext> frameContext);
		void SetFunction(const RenderFunc func);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		Memory::RefPtr<FrameContext> m_refFrameContext;

		RenderFunc m_renderFunc;
	};
};

#endif // __WTR_RENDERWORKER_H__