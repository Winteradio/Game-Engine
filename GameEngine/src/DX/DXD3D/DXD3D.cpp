#include "DXD3D.h"

/////////////////////////////////////////////
/*
@Function : Constructor & Destructor
@Usage : Create DXD3D, Destory DXD3D and Copy Other DXD3D
*/
/////////////////////////////////////////////
DXD3D::DXD3D()
{
	InitPointer();
}


DXD3D::DXD3D( const DXD3D* Other )
{
	*this = *Other;
}


DXD3D::~DXD3D()
{}
/////////////////////////////////////////////


/////////////////////////////////////////////
/*
@Function : Init Functions for DirectX11 setting
@Usage : Init Device and SwapChain, Create other status for rendering
*/
/////////////////////////////////////////////
bool DXD3D::Init( int Width, int Height, DXRECTWINDOW* EngineWindow, DXRECTWINDOW* InGameWindow, HWND hWnd, float SCREEN_DEPTH, float SCREEN_NEAR )
{
	if ( !InitDeviceAndSwapChain( Width, Height, hWnd ) ) { return false; }
	if ( !InitRenderTargetView() ) { return false; }
	if ( !InitDepthStencilStateAndView( Width, Height ) ) { return false; }
	if ( !InitRasterizerState() ) { return false; }
	if ( !InitBlendState() ) { return false; }
	InitViewport( EngineWindow, InGameWindow );
	InitMatrix( EngineWindow, InGameWindow, SCREEN_DEPTH, SCREEN_NEAR );

	return true;
}

void DXD3D::Release()
{
	m_Device->Release();
	m_DeviceContext->Release();
	m_SwapChain->Release();

	m_RenderTargetView->Release();

	m_DepthStencilBuffer->Release();
	m_DepthStencilView->Release();
	m_DepthEnable->Release();
	m_DepthDisable->Release();

	m_RasterWireFrame->Release();
	m_RasterCWCullMode->Release();
	m_RasterCCWCullMode->Release();
	m_RasterNoCullMode->Release();

	m_BlendDisable->Release();
	m_BlendAlphaEnable->Release();
	m_BlendAlphaDisable->Release();

	InitPointer();
}

void DXD3D::InitPointer()
{
	m_Device = nullptr;
	m_DeviceContext = nullptr;
	m_SwapChain = nullptr;

	m_RenderTargetView = nullptr;

	m_DepthStencilBuffer = nullptr;
	m_DepthEnable = nullptr;
	m_DepthDisable = nullptr;
	m_DepthStencilView = nullptr;

	m_RasterCCWCullMode = nullptr;
	m_RasterCWCullMode = nullptr;
	m_RasterNoCullMode = nullptr;
	m_RasterWireFrame = nullptr;

	m_BlendDisable = nullptr;
	m_BlendAlphaEnable = nullptr;
	m_BlendAlphaDisable = nullptr;

	m_FrameDepthStencilState = nullptr;
	m_FrameRasterizerState = nullptr;
	m_FrameBlendState = nullptr;
}

/////////////////////////////////////////////


/////////////////////////////////////////////
/*
@Function : Frame Functions for rendering
@Usage : Clear for scene and set value for rendering
*/
/////////////////////////////////////////////
void DXD3D::BeginScene( float Red, float Blue, float Green, float Alpha )
{
	// Set Color that erase Buffer
	float Color[4] = { Red, Green, Blue, Alpha };

	// Erase Backbuffer
	m_DeviceContext->ClearRenderTargetView( m_RenderTargetView, Color );

	// Erase Depthbuffer
	m_DeviceContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}

void DXD3D::RenderEngine( RENDERSTATE* RenderState )
{
	ChangeDepthStencilState( RenderState->DepthStencilState );
	ChangeBlendState( RenderState->BlendState );
	ChangeRasterizerState( RenderState->RasterizerState );

	m_DeviceContext->RSSetState( m_FrameRasterizerState );
	m_DeviceContext->RSSetViewports( 1, &m_ViewportEngine );

	float blendFactor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	m_DeviceContext->OMSetDepthStencilState( m_FrameDepthStencilState, 1 );
	m_DeviceContext->OMSetRenderTargets( 1, &m_RenderTargetView, m_DepthStencilView );
	m_DeviceContext->OMSetBlendState( m_FrameBlendState, blendFactor, 0xffffffff );
}

void DXD3D::RenderInGame( RENDERSTATE* RenderState )
{
	ChangeDepthStencilState( RenderState->DepthStencilState );
	ChangeBlendState( RenderState->BlendState );
	ChangeRasterizerState( RenderState->RasterizerState );

	m_DeviceContext->RSSetState( m_FrameRasterizerState );
	m_DeviceContext->RSSetViewports( 1, &m_ViewportInGame );

	float blendFactor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	m_DeviceContext->OMSetDepthStencilState( m_FrameDepthStencilState, 1 );
	m_DeviceContext->OMSetRenderTargets( 1, &m_RenderTargetView, m_DepthStencilView );
	m_DeviceContext->OMSetBlendState( m_FrameBlendState, blendFactor, 0xffffffff );
}

void DXD3D::EndScene()
{
	m_SwapChain->Present( 0, 0 );
}
/////////////////////////////////////////////


/////////////////////////////////////////////
/*
@Function : Init Functions that has detailed information
@Usage : Init setting value for rendering
*/
/////////////////////////////////////////////
bool DXD3D::InitDeviceAndSwapChain( int Width, int Height, HWND hWnd )
{
	HRESULT hr;

	// Describe Buffer for Device and SwapChain
	DXGI_MODE_DESC deviceBufferDesc;
	ZeroMemory( &deviceBufferDesc, sizeof( DXGI_MODE_DESC ) );

	deviceBufferDesc.Width = Width;
	deviceBufferDesc.Height = Height;
	deviceBufferDesc.RefreshRate.Numerator = 60;
	deviceBufferDesc.RefreshRate.Denominator = 1;
	deviceBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	deviceBufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	deviceBufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );

	swapChainDesc.BufferDesc = deviceBufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Create Device and SwapChain
	hr = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext );

	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Device and SwapChain ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create Device and SwapChain ");
		return true;
	}

}

bool DXD3D::InitRenderTargetView()
{
	HRESULT hr;

	// Create the Backbuffer
	ID3D11Texture2D* backBuffer;

	hr = m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Backbuffer  ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create Backbuffer  ");
	}

	// Create the Render Target View
	hr = m_Device->CreateRenderTargetView( backBuffer, NULL, &m_RenderTargetView);
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Render Target View  ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create Render Target View  ");
	}

	backBuffer->Release();

	// Set the Render Target
	//m_DeviceContext->OMSetRenderTargets( 1, &m_RenderTargetView, NULL );

	return true;
}

bool DXD3D::InitDepthStencilStateAndView( int Width, int Height )
{
	HRESULT hr;

	// Create Depth Buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory( &depthBufferDesc, sizeof( D3D11_TEXTURE2D_DESC ) );

	depthBufferDesc.Width = Width;
	depthBufferDesc.Height = Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = m_Device->CreateTexture2D( &depthBufferDesc, NULL, &m_DepthStencilBuffer );
	if ( FAILED ( hr ) )
	{
		LOG_ERROR(" Failed - Create Depth/Stencil buffer  ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create Depth/Stencil buffer  ");
	}

	// Create Depth/Stencil State
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create Depth/Stencil State that is enable Depth
	depthStencilDesc.DepthEnable = true;
	hr = m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthEnable );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Enabled Depth/Stencil State  ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Enabled Depth/Stencil State  ");
	}

	// Create Depth/Stencil State that is disable Depth
	depthStencilDesc.DepthEnable = false;
	hr = m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthDisable );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Disabled Depth/Stencil State  ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Disabled Depth/Stencil State  ");
	}

	//m_DeviceContext->OMSetDepthStencilState( m_DepthEnabledStencilState, 1 );


	// Create Depth/Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory( &depthStencilViewDesc, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_Device->CreateDepthStencilView( m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView );
	if ( FAILED ( hr ) )
	{
		LOG_ERROR(" Failed - Create Depth/Stencil View  ");
		return false;
	}
	else
	{
		LOG_INFO(" Succsed - Create Depth/Stencil View  ");
	}

	// Set Depth/Stencil View to Render Target
	//m_DeviceContext->OMSetRenderTargets( 1, &m_RenderTargetView, m_DepthStencilView );

	return true;
}

bool DXD3D::InitRasterizerState()
{
	HRESULT hr;

	// Describe the  Rasterizer
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory( &rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );

    // Create WireFrame mode
    rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterDesc.CullMode = D3D11_CULL_NONE;

    hr = m_Device->CreateRasterizerState( &rasterDesc, &m_RasterWireFrame);
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Create Wire Frame  ");
        return false;
    }
    else
    {
    	LOG_INFO(" Succssed - Create Wire Frame  ");
    }

    // Create Counter Wise State ( CW Cull Mode )
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = false;

    hr = m_Device->CreateRasterizerState( &rasterDesc, &m_RasterCWCullMode );
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Create Clock Wise State ");
    	return false;
    }
    else
    {
    	LOG_INFO(" Successed - Create Clock Wise State ");
    }

    // Create Counter Clock Wise State ( CCW Cull Mode )
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = true;

    hr = m_Device->CreateRasterizerState( &rasterDesc, &m_RasterCCWCullMode );
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Create Counter Clock Wise State ");
    	return false;
    }
    else
    {
    	LOG_INFO(" Successed - Create Counter Clock Wise State ");
    }

    // Create No Cull Mode
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;

    hr = m_Device->CreateRasterizerState( &rasterDesc, &m_RasterNoCullMode );
    if ( FAILED( hr ) )
    {
    	LOG_ERROR(" Failed - Create No Cull Mode ");
    	return false;
    }
    else
    {
    	LOG_INFO(" Successed - Create No Cull Mode ");
    }
    // Set Rasterizer State
    // m_DeviceContext->RSSetState( m_RasterState );

    return true;
}

void DXD3D::InitViewport( DXRECTWINDOW* EngineWindow, DXRECTWINDOW* InGameWindow )
{
	m_ViewportEngine.TopLeftX = EngineWindow->PosX;
	m_ViewportEngine.TopLeftY = EngineWindow->PosY;
	m_ViewportEngine.Width = EngineWindow->Width;
	m_ViewportEngine.Height = EngineWindow->Height;

	m_ViewportEngine.MinDepth = 0.0f;
	m_ViewportEngine.MaxDepth = 1.0f;


	m_ViewportInGame.TopLeftX = InGameWindow->PosX;
	m_ViewportInGame.TopLeftY = InGameWindow->PosY;
	m_ViewportInGame.Width = InGameWindow->Width;
	m_ViewportInGame.Height = InGameWindow->Height;

	m_ViewportInGame.MinDepth = 0.0f;
	m_ViewportInGame.MaxDepth = 1.0f;
}

bool DXD3D::InitBlendState()
{
	HRESULT hr;

	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory( &blendStateDesc, sizeof( D3D11_BLEND_DESC ) );

	D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
	ZeroMemory( &renderTargetBlendDesc, sizeof( D3D11_RENDER_TARGET_BLEND_DESC ) );

	// Create Blend State that is disable to blend
	renderTargetBlendDesc.BlendEnable = false;
	renderTargetBlendDesc.SrcBlend = D3D11_BLEND_ONE;
	renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.RenderTargetWriteMask = 0x0f;

	blendStateDesc.RenderTarget[0] = renderTargetBlendDesc;
	hr = m_Device->CreateBlendState( &blendStateDesc, &m_BlendDisable );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create disable blend ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create disable blend ");
	}

	// Create Blend State that is able to blend
	renderTargetBlendDesc.BlendEnable = true;
	renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_COLOR;
	renderTargetBlendDesc.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	// Able to cover using alpha
	blendStateDesc.AlphaToCoverageEnable = true;
	blendStateDesc.RenderTarget[0] = renderTargetBlendDesc;

	hr = m_Device->CreateBlendState( &blendStateDesc, &m_BlendAlphaEnable);
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Enable Alpha Blend State  ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Enable Alpha Blend State  ");
	}

	// Disable to cover using alpha
	blendStateDesc.AlphaToCoverageEnable = false;
	blendStateDesc.RenderTarget[0] = renderTargetBlendDesc;

	hr = m_Device->CreateBlendState( &blendStateDesc, &m_BlendAlphaDisable );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Disable Alpha Blend State  ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Disable Alpha Blend State  ");
	}

	return true;
}

void DXD3D::InitMatrix( DXRECTWINDOW* EngineWindow, DXRECTWINDOW* InGameWindow, float SCREEN_DEPTH, float SCREEN_NEAR )
{
    float fieldOfView = 3.141592654f / 4.0f;
    float screenASpect = EngineWindow->Width / EngineWindow->Height;

    // Set for 3D Rendering
    m_ProjectionMatrixEngine = XMMatrixPerspectiveFovLH(fieldOfView, screenASpect, SCREEN_NEAR, SCREEN_DEPTH);

    // Set for 2D Rendering
    m_OrthoMatrixEngine = XMMatrixOrthographicLH( EngineWindow->Width, EngineWindow->Height, SCREEN_NEAR, SCREEN_DEPTH);

    screenASpect = InGameWindow->Width / InGameWindow->Height;

    // Set for 3D Rendering
    m_ProjectionMatrixInGame = XMMatrixPerspectiveFovLH(fieldOfView, screenASpect, SCREEN_NEAR, SCREEN_DEPTH);

    // Set for 2D Rendering
    m_OrthoMatrixInGame = XMMatrixOrthographicLH( InGameWindow->Width, InGameWindow->Height, SCREEN_NEAR, SCREEN_DEPTH);
}
/////////////////////////////////////////////


/////////////////////////////////////////////
/*
@Function : Change Value for rendering
@Usage :
*/
/////////////////////////////////////////////
void DXD3D::ChangeDepthStencilState( DEPTHSTENCIL State )
{
	switch( State )
	{
		case DEPTHACTIVE :
			m_FrameDepthStencilState = m_DepthEnable;
			break;

		case DEPTHDEACTIVE :
			m_FrameDepthStencilState = m_DepthDisable;
			break;
	}
}

void DXD3D::ChangeBlendState( BLEND State )
{
	switch( State )
	{
		case NONE :
			m_FrameBlendState = m_BlendDisable;
			break;

		case ACTIVEALPHA :
			m_FrameBlendState = m_BlendAlphaEnable;
			break;

		case DEACTIVEALPHA :
			m_FrameBlendState = m_BlendAlphaDisable;
			break;
	}
}

void DXD3D::ChangeRasterizerState( RASTERIZER State )
{
	switch( State )
	{
	case WIREFRAME :
		m_FrameRasterizerState = m_RasterWireFrame;
		break;

	case CCWCUL :
		m_FrameRasterizerState = m_RasterCCWCullMode;
		break;

	case CWCUL :
		m_FrameRasterizerState = m_RasterCWCullMode;
		break;

	case NOCUL :
		m_FrameRasterizerState = m_RasterNoCullMode;
		break;
	}
}
/////////////////////////////////////////////


/////////////////////////////////////////////
/*
@Function : Get value
@Usage :
*/
/////////////////////////////////////////////
ID3D11Device* DXD3D::GetDevice() { return m_Device; }
ID3D11DeviceContext* DXD3D::GetDeviceContext() { return m_DeviceContext; }
XMMATRIX DXD3D::GetWorldMatrix() { return XMMatrixIdentity(); }

XMMATRIX DXD3D::GetProjectionMatrixEngine() { return m_ProjectionMatrixEngine; }
XMMATRIX DXD3D::GetOrthoMatrixEngine() { return m_OrthoMatrixEngine; }

XMMATRIX DXD3D::GetProjectionMatrixInGame() { return m_ProjectionMatrixInGame; }
XMMATRIX DXD3D::GetOrthoMatrixInGame() { return m_OrthoMatrixInGame; }
/////////////////////////////////////////////