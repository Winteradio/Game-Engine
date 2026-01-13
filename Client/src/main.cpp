#include <DemoApplication.h>
#include <Log/include/LogPlatform.h>

int MAIN()
{
	demo::Game game;
	game.onInit();
	game.onRun();
	game.onShutdown();

	system("pause");

	return 0;
}
