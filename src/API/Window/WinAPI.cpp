#include "WinAPI.h"
#include "ImGuiAPI.h"

WinAPI::WinAPI() {}
WinAPI::~WinAPI() {}

void WinAPI::Init( int PosX, int PosY, int Width, int Height )
{
	m_PosX = PosX;
	m_PosY = PosY;
	m_Width = Width;
	m_Height = Height;

	if ( m_TitleName.empty() )
	{
		m_TitleName = "Default Title";
	}

	if ( m_ClassName.empty() )
	{
		m_ClassName = "Default Class";
	}

	return Create();
}

void WinAPI::Init( const char* TitleName, int PosX, int PosY, int Width, int Height )
{
	m_TitleName = TitleName;

	return Init( PosX, PosY, Width, Height );
}

void WinAPI::Init( const char* ClassName, const char* TitleName, int PosX, int PosY, int Width, int Height )
{
	m_ClassName = ClassName;
	m_TitleName = TitleName;

	return Init( PosX, PosY, Width, Height );
}

void WinAPI::Create()
{
	// Get Instance for this Program
	m_hInstance = GetModuleHandle( NULL );

	// Create WNDCLASSED Structure
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = NULL;
	wcex.cbWndExtra = NULL;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wcex.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW+2 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_ClassName.c_str();

	// Register Window CLass
	if ( !RegisterClassEx( &wcex ) ) { Log::Error(" Failed - Register Wnd Class "); }
	else { Log::Info(" Successed - Register Wnd Class "); }

	// Make Rectangle for Window
	RECT ClientSize = { m_PosX, m_PosY, m_Width + m_PosX, m_Height + m_PosY };
	AdjustWindowRect( &ClientSize, WS_OVERLAPPEDWINDOW, FALSE );

	// Create Window
	m_hWnd = CreateWindowEx( NULL,
		m_ClassName.c_str(), m_TitleName.c_str(),
		WS_OVERLAPPEDWINDOW,
		ClientSize.left, ClientSize.top,
		ClientSize.right - ClientSize.left,
		ClientSize.bottom - ClientSize.top,
		nullptr, nullptr, m_hInstance, nullptr
		);

	if ( !m_hWnd ) { Log::Error(" Failed - Create Window "); }
	else { Log::Info(" Successed - Creat Window "); }

	// Show and Update Window
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
}

void WinAPI::Destroy()
{
	Log::Info("Destroy Window");

	DestroyWindow( m_hWnd );
	UnregisterClass( m_ClassName.c_str(), m_hInstance );

	m_TitleName.clear();
	m_hWnd = NULL;
	m_hInstance = NULL;
}

void WinAPI::Update()
{
	UpdateWindow( m_hWnd );
}

WinAPI& WinAPI::GetHandle() { return m_WinAPI; }
HWND& WinAPI::GetWindowHandle() { return m_hWnd; }
HINSTANCE& WinAPI::GetWindowHandleInstance() { return m_hInstance; }

LRESULT CALLBACK WinAPI::WndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
	if ( ImGui_ImplWin32_WndProcHandler( hWnd, Message, wParam, lParam ) )
	{
		return true;
 	}

	switch( Message )
	{
	case WM_KEYDOWN :
		if ( wParam == VK_ESCAPE )
		{
			DestroyWindow( hWnd );
			return 0;
		}
		else
		{
			return DefWindowProc( hWnd, Message, wParam, lParam );
		}
	case WM_DESTROY :
		PostQuitMessage( 0 );
		return 0;
	default :
		return DefWindowProc( hWnd, Message, wParam, lParam );
	}
}

WinAPI WinAPI::m_WinAPI;