#include "CoreEngine.h"

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	CoreEngine::GetHandle().Init();
	for ( auto Message : Log::GetMessage() )
	{
		std::cout << Message << std::endl;
	}
	CoreEngine::GetHandle().Start();

	return 0;
}
