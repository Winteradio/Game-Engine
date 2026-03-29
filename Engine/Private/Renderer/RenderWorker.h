#ifndef __WTR_RENDERWORKER_H__
#define __WTR_RENDERWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Worker.h>

namespace wtr
{
	class Renderer;
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
		void SetRenderer(Memory::RefPtr<Renderer> renderer);
		void SetExecutor(const Memory::RefPtr<RHIExecutor> executor);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		Memory::RefPtr<Renderer>	m_refRenderer;
		Memory::RefPtr<RHIExecutor> m_refExecutor;
	};
};

#endif // __WTR_RENDERWORKER_H__