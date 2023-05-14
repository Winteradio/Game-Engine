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
	DirectXAPI::GetHandle().Begin();
	ImGuiAPI::GetHandle().Begin();

	FileBrowserUIHandler::GetHandle().Update( 0.0f );
	LogUIHandler::GetHandle().Update( 0.0f );
	MainUIHandler::GetHandle().Update( 0.0f );
	MenuBarUIHandler::GetHandle().Update( 0.0f );
	PropertyUIHandler::GetHandle().Update( 0.0f );
	StateUIHandler::GetHandle().Update( 0.0f );

	ImGui::ShowDemoWindow();

	ImGuiAPI::GetHandle().End();
	DirectXAPI::GetHandle().End();
}

void Core::Init()
{
	InitAPI();
	InitHandler();
	InitManager();
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

void Core::InitHandler()
{
	AssetHandler::GetHandle().Init();
	FileHandler::GetHandle().Init();

	FileBrowserUIHandler::GetHandle().Init( 200, 200, 800, 400 );
	MenuBarUIHandler::GetHandle().Init( 0, 0, 1440, 20 );
	MainUIHandler::GetHandle().Init( 0, 20, 300, 840 - 20 );
	PropertyUIHandler::GetHandle().Init( 1440 - 300, 20, 300, 840 - 20 );
	StateUIHandler::GetHandle().Init( 300, 20, 1440 - 600, 30 );
	LogUIHandler::GetHandle().Init( 300, 600, 1440 - 600, 240 );
}

void Core::InitManager()
{
	ComponentManager::GetHandle().Init();
	EntityManager::GetHandle().Init();
	NodeManager::GetHandle().Init();
	SceneManager::GetHandle().Init();
	SystemManager::GetHandle().Init();
}

void Core::Destroy()
{
	DirectXAPI::GetHandle().Destroy();
	ImGuiAPI::GetHandle().Destroy();
	WinAPI::GetHandle().Destroy();

	AssetHandler::GetHandle().Destroy();
	FileHandler::GetHandle().Destroy();

	FileBrowserUIHandler::GetHandle().Destroy();
	MainUIHandler::GetHandle().Destroy();
	MenuBarUIHandler::GetHandle().Destroy();
	LogUIHandler::GetHandle().Destroy();
	PropertyUIHandler::GetHandle().Destroy();
	StateUIHandler::GetHandle().Destroy();

	ComponentManager::GetHandle().Destroy();
	EntityManager::GetHandle().Destroy();
	NodeManager::GetHandle().Destroy();
	SceneManager::GetHandle().Destroy();
	SystemManager::GetHandle().Destroy();
}

Core Core::m_Core;
