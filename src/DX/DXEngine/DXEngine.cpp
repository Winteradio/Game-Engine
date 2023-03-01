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


bool DXENGINE::Init( int PosX, int PosY, int Width, int Height, HWND hWnd )
{
	if ( !InitDXD3D( Width, Height, hWnd, SCREEN_DEPTH, SCREEN_NEAR ) ) { return false; }
	if ( !InitDXCAMERA() ) { return false; }
	if ( !InitDXIMGUI( hWnd ) ) { return false; }
	if ( !InitDXINPUT( hWnd, PosX, PosY, Width, Height ) ) { return false; }
	if ( !InitShader( m_VertexShader, Vertex, m_ShaderDir ) ) { return false; }
	if ( !InitShader( m_PixelShader, Pixel, m_ShaderDir ) ) { return false; }
	if ( !InitModel() ) { return false; }
	InitRenderState();
	return true;
}


bool DXENGINE::Frame( int FPS, int CPU, float Time, int mouseX, int mouseY )
{
	m_DXINPUT->Frame( m_DXENGINECAMERA->m_MoveLeftRight, m_DXENGINECAMERA->m_MoveBackForward,
		m_DXENGINECAMERA->m_Yaw, m_DXENGINECAMERA->m_Pitch );
	m_DXIMGUI->Frame();
	return Render();
}

bool DXENGINE::Render()
{
	// Erase Buffer for Drawing Scene
	m_DXD3D->BeginScene( 0.2f, 0.2f, 0.2f, 1.0f );

// Game Engine Rendering

	// Cube1
	m_DXENGINECAMERA->Frame();
	m_Model->SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_Model->SetScale( XMFLOAT3( 0.8f, 0.8f, 0.8f ) );
	m_VertexShader->Frame( m_Model->Frame(), m_DXENGINECAMERA->GetViewMatrix(), m_DXD3D->GetProjectionMatrix(), m_DXENGINECAMERA->GetCameraInfo() );
	m_PixelShader->Frame();

	m_DXD3D->RenderEngine( m_RenderState );
	m_VertexShader->Render( m_DXD3D->GetDeviceContext() );
	m_PixelShader->Render( m_DXD3D->GetDeviceContext() );
	m_Model->Render( m_DXD3D->GetDeviceContext() );

	// Cube2
	m_Model->SetPosition( XMFLOAT3( 5.0f, 1.0f, 0.0f ) );
	m_Model->SetScale( XMFLOAT3( 0.8f, 0.8f, 0.8f ) );
	m_VertexShader->Frame( m_Model->Frame(), m_DXENGINECAMERA->GetViewMatrix(), m_DXD3D->GetProjectionMatrix(), m_DXENGINECAMERA->GetCameraInfo() );
	m_PixelShader->Frame();

	m_DXD3D->RenderEngine( m_RenderState );
	m_VertexShader->Render( m_DXD3D->GetDeviceContext() );
	m_PixelShader->Render( m_DXD3D->GetDeviceContext() );
	m_Model->Render( m_DXD3D->GetDeviceContext() );

// In Game Rendering
	// Cube1
	m_DXINGAMECAMERA->Frame();
	m_Model->SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_Model->SetScale( XMFLOAT3( 0.8f, 0.8f, 0.8f ) );
	m_VertexShader->Frame( m_Model->Frame(), m_DXINGAMECAMERA->GetViewMatrix(), m_DXD3D->GetProjectionMatrix(), m_DXINGAMECAMERA->GetCameraInfo() );
	m_PixelShader->Frame();

	m_DXD3D->RenderInGame( m_RenderState );
	m_VertexShader->Render( m_DXD3D->GetDeviceContext() );
	m_PixelShader->Render( m_DXD3D->GetDeviceContext() );
	m_Model->Render( m_DXD3D->GetDeviceContext() );

	// Cube2
	m_Model->SetPosition( XMFLOAT3( 5.0f, 1.0f, 0.0f ) );
	m_Model->SetScale( XMFLOAT3( 0.8f, 0.8f, 0.8f ) );
	m_VertexShader->Frame( m_Model->Frame(), m_DXINGAMECAMERA->GetViewMatrix(), m_DXD3D->GetProjectionMatrix(), m_DXINGAMECAMERA->GetCameraInfo() );
	m_PixelShader->Frame();

	m_DXD3D->RenderInGame( m_RenderState );
	m_VertexShader->Render( m_DXD3D->GetDeviceContext() );
	m_PixelShader->Render( m_DXD3D->GetDeviceContext() );
	m_Model->Render( m_DXD3D->GetDeviceContext() );

	// ImGui Render
	m_DXIMGUI->Render( m_RenderState, m_DXINPUT->m_MouseX, m_DXINPUT->m_MouseY );

	// Print Buffor on Monitor
	m_DXD3D->EndScene();

	return true;
}

void DXENGINE::Release()
{
	LOG_INFO(" Release - DXENGINE ");

	m_DXD3D->Release();
	m_DXIMGUI->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_Model->Release();

	delete m_VertexShader;
	delete m_PixelShader;
	delete m_Model;
	delete m_RenderState;
	delete m_DXD3D;
	delete m_DXIMGUI;
	delete m_DXENGINECAMERA;
	delete m_DXINGAMECAMERA;

	InitPointer();
}

void DXENGINE::InitFileDIR()
{
	m_VertexShaderDir = ".\\..\\..\\shader\\VS_Basic.hlsl";
	m_PixelShaderDir = ".\\..\\..\\shader\\PS_Basic.hlsl";
	m_ShaderDir = ".\\..\\..\\shader\\Basic.hlsl";
	m_ImageFileDir1 = ".\\..\\..\\textures\\bluelight.png";
	m_ImageFileDir2 = ".\\..\\..\\textures\\grass.jpg";
}


void DXENGINE::InitPointer()
{
	m_DXD3D = nullptr;
	m_DXENGINECAMERA = nullptr;
	m_DXINGAMECAMERA = nullptr;
	m_DXIMGUI = nullptr;

	m_VertexShader = nullptr;
	m_PixelShader = nullptr;
	m_Model = nullptr;
	m_RenderState = nullptr;

	m_VertexShaderDir = nullptr;
	m_PixelShaderDir = nullptr;
	m_ShaderDir = nullptr;
	m_ImageFileDir1 = nullptr;
	m_ImageFileDir2 = nullptr;
}

bool DXENGINE::InitDXENGINERECT( int Width, int Height )
{
	m_DXENGINERECT = new DXENGINERECT;

	if ( !m_DXENGINERECT )
	{
		LOG_ERROR(" Failed - Create DXENGINERECT ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXENGINERECT ");
	}

	m_DXENGINERECT->SettingRect = XMFLOAT2( 200, Height );
	m_DXENGINERECT->GameEngineRect = XMFLOAT2( ( Width - 200 ) / 2, Height - 400 );
	m_DXENGINERECT->InGameRect = XMFLOAT2( ( Width - 200 ) / 2, Height - 400 );
	m_DXENGINERECT->LogRect = XMFLOAT2( Width - 200, 400 );
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
	m_DXENGINECAMERA = new DXCAMERA;

	if ( !m_DXENGINECAMERA )
	{
		LOG_ERROR(" Failed - Create DXCAMERA ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXCAMERA ");
	}

	m_DXENGINECAMERA->Init();

	m_DXINGAMECAMERA = new DXCAMERA;

	if ( !m_DXINGAMECAMERA )
	{
		LOG_ERROR(" Failed - Create DXCAMERA ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXCAMERA ");
	}

	m_DXINGAMECAMERA->Init();
	return true;
}


bool DXENGINE::InitDXIMGUI( HWND hWnd )
{
	// Create DXIMGUI Object
	m_DXIMGUI = new DXIMGUI;

	if ( !m_DXIMGUI )
	{
		LOG_ERROR(" Failed - Create DXIMGUI ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXIMGUI ");
	}

	if ( !m_DXIMGUI->Init( hWnd, m_DXD3D->GetDevice(), m_DXD3D->GetDeviceContext() ) )
	{
		LOG_ERROR(" Failed - Init DXIMGUI ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init DXIMGUI ");
	}

	return true;
}

bool DXENGINE::InitDXINPUT( HWND hWnd, int PosX, int PosY, int Width, int Height )
{
	// Create DXINPUT Object
	m_DXINPUT = new DXINPUT;

	if ( !m_DXINPUT )
	{
		LOG_ERROR(" Failed - Create DXINPUT ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXINPUT ");
	}

	if ( !m_DXINPUT->Init( hWnd, PosX, PosY, Width, Height ) )
	{
		LOG_ERROR(" Failed - Init DXINPUT ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init DXINPUT ");
	}

	return true;
}

void DXENGINE::InitRenderState()
{
	m_RenderState = new RENDERSTATE;
	m_RenderState->DepthStencilState = DEPTHACTIVE;
	m_RenderState->RasterizerState = NOCUL;
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

	if ( !m_Model->Init( m_DXD3D->GetDevice(), m_ImageFileDir1, m_ImageFileDir2 ) )
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