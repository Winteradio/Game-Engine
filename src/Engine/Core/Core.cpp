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
	Physics::Get().Update();

	Rendering::Get().Begin();
	Rendering::Get().Render();
	Rendering::Get().End();
}

void Core::Init()
{
	InitAPI();
	InitManager();
}

void Core::InitManager()
{
}

void Core::InitAPI()
{
	WinAPI::GetHandle().Init( 200, 100, 1440, 840 );
	DirectXAPI::GetHandle().Init( WinAPI::GetHandle().GetWidth(), WinAPI::GetHandle().GetHeight(), WinAPI::GetHandle().GetWindowHandle() );
	ImGuiAPI::GetHandle().Init( WinAPI::GetHandle().GetWindowHandle(), DirectXAPI::GetHandle().GetDevice(), DirectXAPI::GetHandle().GetDeviceContext() );
}

void Core::Destroy()
{
	DirectXAPI::GetHandle().Destroy();
	WinAPI::GetHandle().Destroy();
}

Core Core::m_Core;
