#ifndef __DXIMGUI_H__
#define __DXIMGUI_H__

#include "DXImGUICommon.h"
#include "DXCommon.h"
#include <string>
#include "DXWindow.h"
#include "DIFileBrowser.h"

class DXIMGUI
{
	public : 
		DXIMGUI();
		DXIMGUI( const DXIMGUI& Other );
		~DXIMGUI();

	public :
		bool Init( DXRECTWINDOW* UserWindow, DXRECTWINDOW* InfoWindow, HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext );
		bool InitFileBrowser();
		bool Frame();
		bool Render( RENDERSTATE* RenderState, int MouseX, int MouseY );

		void Release();
		void InitPointer();

		void SetFonts( char* Path );

		DXRECTWINDOW* m_DXUserWindow;
		DXRECTWINDOW* m_DXInfoWindow;

		DIFILEBROWSER* m_DIFileBrowser;
	private :

		ImGuiIO* m_ImGuiIO;
		char m_FontAddr[MAXLENGTH];
		char* m_DefaultFont;
		int m_FontSize;
		bool m_FileBrowser = false;
};

#endif // __DXIMGUI_H__