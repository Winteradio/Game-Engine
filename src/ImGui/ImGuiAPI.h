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
		static ImGuiAPI& Get();
		void Init( HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext );
		void Destroy();
		void Begin();
		void End();

		void SetFont();
		void SetStyle();

		ImGuiIO*& GetImGuiIO();

	private :
		static ImGuiAPI m_ImGuiAPI;
		ImGuiIO* m_ImGuiIO;
};

#endif // __IMGUIAPI_H__