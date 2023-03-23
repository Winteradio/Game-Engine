#include "DirectXInput.h"

DirectXInput::DirectXInput()
{
	
}

DirectXInput::~DirectXInput()
{
	Destroy();
}

DirectXInput& DirectXInput::Get() { return m_DirectXInput; }

void DirectXInput::Init( HWND hWnd, int PosX, int PosY, int Width, int Height )
{
	HRESULT hr;

	m_ScreenPosX = PosX;
	m_ScreenPosY = PosY;
	m_ScreenWidth = Width;
	m_ScreenHeight = Height;

	hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_Input, NULL );
	if ( FAILED( hr ) ) { Log::Error(" Failed - Create DirectX Input "); }
	else { Log::Info(" Successed - Create DirectX Input "); }

	m_Input->CreateDevice( GUID_SysKeyboard, &m_Keyboard, NULL );
}

void DirectXInput::Destroy()
{
	
}

DirectXInput DirectXInput::m_DirectXInput;