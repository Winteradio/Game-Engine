#include "WIN.h"
#include "System.h"

WIN::WIN(){}

WIN::WIN( const WIN& Other )
{
	m_hWnd = Other.m_hWnd;
	m_hInstance = Other.m_hInstance;
}

WIN::~WIN(){}

bool WIN::Init()
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
	wcex.lpszClassName = m_Name;

	// Register Window CLass
	if ( !RegisterClassEx( &wcex ) )
	{
		LOG_ERROR(" Failed - Register Wnd Class ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Register Wnd Class ");
	}

	// Make Rectangle for Window
	m_ClientSize = { m_PosX, m_PosY, m_Width + m_PosX, m_Height + m_PosY };
	AdjustWindowRect( &m_ClientSize, WS_OVERLAPPEDWINDOW, FALSE );

	// Create Window
	m_hWnd = CreateWindowEx( NULL,
		m_Name, m_Title,
		WS_OVERLAPPEDWINDOW,
		m_ClientSize.left, m_ClientSize.top,
		m_ClientSize.right - m_ClientSize.left,
		m_ClientSize.bottom - m_ClientSize.top,
		nullptr, nullptr, m_hInstance, nullptr
		);

	if ( !m_hWnd )
	{
		LOG_ERROR(" Failed - Create Window ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Window ");
	}

	// Show and Update Window
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );


	LOG_INFO(" Successed - Init WIN Objects");

	return true;
}

void WIN::Release()
{
	LOG_INFO(" Release - WIN ");
	// Delete Windows
	DestroyWindow( m_hWnd );
	m_hWnd = NULL;

	// Delete Instance for this Program
	UnregisterClass( m_Name, m_hInstance );
	m_hInstance = NULL;

	// Delete WININFO
	m_Name = nullptr; // char* "Default Name" is not Heap Data
	m_Title = nullptr; // char* "Default Title" is not Heap Data
}

bool WIN::Frame()
{
	UpdateWindow( m_hWnd );

	return true;
}

void WIN::SetWindowPosition( int PosX, int PosY )
{
	m_PosX = PosX;
	m_PosY = PosY;
}

void WIN::SetWindowSize( int Width, int Height )
{
	m_Width = Width;
	m_Height = Height;
}

void WIN::SetWindowNameNTitle( char* Name, char* Title )
{
	m_Name = Name;
	m_Title = Title;
}

void WIN::SetWindowNameNTitle( const char* Name, const char* Title )
{
	m_Name = (char*)Name;
	m_Title = (char*)Title;
}

void WIN::SetWindowHandle( HWND hWnd ){ m_hWnd = hWnd; }
void WIN::SetWindowHandleInstance( HINSTANCE hInstance ){ m_hInstance = hInstance; }

int WIN::GetWindowPosX() { return m_PosX; }
int WIN::GetWindowPosY() { return m_PosY; }
int WIN::GetWindowWidth() { return m_Width; }
int WIN::GetWindowHeight() { return m_Height; }

HWND WIN::GetWindowHandle(){ return m_hWnd; }
HINSTANCE WIN::GetWindowHandleInstance(){ return m_hInstance; }