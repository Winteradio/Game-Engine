#include "DirectXAPI.h"

DirectXAPI::DirectXAPI()
{
	m_Device = nullptr;
	m_DeviceContext = nullptr;
	m_SwapChain = nullptr;

	m_RenderTargetView = nullptr;
	m_DepthStencilBuffer = nullptr;
	m_DepthStencilView = nullptr;
	m_DepthStencilState = nullptr;
	m_RasterizerState = nullptr;
	m_BlendState = nullptr;
}

DirectXAPI::~DirectXAPI()
{
	Destroy();
}

void DirectXAPI::Init( int Width, int Height, HWND hWnd )
{
	CreateDeviceAndSwapChain( Width, Height, hWnd );
	CreateRenderTargetView();
	CreateDepthStencilBuffer();
	CreateDepthStencilView();
	CreateDepthStencilEnable();
	CreateRasterWireFrame();
	CreateBlendAlphaEnable();

	Log::Info(" Successed - Init DirectX API ");
}

void DirectXAPI::Destroy()
{
	if ( m_Device != nullptr ) m_Device->Release();
	if ( m_DeviceContext != nullptr ) m_DeviceContext->Release();
	if ( m_SwapChain != nullptr ) m_SwapChain->Release();
	if ( m_RenderTargetView != nullptr ) m_RenderTargetView->Release();
	if ( m_DepthStencilBuffer != nullptr ) m_DepthStencilBuffer->Release();
	if ( m_DepthStencilView != nullptr ) m_DepthStencilView->Release();
	if ( m_DepthStencilState != nullptr ) m_DepthStencilState->Release();
	if ( m_RasterizerState != nullptr ) m_RasterizerState->Release();
	if ( m_BlendState != nullptr ) m_BlendState->Release();

	Log::Info(" Destroy - DirectX API ");
}

void DirectXAPI::Begin()
{
	float Color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	m_DeviceContext->OMSetRenderTargets( 1, &m_RenderTargetView, m_DepthStencilView );
	m_DeviceContext->ClearRenderTargetView( m_RenderTargetView, Color );
	m_DeviceContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}

void DirectXAPI::End()
{
    m_SwapChain->Present( 0, 0 );
}

void DirectXAPI::CreateDeviceAndSwapChain( int Width, int Height, HWND hWnd )
{
	m_Width = Width;
	m_Height = Height;

	HRESULT hr;

	// Describe Buffer for Device and SwapChain
	DXGI_MODE_DESC deviceBufferDesc;
	ZeroMemory( &deviceBufferDesc, sizeof( DXGI_MODE_DESC ) );

	deviceBufferDesc.Width = m_Width;
	deviceBufferDesc.Height = m_Height;
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

	if ( FAILED( hr ) ) { Log::Error(" Failed - Create Device, Device Context and SwapChain "); }
	else { Log::Info(" Successed - Create Device, Device Context and SwapChain "); }
}

void DirectXAPI::CreateRenderTargetView()
{
	if ( m_RenderTargetView != nullptr ) m_RenderTargetView->Release();

	HRESULT hr;

	// Create the Backbuffer
	ID3D11Texture2D* backBuffer;

	hr = DirectXAPI::GetHandle().GetSwapChain()->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer );
	if ( FAILED ( hr ) ) { Log::Error(" Failed - Create Back Buffer "); }
	else { Log::Info(" Successed - Create Back Buffer "); }

	// Create the Render Target View
	hr = DirectXAPI::GetHandle().GetDevice()->CreateRenderTargetView( backBuffer, NULL, &m_RenderTargetView);
	if ( FAILED ( hr ) ) { Log::Error(" Failed - Create Render Target View "); }
	else { Log::Info(" Successed - Create Render Target View "); }

	backBuffer->Release();
}

void DirectXAPI::CreateDepthStencilBuffer()
{
	if ( m_DepthStencilBuffer != nullptr ) m_DepthStencilBuffer->Release();

	HRESULT hr;

	// Create Depth Buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory( &depthBufferDesc, sizeof( D3D11_TEXTURE2D_DESC ) );

	depthBufferDesc.Width = DirectXAPI::GetHandle().GetWidth();
	depthBufferDesc.Height = DirectXAPI::GetHandle().GetHeight();
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = DirectXAPI::GetHandle().GetDevice()->CreateTexture2D( &depthBufferDesc, NULL, &m_DepthStencilBuffer );
	if ( FAILED ( hr ) ) { Log::Error(" Failed - Create Depth, Stencil Buffer "); }
	else { Log::Info(" Successed - Create Depth, Stencil Buffer "); }
}

void DirectXAPI::CreateDepthStencilView()
{
	if ( m_DepthStencilView != nullptr ) m_DepthStencilView->Release();
	if ( m_DepthStencilBuffer == nullptr ) 
	{ 
		Log::Error( " Failed - Create Depth Stencil View - No Depth Stencil Buffer ");
		return ;
	}

	HRESULT hr;

	// Create Depth/Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory( &depthStencilViewDesc, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = DirectXAPI::GetHandle().GetDevice()->CreateDepthStencilView( m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Depth Stencil view "); }
	else { Log::Info(" Successed - Create Depth Stencil view "); }
}

void DirectXAPI::CreateDepthStencilEnable()
{
	if ( m_DepthStencilState != nullptr ) m_DepthStencilState->Release();

	HRESULT hr;

	// Create Depth/Stencil State
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Create Depth/Stencil State that is disable Depth
	depthStencilDesc.DepthEnable = true;

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

	hr = DirectXAPI::GetHandle().GetDevice()->CreateDepthStencilState( &depthStencilDesc, &m_DepthStencilState );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Depth Enable State "); }
	else { Log::Info(" Successed - Create Depth Enable State "); }
}

void DirectXAPI::CreateDepthStencilDisable()
{
	if ( m_DepthStencilState != nullptr ) m_DepthStencilState->Release();

	HRESULT hr;

	// Create Depth/Stencil State
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Create Depth/Stencil State that is disable Depth
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.StencilEnable = false;

	hr = DirectXAPI::GetHandle().GetDevice()->CreateDepthStencilState( &depthStencilDesc, &m_DepthStencilState );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Depth Disable State "); }
	else { Log::Info(" Successed - Create Depth Disable State "); }
}

void DirectXAPI::CreateRasterWireFrame()
{
	HRESULT hr;

	// Describe the  Rasterizer
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory( &rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );

    // Create WireFrame mode
    rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterDesc.CullMode = D3D11_CULL_NONE;

    hr = DirectXAPI::GetHandle().GetDevice()->CreateRasterizerState( &rasterDesc, &m_RasterizerState);
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Rasterizer Wire Frame "); }
	else { Log::Info(" Successed - Create Rasterizer Wire Frame "); }
}

void DirectXAPI::CreateRasterCWCull()
{
	HRESULT hr;

	// Describe the  Rasterizer
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory( &rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );

    // Create Counter Wise State ( CW Cull Mode )
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = false;

    hr = DirectXAPI::GetHandle().GetDevice()->CreateRasterizerState( &rasterDesc, &m_RasterizerState );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Rasterizer CW Cull Mode "); }
	else { Log::Info(" Successed - Create Rasterizer CW Cull Mode "); }
}

void DirectXAPI::CreateRasterCCWCull()
{
	HRESULT hr;

	// Describe the  Rasterizer
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory( &rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );

    // Create Counter Clock Wise State ( CCW Cull Mode )
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = true;

    hr = DirectXAPI::GetHandle().GetDevice()->CreateRasterizerState( &rasterDesc, &m_RasterizerState );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Rasterizer CCW Cull Mode "); }
	else { Log::Info(" Successed - Create Rasterizer CCW Cull Mode "); }
}

void DirectXAPI::CreateRasterNoCull()
{
	HRESULT hr;

	// Describe the  Rasterizer
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory( &rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );

    // Create No Cull Mode
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;

    hr = DirectXAPI::GetHandle().GetDevice()->CreateRasterizerState( &rasterDesc, &m_RasterizerState );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Rasterizer No Cull Mode "); }
	else { Log::Info(" Successed - Create Rasterizer No Cull Mode "); }
}

void DirectXAPI::CreateBlendDisable()
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
	hr = DirectXAPI::GetHandle().GetDevice()->CreateBlendState( &blendStateDesc, &m_BlendState );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Blend Disable "); }
	else { Log::Info(" Successed - Create Blend Disable "); }
}

void DirectXAPI::CreateBlendAlphaEnable()
{
	HRESULT hr;

	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory( &blendStateDesc, sizeof( D3D11_BLEND_DESC ) );

	D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
	ZeroMemory( &renderTargetBlendDesc, sizeof( D3D11_RENDER_TARGET_BLEND_DESC ) );

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

	hr = DirectXAPI::GetHandle().GetDevice()->CreateBlendState( &blendStateDesc, &m_BlendState);
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Blend Alpha Enable "); }
	else { Log::Info(" Successed - Create Blend Alpha Enable "); }
}

void DirectXAPI::CreateBlendAlphaDisable()
{
	HRESULT hr;

	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory( &blendStateDesc, sizeof( D3D11_BLEND_DESC ) );

	D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
	ZeroMemory( &renderTargetBlendDesc, sizeof( D3D11_RENDER_TARGET_BLEND_DESC ) );

	// Create Blend State that is able to blend
	renderTargetBlendDesc.BlendEnable = true;
	renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_COLOR;
	renderTargetBlendDesc.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	// Disable to cover using alpha
	blendStateDesc.AlphaToCoverageEnable = false;
	blendStateDesc.RenderTarget[0] = renderTargetBlendDesc;

	hr = DirectXAPI::GetHandle().GetDevice()->CreateBlendState( &blendStateDesc, &m_BlendState );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Blend Alpha Diable "); }
	else { Log::Info(" Successed - Create Blend Alpha Diable "); }
}

DirectXAPI& DirectXAPI::GetHandle() { return m_DirectXAPI; }

ID3D11Device*& DirectXAPI::GetDevice() { return m_Device; }
ID3D11DeviceContext*& DirectXAPI::GetDeviceContext() { return m_DeviceContext; }
IDXGISwapChain*& DirectXAPI::GetSwapChain() { return m_SwapChain; }

ID3D11RenderTargetView*& DirectXAPI::GetRenderTargetView() { return m_RenderTargetView; }
ID3D11Texture2D*& DirectXAPI::GetDepthStencilBuffer() { return m_DepthStencilBuffer; }
ID3D11DepthStencilView*& DirectXAPI::GetDepthStencilView() { return m_DepthStencilView; }
ID3D11DepthStencilState*& DirectXAPI::GetDepthStencilState() { return m_DepthStencilState; }
ID3D11RasterizerState*& DirectXAPI::GetRasterizerState() { return m_RasterizerState; }
ID3D11BlendState*& DirectXAPI::GetBlendState() { return m_BlendState; }

DirectXAPI DirectXAPI::m_DirectXAPI;