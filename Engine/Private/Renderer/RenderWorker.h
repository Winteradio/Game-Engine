#ifndef __WTR_RENDERWORKER_H__
#define __WTR_RENDERWORKER_H__

#include <Framework/Worker.h>
#include <Renderer/Renderer.h>
#include <RHI/RHISystem.h>

namespace wtr
{
	class RenderWorker : public Worker
	{
	public :
		RenderWorker();
		~RenderWorker();

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		Renderer m_Renderer;
	};
};

#endif // __WTR_RENDERWORKER_H__