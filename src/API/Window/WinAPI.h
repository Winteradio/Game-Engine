#ifndef __WINAPI_H__
#define __WINAPI_H__

#include "IWindow.h"
#include "Log.h"

class WinAPI : public IWindow
{
	private :
		WinAPI();
		~WinAPI();

	public :
		void Init( const char* ClassName, const char* TitleName, int PosX, int PosY, int Width, int Height );
		void Init( const char* TitleName, int PosX, int PosY, int Width, int Height );
		void Init( int PosX, int PosY, int Width, int Height );

		void Create();
		void Destroy();
		void Update();

		static WinAPI& GetHandle();
		HWND& GetWindowHandle();
		HINSTANCE& GetWindowHandleInstance();

	private :
		static WinAPI m_WinAPI;
		static LRESULT CALLBACK WndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );

		HWND m_hWnd;
		HINSTANCE m_hInstance;
		std::string m_ClassName;
};

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT Message, WPARAM, LPARAM lParma );

#endif // __WINAPI_H__