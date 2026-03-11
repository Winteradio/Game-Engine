#include <DemoApplication.h>
#include <Log/include/LogPlatform.h>

#include <Memory/include/Core.h>

int MAIN()
{
	demo::Game game;
	game.onInit();
	game.onRun();
	game.onShutdown();

	system("pause");

	return 0;
}
