#ifndef __WTR_WORLDCONTEXT_H__
#define __WTR_WORLDCONTEXT_H__

#include <Memory/include/Pointer/ObjectPtr.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <ECS/include/TimeStep.h>

namespace wtr
{
	class World;
	class WorldCommandList;
	class ViewController;
	class PlayerController;
};

namespace wtr
{
	class WorldContext
	{
	public :
		Memory::ObjectPtr<World> world;
		Memory::RefPtr<ViewController> view;
		Memory::RefPtr<PlayerController> player;

	public :
		WorldContext();
		~WorldContext();

	public :
		bool Init();
		void Prepare();
		void Update(const ECS::TimeStep& timeStep);

	private :
		Memory::RefPtr<WorldCommandList> m_refCommandList;
	};
};

#endif // __WTR_WORLDCONTEXT_H__