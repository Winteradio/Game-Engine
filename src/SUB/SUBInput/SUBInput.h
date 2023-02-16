#ifndef __SUBINPUT_H__
#define __SUBINPUT_H__

#include <Windows.h>
#include <cmath>

struct MOUSEINFO
{
	int PosX = 0;
	int PosY = 0;
	bool LeftButton = false;;
	bool RightButton = false;;
	bool Wheel = false;
	bool Frame = true;
};

class SUBINPUT
{
	public :

		SUBINPUT();
		SUBINPUT( const SUBINPUT* );
		~SUBINPUT();

	public :

		bool Init( int ,int );

		MOUSEINFO* GetMouse();
		LRESULT CALLBACK MessageHandler( HWND, UINT, WPARAM, LPARAM );

	private :

		void SetMousePosition( int ScreenWidth, int ScreenHeight );
		void SetMouseLeftState( bool ButtonState );
		void SetMouseRightState( bool ButtonState );
		void SetMouseWheelState( bool ButtonState );

	private :

		unsigned char m_KeyboardState[256];

	public :
		int m_ScreenWidth = 200;
		int m_ScreenHeight = 200;
		MOUSEINFO* m_Mouse;
};

#endif