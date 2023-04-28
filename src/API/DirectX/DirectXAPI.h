#ifndef __DIRECTXAPI_H__
#define __DIRECTXAPI_H__

#include "DirectXUtils.h"
#include "IWindow.h"

class DirectXAPI : public IWindow
{
	private :
		DirectXAPI();
		~DirectXAPI();

	public :
		void Init( int Width, int Height, HWND hWnd );
		void Destroy();

		static DirectXAPI& GetHandle();
		ID3D11Device*& GetDevice();
		ID3D11DeviceContext*& GetDeviceContext();
		IDXGISwapChain*& GetSwapChain();

	private :
		static DirectXAPI m_DirectXAPI;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		IDXGISwapChain* m_SwapChain = nullptr;
};

#endif // __DIRECTXAPI_H__