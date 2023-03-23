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
	if ( !InitDXWINDOW( Width, Height ) ) { return false; }
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
	/*
	m_DXINPUT->Frame( m_DXEngineCamera->m_MoveLeftRight, m_DXEngineCamera->m_MoveBackForward,
		m_DXEngineCamera->m_Yaw, m_DXEngineCamera->m_Pitch );
	*/
	{
		Count++;
		if ( Count % 10 == 0 )
		{
			std::string Temp = ".\\..\\..\\textures\\GVFX 03\\GVFX 03_00";
			if ( FrameCount < 100 )
			{
				Temp = Temp + "0" + std::to_string(FrameCount) + ".jpg";
			}
			else
			{
				Temp = Temp + std::to_string(FrameCount) + ".jpg";
			}
			m_Model->InitTexture( m_DXD3D->GetDevice(), Temp.c_str(), m_ImageFileDir2 );
			Count = 0;
			FrameCount++;
			if ( FrameCount > 198 )
			{
				FrameCount = 40;
			}
		}
	}
	m_DXIMGUI->Frame();
	return Render();
}

bool DXENGINE::Render()
{
	// Erase Buffer for Drawing Scene
	m_DXD3D->BeginScene( 0.2f, 0.2f, 0.2f, 1.0f );

// Game Engine Rendering

	// Cube1
	m_DXEngineCamera->Frame();
	m_Model->SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_Model->SetScale( XMFLOAT3( 0.8f, 0.8f, 0.8f ) );
	m_VertexShader->Frame( m_Model->Frame(), m_DXEngineCamera->GetViewMatrix(), m_DXD3D->GetProjectionMatrixEngine(), m_DXEngineCamera->GetCameraInfo() );
	m_PixelShader->Frame();

	m_DXD3D->RenderEngine( m_RenderState );
	m_VertexShader->Render( m_DXD3D->GetDeviceContext() );
	m_PixelShader->Render( m_DXD3D->GetDeviceContext() );
	m_Model->Render( m_DXD3D->GetDeviceContext() );

	// Cube2
	m_Model->SetPosition( XMFLOAT3( 5.0f, 1.0f, 0.0f ) );
	m_Model->SetScale( XMFLOAT3( 0.8f, 0.8f, 0.8f ) );
	m_VertexShader->Frame( m_Model->Frame(), m_DXEngineCamera->GetViewMatrix(), m_DXD3D->GetProjectionMatrixEngine(), m_DXEngineCamera->GetCameraInfo() );
	m_PixelShader->Frame();

	m_DXD3D->RenderEngine( m_RenderState );
	m_VertexShader->Render( m_DXD3D->GetDeviceContext() );
	m_PixelShader->Render( m_DXD3D->GetDeviceContext() );
	m_Model->Render( m_DXD3D->GetDeviceContext() );

// In Game Rendering
	// Cube1
	m_DXInGameCamera->Frame();
	m_Model->SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_Model->SetScale( XMFLOAT3( 0.8f, 0.8f, 0.8f ) );
	m_VertexShader->Frame( m_Model->Frame(), m_DXInGameCamera->GetViewMatrix(), m_DXD3D->GetProjectionMatrixInGame(), m_DXInGameCamera->GetCameraInfo() );
	m_PixelShader->Frame();

	m_DXD3D->RenderInGame( m_RenderState );
	m_VertexShader->Render( m_DXD3D->GetDeviceContext() );
	m_PixelShader->Render( m_DXD3D->GetDeviceContext() );
	m_Model->Render( m_DXD3D->GetDeviceContext() );

	// Cube2
	m_Model->SetPosition( XMFLOAT3( 5.0f, 1.0f, 0.0f ) );
	m_Model->SetScale( XMFLOAT3( 0.8f, 0.8f, 0.8f ) );
	m_VertexShader->Frame( m_Model->Frame(), m_DXInGameCamera->GetViewMatrix(), m_DXD3D->GetProjectionMatrixInGame(), m_DXInGameCamera->GetCameraInfo() );
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
	delete m_DXEngineCamera;
	delete m_DXInGameCamera;

	delete m_DXEngineWindow;
	delete m_DXInGameWindow;
	delete m_DXUserWindow;
	delete m_DXInfoWindow;

	InitPointer();
}

void DXENGINE::InitFileDIR()
{
	m_VertexShaderDir = ".\\..\\..\\shader\\VS_Basic.hlsl";
	m_PixelShaderDir = ".\\..\\..\\shader\\PS_Basic.hlsl";
	m_ShaderDir = ".\\..\\..\\shader\\Basic.hlsl";
	m_ImageFileDir1 = ".\\..\\..\\textures\\GVFX 03\\GVFX 03_00075.jpg";
	m_ImageFileDir2 = ".\\..\\..\\textures\\grass.jpg";
}


void DXENGINE::InitPointer()
{
	m_DXD3D = nullptr;
	m_DXEngineCamera = nullptr;
	m_DXInGameCamera = nullptr;
	m_DXIMGUI = nullptr;

	m_DXEngineWindow = nullptr;
	m_DXInGameWindow = nullptr;
	m_DXUserWindow = nullptr;
	m_DXInfoWindow = nullptr;

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

bool DXENGINE::InitDXWINDOW( int Width, int Height )
{
	m_DXEngineWindow = new DXRECTWINDOW;
	m_DXInGameWindow = new DXRECTWINDOW;
	m_DXUserWindow = new DXRECTWINDOW;
	m_DXInfoWindow = new DXRECTWINDOW;

	m_DXEngineWindow->Name = "Engine";
	m_DXEngineWindow->PosX = 300;
	m_DXEngineWindow->PosY = 0;
	m_DXEngineWindow->Width = (int)( ( Width - 300 ) / 2 );
	m_DXEngineWindow->Height = Height - 300;
	m_DXEngineWindow->Rendering = true;

	m_DXInGameWindow->Name = "In Game";
	m_DXInGameWindow->PosX = 300 + m_DXEngineWindow->Width;
	m_DXInGameWindow->PosY = 0;
	m_DXInGameWindow->Width = m_DXEngineWindow->Width;
	m_DXInGameWindow->Height = m_DXEngineWindow->Height;
	m_DXInGameWindow->Rendering = true;

	m_DXUserWindow->Name = "User";
	m_DXUserWindow->PosX = 0;
	m_DXUserWindow->PosY = 0;
	m_DXUserWindow->Width = 300;
	m_DXUserWindow->Height = Height;
	m_DXUserWindow->Rendering = true;

	m_DXInfoWindow->Name = "Info";
	m_DXInfoWindow->PosX = 300;
	m_DXInfoWindow->PosY = Height - 300;
	m_DXInfoWindow->Width = Width - 300;
	m_DXInfoWindow->Height = 300;
	m_DXInfoWindow->Rendering = true;
	return true;
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
	if ( !m_DXD3D->Init( Width, Height, m_DXEngineWindow, m_DXInGameWindow, hWnd, SCREEN_DEPTH, SCREEN_NEAR ) )
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
	m_DXEngineCamera = new DXCAMERA;
	m_DXInGameCamera = new DXCAMERA;

	if ( m_DXEngineCamera != nullptr && m_DXInGameCamera != nullptr )
	{
		LOG_INFO(" Successed - Create Objects of Camera ");
	}
	else
	{
		LOG_ERROR(" Failed - Create Objects of Camera ");
		return false;
	}

	m_DXEngineCamera->Init();
	m_DXInGameCamera->Init();

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

	if ( !m_DXIMGUI->Init( m_DXUserWindow, m_DXInfoWindow, hWnd, m_DXD3D->GetDevice(), m_DXD3D->GetDeviceContext() ) )
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