#include "RenderingStatus.h"

RenderingStatus::RenderingStatus()
{
	m_DepthStencilView = nullptr;
	m_DepthEnable = nullptr;
	m_DepthDisable = nullptr;

	m_RasterWireFrame = nullptr;
	m_RasterCWCull = nullptr;
	m_RasterCCWCull = nullptr;
	m_RasterNoCull = nullptr;

	m_BlendDisable = nullptr;
	m_BlendAlphaEnable = nullptr;
	m_BlendAlphaDisable = nullptr;

	m_CurrentDepthStencilState = nullptr;
	m_CurrentRasterizerState = nullptr;
	m_CurrentBlendState = nullptr;
}

RenderingStatus::~RenderingStatus()
{
	Destroy();
}

RenderingStatus& RenderingStatus::Get() { return m_RenderingStatus; }

void RenderingStatus::Init()
{
	CreateRenderTargetView();
	CreateDepthStencilState();
	CreateRasterizerState();
	CreateBlendState();
}

void RenderingStatus::Destroy()
{
	if ( m_DepthStencilView != nullptr ) m_DepthStencilView->Release();
	if ( m_DepthEnable != nullptr ) m_DepthEnable->Release();
	if ( m_DepthDisable != nullptr ) m_DepthDisable->Release();

	if ( m_RasterWireFrame != nullptr ) m_RasterWireFrame->Release();
	if ( m_RasterCWCull != nullptr ) m_RasterCWCull->Release();
	if ( m_RasterCCWCull != nullptr ) m_RasterCCWCull->Release();
	if ( m_RasterNoCull != nullptr ) m_RasterNoCull->Release();

	if ( m_BlendDisable != nullptr ) m_BlendDisable->Release();
	if ( m_BlendAlphaEnable != nullptr ) m_BlendAlphaEnable->Release();
	if ( m_BlendAlphaDisable != nullptr ) m_BlendAlphaDisable->Release();

	if ( m_CurrentDepthStencilState != nullptr ) m_CurrentDepthStencilState->Release();
	if ( m_CurrentRasterizerState != nullptr ) m_CurrentRasterizerState->Release();
	if ( m_CurrentBlendState != nullptr ) m_CurrentBlendState->Release();
}

void RenderingStatus::CreateRenderTargetView()
{
	HRESULT hr;

	// Create the Backbuffer
	ID3D11Texture2D* backBuffer;

	hr = DirectXAPI::Get().GetSwapChain()->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer );
	if ( FAILED ( hr ) ) { Log::Error(" Failed - Create Back Buffer "); }
	else { Log::Info(" Successed - Create Back Buffer "); }

	// Create the Render Target View
	hr = DirectXAPI::Get().GetDevice()->CreateRenderTargetView( backBuffer, NULL, &m_RenderTargetView);
	if ( FAILED ( hr ) ) { Log::Error(" Failed - Create Render Target View "); }
	else { Log::Info(" Successed - Create Render Target View "); }

	backBuffer->Release();
}

void RenderingStatus::CreateDepthStencilState()
{
	HRESULT hr;

	ID3D11Texture2D* depthStencilBuffer;

	// Create Depth Buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory( &depthBufferDesc, sizeof( D3D11_TEXTURE2D_DESC ) );

	depthBufferDesc.Width = DirectXAPI::Get().GetWidth();
	depthBufferDesc.Height = DirectXAPI::Get().GetHeight();
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = DirectXAPI::Get().GetDevice()->CreateTexture2D( &depthBufferDesc, NULL, &depthStencilBuffer );
	if ( FAILED ( hr ) ) { Log::Error(" Failed - Create Depth, Stencil Buffer "); }
	else { Log::Info(" Successed - Create Depth, Stencil Buffer "); }

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
	hr = DirectXAPI::Get().GetDevice()->CreateDepthStencilState( &depthStencilDesc, &m_DepthEnable );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Depth Enable State "); }
	else { Log::Info(" Successed - Create Depth Enable State "); }

	// Create Depth/Stencil State that is disable Depth
	depthStencilDesc.DepthEnable = false;
	hr = DirectXAPI::Get().GetDevice()->CreateDepthStencilState( &depthStencilDesc, &m_DepthDisable );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Depth Disable State "); }
	else { Log::Info(" Successed - Create Depth Disable State "); }

	// Create Depth/Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory( &depthStencilViewDesc, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = DirectXAPI::Get().GetDevice()->CreateDepthStencilView( depthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Depth Stencil view "); }
	else { Log::Info(" Successed - Create Depth Stencil view "); }

	depthStencilBuffer->Release();

	m_CurrentDepthStencilState = m_DepthEnable;
}

void RenderingStatus::CreateRasterizerState()
{
	HRESULT hr;

	// Describe the  Rasterizer
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory( &rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );

    // Create WireFrame mode
    rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterDesc.CullMode = D3D11_CULL_NONE;

    hr = DirectXAPI::Get().GetDevice()->CreateRasterizerState( &rasterDesc, &m_RasterWireFrame);
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Rasterizer Wire Frame "); }
	else { Log::Info(" Successed - Create Rasterizer Wire Frame "); }

    // Create Counter Wise State ( CW Cull Mode )
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = false;

    hr = DirectXAPI::Get().GetDevice()->CreateRasterizerState( &rasterDesc, &m_RasterCWCull );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Rasterizer CW Cull Mode "); }
	else { Log::Info(" Successed - Create Rasterizer CW Cull Mode "); }

    // Create Counter Clock Wise State ( CCW Cull Mode )
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = true;

    hr = DirectXAPI::Get().GetDevice()->CreateRasterizerState( &rasterDesc, &m_RasterCCWCull );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Rasterizer CCW Cull Mode "); }
	else { Log::Info(" Successed - Create Rasterizer CCW Cull Mode "); }

    // Create No Cull Mode
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;

    hr = DirectXAPI::Get().GetDevice()->CreateRasterizerState( &rasterDesc, &m_RasterNoCull );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Rasterizer No Cull Mode "); }
	else { Log::Info(" Successed - Create Rasterizer No Cull Mode "); }

	m_CurrentRasterizerState = m_RasterWireFrame;
}

void RenderingStatus::CreateBlendState()
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
	hr = DirectXAPI::Get().GetDevice()->CreateBlendState( &blendStateDesc, &m_BlendDisable );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Blend Disable "); }
	else { Log::Info(" Successed - Create Blend Disable "); }

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

	hr = DirectXAPI::Get().GetDevice()->CreateBlendState( &blendStateDesc, &m_BlendAlphaEnable);
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Blend Alpha Enable "); }
	else { Log::Info(" Successed - Create Blend Alpha Enable "); }

	// Disable to cover using alpha
	blendStateDesc.AlphaToCoverageEnable = false;
	blendStateDesc.RenderTarget[0] = renderTargetBlendDesc;

	hr = DirectXAPI::Get().GetDevice()->CreateBlendState( &blendStateDesc, &m_BlendAlphaDisable );
	if ( FAILED( hr ) ) { Log::Error( " Failed - Create Blend Alpha Diable "); }
	else { Log::Info(" Successed - Create Blend Alpha Diable "); }

	m_CurrentBlendState = m_BlendDisable;
}

void RenderingStatus::ChangeToDepthStencilStateEnable() { m_CurrentDepthStencilState = m_DepthEnable; }
void RenderingStatus::ChangeToDepthStencilStateDisable() { m_CurrentDepthStencilState = m_DepthDisable; }

void RenderingStatus::ChangeToRasterWireFrame() { m_CurrentRasterizerState = m_RasterWireFrame; }
void RenderingStatus::ChangeToRasterCWCull() { m_CurrentRasterizerState = m_RasterCWCull; }
void RenderingStatus::ChangeToRasterCCWCull() { m_CurrentRasterizerState = m_RasterCCWCull; }
void RenderingStatus::ChangeToRasterNoCull() { m_CurrentRasterizerState = m_RasterNoCull; }

void RenderingStatus::ChangeToBlendDisable() { m_CurrentBlendState = m_BlendDisable; }
void RenderingStatus::ChangeToBlendAlphaEnable() { m_CurrentBlendState = m_BlendAlphaEnable; }
void RenderingStatus::ChangeToBlendAlphaDisable() { m_CurrentBlendState = m_BlendAlphaDisable; }

ID3D11RenderTargetView*& RenderingStatus::GetRenderTargetView() { return m_RenderTargetView; }
ID3D11DepthStencilView*& RenderingStatus::GetDepthStencilView() { return m_DepthStencilView; }
ID3D11DepthStencilState*& RenderingStatus::GetDepthStencilState() { return m_CurrentDepthStencilState; }
ID3D11RasterizerState*& RenderingStatus::GetRasterizerState() { return m_CurrentRasterizerState; }
ID3D11BlendState*& RenderingStatus::GetBlendState() { return m_CurrentBlendState; }

RenderingStatus RenderingStatus::m_RenderingStatus;

