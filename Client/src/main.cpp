#include <DemoApplication.h>
#include <Log/include/LogPlatform.h>

#include <Memory/include/Core.h>

#ifdef DEBUG_MEMORY
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

int MAIN()
{
#ifdef DEBUG_MEMORY
	_CrtSetBreakAlloc(160);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	{
		demo::Game game;
		game.onInit();
		game.onRun();
	}

	return 0;
}
