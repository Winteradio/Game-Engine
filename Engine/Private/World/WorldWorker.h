#ifndef __WTR_WORLDWORKER_H__
#define __WTR_WORLDWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <ECS/include/TimeStep.h>
#include <Framework/Worker.h>

namespace wtr
{
	class InputStorage;
	class WorldContext;

	class ViewController;
	class ViewInfo;
	class FrameView;

	class Player;
	class PlayerController;
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
		void SetWorldContext(const Memory::RefPtr<WorldContext> worldContext);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		void UpdateView(Memory::RefPtr<PlayerController> playerController, Memory::RefPtr<ViewController> viewController);
		FrameView MakeView(Memory::RefPtr<Player> player, Memory::RefPtr<ViewInfo> view);
		
	private :
		ECS::TimeStep m_timeStep;

		Memory::RefPtr<InputStorage>	m_refInputStorage;
		Memory::RefPtr<WorldContext>	m_refWorldContext;

		wtr::DynamicArray<FrameView>	m_frameViews;
	};
};

#endif // __WTR_WORLDWORKER_H__