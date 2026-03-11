#ifndef __WTR_WORLDWORKER_H__
#define __WTR_WORLDWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Memory/include/Pointer/ObjectPtr.h>
#include <ECS/include/TimeStep.h>
#include <Framework/Worker.h>
#include <World/World.h>

namespace wtr
{
	class InputStorage;
	class FrameContext;
};

namespace wtr
{
	class WorldWorker : public Worker
	{
	public :
		WorldWorker();
		~WorldWorker();

	public :
		void SetInputStorage(const Memory::RefPtr<InputStorage> inputStorage);
		void SetFrameContext(const Memory::RefPtr<FrameContext> frameContext);
		void SetWorld(const Memory::ObjectPtr<World> world);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		ECS::TimeStep m_timeStep;

		Memory::RefPtr<InputStorage> m_refInputStorage;
		Memory::RefPtr<FrameContext> m_refFrameContext;
		Memory::ObjectPtr<World> m_world;
	};
};

#endif // __WTR_WORLDWORKER_H__