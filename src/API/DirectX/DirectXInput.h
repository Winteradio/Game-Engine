#ifndef __DIRECTXINPUT_H__
#define __DIRECTXINPUT_H__

#include "DirectXAPI.h"
#include "winuser.h"

class DirectXInput
{
	private :
		DirectXInput();
		~DirectXInput();

	public :
		static DirectXInput& GetHandle();
		void Init( HWND hWnd, int PosX, int PosY, int Width, int Height );
		void Update();
		void Destroy();

	private :
		static DirectXInput m_DirectXInput;

		int m_ScreenPosX;
		int m_ScreenPosY;
		int m_ScreenWidth;
		int m_ScreenHeight;

		HINSTANCE m_hInstance;
		IDirectInputDevice8* m_Keyboard;
		IDirectInputDevice8* m_Mouse;
		LPDIRECTINPUT8 m_Input;
};

#endif // __DIRECTXINPUT_H__