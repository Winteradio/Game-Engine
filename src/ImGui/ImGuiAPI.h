#ifndef __IMGUIAPI_H__
#define __IMGUIAPI_H__

#include "ImGuiUtils.h"
#include "DirectXUtils.h"
#include "Log.h"

class ImGuiAPI
{
	private :
		ImGuiAPI();
		~ImGuiAPI();

	public :
		void Init( HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext );
		void Destroy();
		void Begin();
		void End();

		void SetFont();
		void SetStyle();

		static ImGuiAPI& GetHandle();
		ImGuiIO*& GetImGuiIO();

	private :
		static ImGuiAPI m_ImGuiAPI;
		ImGuiIO* m_ImGuiIO;
};

#endif // __IMGUIAPI_H__