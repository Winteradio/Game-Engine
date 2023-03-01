#ifndef __DXIMGUI_H__
#define __DXIMGUI_H__

#include "DXImGUICommon.h"
#include "DXCommon.h"
#include <string>
#include "Readtext.h"

class DXIMGUI
{
	public : 
		DXIMGUI();
		DXIMGUI( const DXIMGUI& Other );
		~DXIMGUI();

	public :
		bool Init( HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext );
		bool Frame();
		bool Render( RENDERSTATE* RenderState, int MouseX, int MouseY );

		void Release();
		void InitPointer();

	private :
};

#endif // __DXIMGUI_H__