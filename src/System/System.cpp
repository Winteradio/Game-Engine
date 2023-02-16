#include "System.h"


SYSTEM::SYSTEM()
{
	InitPointer();

	if ( !Init() )
	{
		LOG_ERROR(" Failed - Init System Object ");
		system("pause");
	}
	else
	{
		LOG_INFO(" Succssed - Init System Object");
	}
}


SYSTEM::SYSTEM(const SYSTEM* Other)
{
	*this = *Other;
}


SYSTEM::~SYSTEM(){}


bool SYSTEM::Init()
{
	// Windows Init
	if ( !InitWindows() ) { return false; }
	// SUBFPS Init
	if ( !InitSUBFPS() ) { return false; }
	// SUBINPUT Init
	if ( !InitSUBINPUT() ) { return false; }
	// SUBTIMER Init
	if ( !InitSUBTIMER() ) { return false; }
	// DXENGINE Init
	if ( !InitDXENGINE() ) { return false; }

	return true;
}


void SYSTEM::Run()
{
	// Message Structor Create and Init
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Main Loop
	while(true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!Frame())
			{
				LOG_INFO(" Error during Run");
				break;
			}
		}
	}
}


void SYSTEM::Release()
{
	LOG_INFO(" Release - System ");

	m_WIN->Release();
	delete m_WIN;

	delete m_SUBFPS;
	delete m_SUBINPUT;
	delete m_SUBTIMER;

	m_DXENGINE->Release();
	delete m_DXENGINE;

	InitPointer();

	LOG_INFO(" End - Program");
}


bool SYSTEM::Frame()
{
	// Update Sub System
	m_SUBFPS->Frame();
	m_SUBTIMER->Frame();

	// Update Mouse Position
	int mouseX = m_SUBINPUT->GetMouse()->PosX;
	int mouseY = m_SUBINPUT->GetMouse()->PosY;

	if ( !m_DXENGINE->Frame( m_SUBFPS->GetFPS(), 100, m_SUBTIMER->GetTime(), mouseX, mouseY ) )
	{
		LOG_ERROR(" Failed - Frame DXENGINE ");
		return false;
	}

	return true;
}


bool SYSTEM::InitWindows()
{
	// Create WIN Object;
	m_WIN = new WIN;
	if ( !m_WIN )
	{
		LOG_ERROR(" Failed - Create WIN Object ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create WIN Object ");
	}

	if ( !m_WIN->Init() )
	{
		LOG_ERROR(" Failed - Init WIN Object ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init WIN Object ");
	}
	return true;
}


bool SYSTEM::InitSUBINPUT()
{
	// Create SUBINPUT Objects
	g_SUBINPUT = new SUBINPUT;
	if ( !g_SUBINPUT )
	{
		LOG_ERROR(" Failed - Create SUBINPUT ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create SUBINPUT ");
	}

	// Init INPUT Objects
	if ( !g_SUBINPUT->Init(
		m_WIN->GetWindowWidth(), m_WIN->GetWindowHeight() ) )
	{
		LOG_ERROR(" Failed - Init SUBINPUT ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init SUBINPUT ");
	}

	m_SUBINPUT = g_SUBINPUT;

	return true;
}

bool SYSTEM::InitSUBFPS()
{
	// Create SUBFPS Object
	m_SUBFPS = new SUBFPS;
	if ( !m_SUBFPS )
	{
		LOG_ERROR(" Failed - Create SUBFPS ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create SUBFPS ");
	}

	// Init SUBFPS Object
	if ( !m_SUBFPS->Init() )
	{
		LOG_ERROR(" Failed - Init SUBFPS ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init SUBFPS ");
	}

	return true;
}


bool SYSTEM::InitSUBTIMER()
{
	// Create SUBTIMER Object
	m_SUBTIMER = new SUBTIMER;
	if ( !m_SUBTIMER )
	{
		LOG_ERROR(" Failed - Create SUBTIMER ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create SUBTIMER ");
	}

	// Init SUBTIMER Object
	if ( !m_SUBTIMER->Init() )
	{
		LOG_ERROR(" Failed - Init SUBTIMER ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init SUBTIMER ");
	}

	return true;
}


bool SYSTEM::InitDXENGINE()
{
	// Create DXENGINE Objects
	m_DXENGINE = new DXENGINE;
	if ( !m_DXENGINE )
	{
		LOG_ERROR(" Failed - Create DXENGINE ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create DXENGINE ");
	}

	// Init DXENGINE Objects
	if ( !m_DXENGINE->Init( m_WIN->GetWindowPosX(), m_WIN->GetWindowPosY(),
		m_WIN->GetWindowWidth(), m_WIN->GetWindowHeight(), m_WIN->GetWindowHandle() ) )
	{
		LOG_ERROR(" Failed - Init DXENGINE ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXENGINE ");
	}

	return true;
}

void SYSTEM::InitPointer()
{
	m_WIN = nullptr;
	m_DXENGINE = nullptr;

	m_SUBFPS = nullptr;
	m_SUBINPUT = nullptr;
	m_SUBTIMER = nullptr;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( ImGui_ImplWin32_WndProcHandler( hWnd, message, wParam, lParam ) )
	{
		return true;
 	}

	switch( message )
	{
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE ){
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc( hWnd, message, wParam, lParam );
}



