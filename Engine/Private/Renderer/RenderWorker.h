#ifndef __WTR_RENDERWORKER_H__
#define __WTR_RENDERWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Worker.h>
#include <Renderer/RenderContext.h>
#include <Renderer/Renderer.h>
#include <RHI/RHISystem.h>

namespace wtr
{
	class RenderWorker : public Worker
	{
	public :
		RenderWorker();
		~RenderWorker();

	public :
		bool Init(const Memory::RefPtr<RenderContext> refRenderContext);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		Renderer m_renderer;
		RHISystem m_system;

		Memory::RefPtr<RenderContext> m_refRenderContext;
	};
};

#endif // __WTR_RENDERWORKER_H__