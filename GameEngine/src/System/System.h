#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <Windows.h>

#include "WIN.h"
#include "DXEngine.h"
#include "SUBCommon.h"

class SYSTEM
{
	// Functions - Constructor and Destructor
	public :

		SYSTEM();
		SYSTEM( const SYSTEM* );
		~SYSTEM();

	// Functions - Running and Ending(Done)
	public :

		void Run();
		void Release();

	// Functions - Basic Initialization and Run Frame
	private :

		bool Init();
		bool Frame();
		bool InitWindows();
		bool InitDXENGINE();

		bool InitSUBINPUT();
		bool InitSUBFPS();
		bool InitSUBTIMER();

		void InitPointer();

	// Variables
	public :
		WIN* m_WIN;
		DXENGINE* m_DXENGINE;

		SUBFPS* m_SUBFPS;
		SUBINPUT* m_SUBINPUT;
		SUBTIMER* m_SUBTIMER;

		int m_PosX;
		int m_PosY;
		int m_Width;
		int m_Height;
};

static SUBINPUT* g_SUBINPUT = 0;

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT message, WPARAM, LPARAM lParma );
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

#endif __SYSTEM_H__