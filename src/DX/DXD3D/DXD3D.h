#ifndef __DXD3D_H__
#define __DXD3D_H__

#include "DXCommon.h"

class DXD3D
{
	// Functions - Constructor and Destructor
	public :

		DXD3D();
		DXD3D( const DXD3D* );
		~DXD3D();


	// Functions - Basic Initialization, End and Action related Scene
	public :

		bool Init( int Width, int Height, HWND hWnd, float SCREEN_DEPTH, float SCREEN_NEAR );
		void Release();
		void InitPointer();

		void BeginScene( float Red, float Blue, float Green, float Alpha );
		void RenderEngine( RENDERSTATE* RenderState );
		void RenderInGame( RENDERSTATE* RenderState );
		void EndScene();


	// Functions - Divide Init Functions
	private :

		bool InitDeviceAndSwapChain( int Width, int Height, HWND hWnd );
		bool InitRenderTargetView();
		bool InitDepthStencilStateAndView( int Width, int Height );
		bool InitRasterizerState();
		void InitViewport( int Width, int Height );
		bool InitBlendState();
		void InitMatrix( int Width, int Height, float ScreenDepth, float ScreenNear );


	// Functions - Get private variables
	public :
		void ChangeDepthStencilState( DEPTHSTENCIL State );
		void ChangeBlendState( BLEND State );
		void ChangeRasterizerState( RASTERIZER State );

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		XMMATRIX GetProjectionMatrix( );
		XMMATRIX GetWorldMatrix( );
		XMMATRIX GetOrthoMatrix( );


	// Variables - Direct3D Objects
	private :

		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		IDXGISwapChain* m_SwapChain;

		ID3D11RenderTargetView* m_RenderTargetView;
		D3D11_VIEWPORT m_ViewportEngine;
		D3D11_VIEWPORT m_ViewportInGame;

		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11DepthStencilState* m_DepthEnable;
		ID3D11DepthStencilState* m_DepthDisable;

		ID3D11RasterizerState* m_RasterWireFrame;
		ID3D11RasterizerState* m_RasterCWCullMode;
		ID3D11RasterizerState* m_RasterCCWCullMode;
		ID3D11RasterizerState* m_RasterNoCullMode;

		ID3D11BlendState* m_BlendDisable;
		ID3D11BlendState* m_BlendAlphaEnable;
		ID3D11BlendState* m_BlendAlphaDisable;

		XMMATRIX m_ProjectionMatrix;
		XMMATRIX m_WorldMatrix;
		XMMATRIX m_OrthoMatrix;

		ID3D11DepthStencilState* m_FrameDepthStencilState;
		ID3D11RasterizerState* m_FrameRasterizerState;
		ID3D11BlendState* m_FrameBlendState;
};

#endif __DXD3D_H__