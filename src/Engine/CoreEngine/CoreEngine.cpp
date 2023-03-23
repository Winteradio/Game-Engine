#include "CoreEngine.h"

#include "RenderingEngine.h"
#include "PhysicsEngine.h"

#include "LayerManager.h"
#include "ResourceManager.h"

CoreEngine::CoreEngine() {}

CoreEngine::~CoreEngine() {}

CoreEngine& CoreEngine::Get()
{
	return m_CoreEngine;
}

void CoreEngine::Start()
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

void CoreEngine::Update()
{
	PhysicsEngine::Get().Update();

	RenderingEngine::Get().Begin();
	RenderingEngine::Get().Render();
	RenderingEngine::Get().End();
}

void CoreEngine::Init()
{
	InitAPI();
	InitOtherEngine();
	InitManager();
}

void CoreEngine::InitOtherEngine()
{
	RenderingEngine::Get().Init();
	PhysicsEngine::Get().Init();
}

void CoreEngine::InitManager()
{
	LayerManager::Get().Init( WinAPI::Get().GetWidth(), WinAPI::Get().GetHeight() );
	ResourceManager::Init();
}

void CoreEngine::InitAPI()
{
	WinAPI::Get().Init( 200, 100, 1440, 840 );
	DirectXAPI::Get().Init( WinAPI::Get().GetWidth(), WinAPI::Get().GetHeight(), WinAPI::Get().GetWindowHandle() );
	ImGuiAPI::Get().Init( WinAPI::Get().GetWindowHandle(), DirectXAPI::Get().GetDevice(), DirectXAPI::Get().GetDeviceContext() );
}

void CoreEngine::Destroy()
{
	DirectXAPI::Get().Destroy();
	WinAPI::Get().Destroy();
}

CoreEngine CoreEngine::m_CoreEngine;
