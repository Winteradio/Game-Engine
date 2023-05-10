#include "Core.h"

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	Core::GetHandle().Init();

	for ( auto Message : Log::GetMessage() )
	{
		std::cout << Message << std::endl;
	}
	Core::GetHandle().Start();

	system("pause");
	return 0;
}
