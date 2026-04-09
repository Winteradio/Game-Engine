#ifndef __WTR_RENDERWORKER_H__
#define __WTR_RENDERWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Worker.h>

namespace wtr
{
	class Renderer;
	class RHIExecutor;

	class FrameConsumer;
	class FrameProducer;
};

namespace wtr
{
	class RenderWorker : public Worker
	{
	public :
		RenderWorker();
		~RenderWorker();

	public :
		void SetRenderer(Memory::RefPtr<Renderer> renderer);
		void SetExecutor(const Memory::RefPtr<RHIExecutor> executor);
		void SetConsumer(const Memory::RefPtr<FrameConsumer> consumer);
		void SetProducer(const Memory::RefPtr<FrameProducer> producer);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onNotify() override;

	private :
		Memory::RefPtr<Renderer>	m_refRenderer;
		Memory::RefPtr<RHIExecutor> m_refExecutor;
		Memory::RefPtr<FrameConsumer> m_refConsumer;
		Memory::RefPtr<FrameProducer> m_refProducer;
	};
};

#endif // __WTR_RENDERWORKER_H__