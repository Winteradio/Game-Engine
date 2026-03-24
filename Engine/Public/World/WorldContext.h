#ifndef __WTR_WORLDCONTEXT_H__
#define __WTR_WORLDCONTEXT_H__

#include <Memory/include/Pointer/ObjectPtr.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class World;
	class ViewController;
	class Player;
};

namespace wtr
{
	struct WorldContext
	{
		Memory::ObjectPtr<World> world;
		Memory::RefPtr<ViewController> viewController;
		Memory::RefPtr<Player> player;
	};
};

#endif // __WTR_WORLDCONTEXT_H__