#pragma once

#ifndef __WIN_H__
#define __WIN_H__

#include <Windows.h>
#include <string.h>
#include "SUBLog.h"

class WIN
{
	// Constructor and Destructor
	public :
		WIN();
		WIN( const WIN& Other );
		~WIN();

	// Init
	public :
		bool Init();
		void Release();

		bool Frame();

		void SetWindowPosition( int PosX, int PosY );
		void SetWindowSize( int Width, int Height  );

		void SetWindowNameNTitle( char* Name, char* Title );
		void SetWindowNameNTitle( const char* Name, const char* Title );

		void SetWindowHandle( HWND hWnd );
		void SetWindowHandleInstance( HINSTANCE hInstance );

		int GetWindowPosX();
		int GetWindowPosY();
		int GetWindowWidth();
		int GetWindowHeight();

		HWND GetWindowHandle();
		HINSTANCE GetWindowHandleInstance();

	// Variable
	private :
		HWND m_hWnd;
		HINSTANCE m_hInstance;

		int m_PosX = 100;
		int m_PosY = 100;
		int m_Width = 1440;
		int m_Height = 840;
		RECT m_ClientSize;
		bool m_FullScreen = false;
		char* m_Name = "Default Name";
		char* m_Title = "Default Title";
};

#endif // __WIN_H__