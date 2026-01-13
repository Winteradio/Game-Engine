#ifndef __WTR_WORLDWORKER_H__
#define __WTR_WORLDWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <ECS/include/TimeStep.h>
#include <Framework/Worker.h>
#include <functional>

namespace wtr
{
	class InputStorage;
	class FrameContext;
	class RenderCommandList;
};

namespace wtr
{
	class WorldWorker : public Worker
	{
	public :
		using UpdateFunc = std::function<void(const ECS::TimeStep&)>;
		using RenderFunc = std::function<void(RenderCommandList&)>;

		WorldWorker();
		~WorldWorker();

	public :
		void SetInputStorage(const Memory::RefPtr<InputStorage> inputStorage);
		void SetFrameContext(const Memory::RefPtr<FrameContext> frameContext);
		void SetFunction(const UpdateFunc func);
		void SetFunction(const RenderFunc func);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		ECS::TimeStep m_timeStep;

		Memory::RefPtr<InputStorage> m_refInputStorage;
		Memory::RefPtr<FrameContext> m_refFrameContext;

		UpdateFunc m_updateFunc;
		RenderFunc m_renderFunc;
	};
};

#endif // __WTR_WORLDWORKER_H__