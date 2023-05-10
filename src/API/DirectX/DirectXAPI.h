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
		static DirectXAPI& GetHandle();
		void Init( int Width, int Height, HWND hWnd );
		void Destroy();
		void Begin();
		void End();
		
	public :
		void CreateDeviceAndSwapChain( int Width, int Height, HWND hWnd );

		void CreateRenderTargetView();

		void CreateDepthStencilBuffer();
		void CreateDepthStencilView();
		void CreateDepthStencilEnable();
		void CreateDepthStencilDisable();

		void CreateRasterWireFrame();
		void CreateRasterCWCull();
		void CreateRasterCCWCull();
		void CreateRasterNoCull();

		void CreateBlendDisable();
		void CreateBlendAlphaEnable();
		void CreateBlendAlphaDisable();

	public :
		ID3D11Device*& GetDevice();
		ID3D11DeviceContext*& GetDeviceContext();
		IDXGISwapChain*& GetSwapChain();

		ID3D11RenderTargetView*& GetRenderTargetView();
		ID3D11Texture2D*& GetDepthStencilBuffer();
		ID3D11DepthStencilView*& GetDepthStencilView();
		ID3D11DepthStencilState*& GetDepthStencilState();
		ID3D11RasterizerState*& GetRasterizerState();
		ID3D11BlendState*& GetBlendState();

	private :
		static DirectXAPI m_DirectXAPI;

		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		IDXGISwapChain* m_SwapChain;

		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11DepthStencilState* m_DepthStencilState;
		ID3D11RasterizerState* m_RasterizerState;
		ID3D11BlendState* m_BlendState;
};

#endif // __DIRECTXAPI_H__