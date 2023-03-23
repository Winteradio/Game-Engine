#if WIN32
    #define YAML_CPP_STATIC_DEFINE
#endif

#include "yaml-cpp/dll.h"
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/Parser.h"

#include "CoreEngine.h"

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	CoreEngine::Get().Init();
	for ( auto Message : Log::GetMessage() )
	{
		std::cout << Message << std::endl;
	}
	CoreEngine::Get().Start();
	return 0;
}