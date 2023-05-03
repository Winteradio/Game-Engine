#include "Core.h"


Core::Core() {}

Core::~Core() {}

Core& Core::GetHandle()
{
	return m_Core;
}

void Core::Start()
{
	MSG msg;
	ZeroMemory( &msg, sizeof( MSG ) );
	while( true )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
			{
				break;
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			Update();
		}
	}
}

void Core::Update()
{

}

void Core::Init()
{
	InitAPI();
	InitManager();
}

void Core::InitManager()
{
	SceneManager::GetHandle().Init();
	NodeManager::GetHandle().Init();
	ComponentManager::GetHandle().Init();
	SystemManager::GetHandle().Init();
}

void Core::InitAPI()
{
	WinAPI::GetHandle().Init( "NewEngine", 200, 100, 1440, 840 );

	DirectXAPI::GetHandle().Init(
		WinAPI::GetHandle().GetWidth(),
		WinAPI::GetHandle().GetHeight(),
		WinAPI::GetHandle().GetWindowHandle()
		);

	ImGuiAPI::GetHandle().Init(
		WinAPI::GetHandle().GetWindowHandle(),
		DirectXAPI::GetHandle().GetDevice(),
		DirectXAPI::GetHandle().GetDeviceContext()
		);
}

void Core::Destroy()
{
	DirectXAPI::GetHandle().Destroy();
	ImGuiAPI::GetHandle().Destroy();
	WinAPI::GetHandle().Destroy();

	SceneManager::GetHandle().Destroy();
	NodeManager::GetHandle().Destroy();
	ComponentManager::GetHandle().Destroy();
	SystemManager::GetHandle().Destroy();
}

Core Core::m_Core;
