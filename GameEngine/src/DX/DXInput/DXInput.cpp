#include "DXInput.h"

DXINPUT::DXINPUT()
{
	InitPointer();
}

DXINPUT::DXINPUT( const DXINPUT* Other )
{
	*this = *Other;
}

DXINPUT::~DXINPUT() {}

bool DXINPUT::Init( HWND hWnd, int posX, int posY, int screenWidth, int screenHeight )
{
	HRESULT hr;

	m_PhWnd = &hWnd;

	m_PosX = posX;
	m_PosY = posY;

	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	m_hInstance = GetModuleHandle( NULL );
	hr = DirectInput8Create( m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Direct Input ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Direct Input ");
	}

	hr = m_DirectInput->CreateDevice( GUID_SysKeyboard, &m_DIKeyboard, NULL );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Direct Input that is Keyboard ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Direct Input that is Keyboard ");
	}

	hr = m_DirectInput->CreateDevice( GUID_SysMouse, &m_DIMouse, NULL );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Direct Input that is Mouse ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Direct Input that is Mouse ");
	}

	hr = m_DIKeyboard->SetDataFormat( &c_dfDIKeyboard );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Set Data format for Keyboard ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Set Data format for Keyboard ");
	}

	hr = m_DIKeyboard->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Set cooperative level for Keyboard ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Set cooperative level for keyboard ");
	}

	hr = m_DIMouse->SetDataFormat( &c_dfDIMouse );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Set Data format for Mouse ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Set Data format for Mouse ");
	}

	hr = m_DIMouse->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE| DISCL_FOREGROUND );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Set cooperative level for Mouse ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Set cooperative level for Mouse ");
	}

	m_DIKeyboard->Acquire();
	m_DIMouse->Acquire();

	return true;
}

void DXINPUT::InitPointer()
{
	m_DIKeyboard = nullptr;
	m_DIMouse = nullptr;
	m_DirectInput = NULL;

	m_MouseX = 0;
	m_MouseY = 0;
}

void DXINPUT::Release()
{
	m_DIKeyboard->Unacquire();
	m_DIKeyboard->Release();
	m_DIMouse->Unacquire();
	m_DIMouse->Release();
	m_DirectInput->Release();

	InitPointer();
}

void DXINPUT::Frame( float& moveLeftRight, float& moveBackForward, float& Yaw, float& Pitch)
{
	HRESULT hr;
	DIMOUSESTATE mouseCurrState;

	hr = m_DIKeyboard->GetDeviceState( sizeof( m_KeyboardState ), (LPVOID)&m_KeyboardState );
	if ( FAILED( hr ) )
	{
		if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED )
		{
			m_DIKeyboard->Acquire();
		}
	}

	hr = m_DIMouse->GetDeviceState( sizeof( DIMOUSESTATE ), &mouseCurrState );
	if ( FAILED( hr ) )
	{
		if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED )
		{
			m_DIMouse->Acquire();
		}
	}

	POINT* Mouse = new POINT;
	GetCursorPos( Mouse );
	ScreenToClient( *m_PhWnd, Mouse );
	m_MouseX = Mouse->x - m_PosX;
	m_MouseY = Mouse->y - m_PosY;

	if ( m_MouseX < 0 ) { m_MouseX = 0; }
	if ( m_MouseY < 0 ) { m_MouseY = 0; }

	if ( m_MouseX > m_ScreenWidth ) { m_MouseX = m_ScreenWidth; }
	if ( m_MouseY > m_ScreenHeight ) { m_MouseY = m_ScreenHeight; }

    if( m_KeyboardState[DIK_A] & 0x80)
    {
    	moveLeftRight -= 0.003f;
    }
    if( m_KeyboardState[DIK_D] & 0x80)
    {
    	moveLeftRight += 0.003f;
    }
    if( m_KeyboardState[DIK_W] & 0x80)
    {
        moveBackForward += 0.003f;
    }
    if( m_KeyboardState[DIK_S] & 0x80)
    {
        moveBackForward -= 0.003f;
    }

    if ( mouseCurrState.rgbButtons[0] & 0x80 )
    {
    	if( (mouseCurrState.lX != m_MouseLastState.lX) || (mouseCurrState.lY != m_MouseLastState.lY) )
	    {
	        Yaw += mouseCurrState.lX * 0.003f;

	        Pitch += mouseCurrState.lY * 0.003f;

	        m_MouseLastState = mouseCurrState;
	    }
    }

    delete Mouse;
}