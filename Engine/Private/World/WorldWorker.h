#ifndef __WTR_WORLDWORKER_H__
#define __WTR_WORLDWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Framework/Worker.h>
#include <Framework/Input/InputStorage.h>
#include <Renderer/RenderContext.h>

namespace wtr
{
	class WorldWorker : public Worker
	{
	public :
		WorldWorker();
		~WorldWorker();

	public :
		bool Init(const Memory::RefPtr<InputStorage> refInputStorage, const Memory::RefPtr<RenderContext> refRenderContext);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		Memory::RefPtr<InputStorage> m_refInputStorage;
		Memory::RefPtr<RenderContext> m_refRenderContext;
	};
};

#endif // __WTR_WORLDWORKER_H__