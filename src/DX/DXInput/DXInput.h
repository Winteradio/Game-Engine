#ifndef __DXINPUT_H__
#define __DXINPUT_H__

#include "DXCommon.h"
#include "winuser.h"

class DXINPUT
{
	public :
		DXINPUT();
		DXINPUT( const DXINPUT* Other );
		~DXINPUT();	

	public :
		bool Init( HWND hWnd, int PosX, int PosY, int screenWidth, int screenHeight );
		void Frame( float& moveLeftRight, float& moveBackForward, float& Yaw, float& Picth);

	private :
		void InitPointer();
		void Release();

	public :
		int m_MouseX;
		int m_MouseY;

		int m_PosX;
		int m_PosY;
		int m_ScreenWidth;
		int m_ScreenHeight;

	private :
		HINSTANCE m_hInstance;

		IDirectInputDevice8* m_DIKeyboard;
		IDirectInputDevice8* m_DIMouse;

		DIMOUSESTATE m_MouseLastState;
		LPDIRECTINPUT8 m_DirectInput;

		unsigned char m_KeyboardState[256];

		HWND* m_PhWnd;
};

#endif // __DXINPUT_H__