#include "DirectXAPI.h"

DirectXAPI::DirectXAPI()
{
	m_Device = nullptr;
	m_DeviceContext = nullptr;
	m_SwapChain = nullptr;
}

DirectXAPI::~DirectXAPI()
{
	Destroy();
}

void DirectXAPI::Init( int Width, int Height, HWND hWnd )
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

void DirectXAPI::Destroy()
{
	if ( m_Device != nullptr ) m_Device->Release();
	if ( m_DeviceContext != nullptr ) m_DeviceContext->Release();
	if ( m_SwapChain != nullptr ) m_SwapChain->Release();
}

DirectXAPI& DirectXAPI::GetHandle() { return m_DirectXAPI; }
ID3D11Device*& DirectXAPI::GetDevice() { return m_Device; }
ID3D11DeviceContext*& DirectXAPI::GetDeviceContext() { return m_DeviceContext; }
IDXGISwapChain*& DirectXAPI::GetSwapChain() { return m_SwapChain; }

DirectXAPI DirectXAPI::m_DirectXAPI;