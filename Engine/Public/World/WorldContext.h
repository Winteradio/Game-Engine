#ifndef __WTR_WORLDCONTEXT_H__
#define __WTR_WORLDCONTEXT_H__

#include <Memory/include/Pointer/RootPtr.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <ECS/include/TimeStep.h>

#include <World/World.h>

namespace wtr
{
	class WorldCommandList;
	class ViewController;
	class PlayerController;
};

namespace wtr
{
	class WorldContext
	{
	public :
		Memory::RootPtr<World> world;
		Memory::RefPtr<ViewController> views;
		Memory::RefPtr<PlayerController> players;
		Memory::RefPtr<Commander> commander;
		
	public :
		WorldContext();
		~WorldContext();

	public :
		bool Init();
		void Clear();
		void Prepare();
		void Update(const ECS::TimeStep& timeStep);

	private :
		Memory::RefPtr<WorldCommandList> m_refCommandList;
	};
};

#endif // __WTR_WORLDCONTEXT_H__