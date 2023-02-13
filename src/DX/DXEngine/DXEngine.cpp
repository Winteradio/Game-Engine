#include "DXEngine.h"

DXENGINE::DXENGINE()
{
	InitPointer();
	InitFileDIR();
}


DXENGINE::DXENGINE( const DXENGINE* Other )
{
	*this = *Other;
}


DXENGINE::~DXENGINE()
{}


bool DXENGINE::Init( int Width, int Height, HWND hWnd )
{
	if ( !InitDXD3D( Width, Height, hWnd, SCREEN_DEPTH, SCREEN_NEAR ) ) { return false; }
	if ( !InitDXCAMERA() ) { return false; }
	if ( !InitShader( m_VertexShader, Vertex, m_ShaderDir ) ) { return false; }
	if ( !InitShader( m_PixelShader, Pixel, m_ShaderDir ) ) { return false; }
	if ( !InitModel() ) { return false; }
	InitRenderState();
	return true;
}


bool DXENGINE::Frame( int FPS, int CPU, float Time, int mouseX, int mouseY )
{
	return Render();
}

bool DXENGINE::Render()
{
	// Erase Buffer for Drawing Scene
	m_DXD3D->BeginScene( 0.2f, 0.2f, 0.2f, 1.0f );

	// Cube1

	m_DXCAMERA->Frame();
	m_Model->SetPosition( XMFLOAT3( 2.0f, 2.0f, 0.0f ) );
	m_Model->SetScale( XMFLOAT3( 0.8f, 0.8f, 0.8f ) );
	m_VertexShader->Frame( m_Model->Frame(), m_DXCAMERA->GetViewMatrix(), m_DXD3D->GetProjectionMatrix(), m_DXCAMERA->GetCameraInfo() );
	m_PixelShader->Frame( m_Model->Frame() );

	m_DXD3D->Render( m_RenderState );
	m_VertexShader->Render( m_DXD3D->GetDeviceContext() );
	m_PixelShader->Render( m_DXD3D->GetDeviceContext() );
	m_Model->Render( m_DXD3D->GetDeviceContext() );

	// Cube2
	m_Model->SetPosition( XMFLOAT3( 5.0f, 0.0f, 0.0f ) );
	m_Model->SetScale( XMFLOAT3( 0.8f, 0.8f, 0.8f ) );
	m_VertexShader->Frame( m_Model->Frame(), m_DXCAMERA->GetViewMatrix(), m_DXD3D->GetProjectionMatrix(), m_DXCAMERA->GetCameraInfo() );
	m_PixelShader->Frame( m_Model->Frame() );

	m_DXD3D->Render( m_RenderState );
	m_VertexShader->Render( m_DXD3D->GetDeviceContext() );
	m_PixelShader->Render( m_DXD3D->GetDeviceContext() );
	m_Model->Render( m_DXD3D->GetDeviceContext() );

	// Print Buffor on Monitor
	m_DXD3D->EndScene();

	return true;
}

void DXENGINE::Release()
{
	LOG_INFO(" Release - DXENGINE ");

	m_DXD3D->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_Model->Release();

	delete m_VertexShader;
	delete m_PixelShader;
	delete m_Model;
	delete m_RenderState;
	delete m_DXD3D;
	delete m_DXCAMERA;

	InitPointer();
}

void DXENGINE::InitFileDIR()
{
	m_VertexShaderDir = ".\\..\\..\\shader\\VS_Basic.hlsl";
	m_PixelShaderDir = ".\\..\\..\\shader\\PS_Basic.hlsl";
	m_ShaderDir = ".\\..\\..\\shader\\Basic.hlsl";
	m_ImageFileDir = ".\\..\\..\\textures\\braynzar.jpg";
}


void DXENGINE::InitPointer()
{
	m_DXD3D = nullptr;
	m_DXCAMERA = nullptr;

	m_VertexShader = nullptr;
	m_PixelShader = nullptr;
	m_Model = nullptr;
	m_RenderState = nullptr;

	m_VertexShaderDir = nullptr;
	m_PixelShaderDir = nullptr;
	m_ShaderDir = nullptr;
	m_ImageFileDir = nullptr;
}


bool DXENGINE::InitDXD3D( int Width, int Height, HWND hWnd, float SCREEN_DEPTH, float SCREEN_NEAR )
{
	// Create DXD3D Object
	m_DXD3D = new DXD3D;

	if ( !m_DXD3D )
	{
		LOG_ERROR(" Failed - Create DXD3D ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Create DXD3D ");
	}

	// Init DXD3D Object
	if ( !m_DXD3D->Init( Width, Height, hWnd, SCREEN_DEPTH, SCREEN_NEAR ) )
	{
		LOG_ERROR(" Failed - Init DXD3D ");
		return false;
	}
	else
	{
		LOG_INFO(" Succssed - Init DXD3D ");
	}

	return true;
}


bool DXENGINE::InitDXCAMERA()
{
	// Create DXCAMERA Object
	m_DXCAMERA = new DXCAMERA;

	if ( !m_DXCAMERA )
	{
		LOG_ERROR(" Failed - Create DXCAMERA ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXCAMERA ");
	}

	m_DXCAMERA->Init();
	return true;
}

void DXENGINE::InitRenderState()
{
	m_RenderState = new RENDERSTATE;
	m_RenderState->DepthStencilState = DEPTHACTIVE;
	m_RenderState->RasterizerState = CWCUL;
	m_RenderState->BlendState = ACTIVEALPHA;
}

bool DXENGINE::InitShader( OBSHADER*& Shader, SHADERTYPE Type, const char* FileDir )
{
	Shader = new OBSHADER;

	if ( !Shader )
	{
		LOG_ERROR(" Failed - Create Shader Object ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Shader Object ");
	}

	if ( !Shader->Init( m_DXD3D->GetDevice(), Type, FileDir ) )
	{
		LOG_ERROR(" Failed - Init Shader Object ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init Shader Object ");
	}

	return true;
}

bool DXENGINE::InitModel()
{
	m_Model = new OBMODEL;

	if ( !m_Model )
	{
		LOG_ERROR(" Failed - Create Model Object ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Model Object ");
	}

	if ( !m_Model->Init( m_DXD3D->GetDevice(), m_ImageFileDir ) )
	{
		LOG_ERROR(" Failed - Init Model Object ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init Model Object ");
	}

	return true;
}