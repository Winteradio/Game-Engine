#include "DXImGUI.h"

DXIMGUI::DXIMGUI()
{
	InitPointer();
}

DXIMGUI::DXIMGUI( const DXIMGUI& Other )
{
	*this = Other;
}

DXIMGUI::~DXIMGUI(){}

bool DXIMGUI::Init( DXRECTWINDOW* UserWindow, DXRECTWINDOW* InfoWindow, HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext )
{
	m_DXUserWindow = UserWindow;
	m_DXInfoWindow = InfoWindow;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_ImGuiIO = &ImGui::GetIO();
	if ( !ImGui_ImplWin32_Init( hWnd ) )
	{
		LOG_ERROR(" Failed - Init Window Handle for IMGUI ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init Window Handle for IMGUI ");
	}

	if ( !ImGui_ImplDX11_Init( Device, DeviceContext ) )
	{
		LOG_ERROR(" Failed - Init DirectX11 Device and Device Context for IMGUI ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init DirectX11 Device and Device Context for IMGUI ");
	}
	ImGui::StyleColorsDark();

	if ( !InitFileBrowser() ) { return false; }

	m_DefaultFont = "../../fonts/font.ttf";

	SetFonts( m_DefaultFont );

	return true;
}

bool DXIMGUI::InitFileBrowser()
{
	m_DIFileBrowser = new DIFILEBROWSER;

	if ( !m_DIFileBrowser )
	{
		LOG_ERROR(" Failed - Create File Browser Object ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create File Browser Object ");
	}

	m_DIFileBrowser->Init();

	return true;
}

bool DXIMGUI::Frame()
{
	SetFonts( m_DIFileBrowser->GetFilePath() );

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	return true;
}

bool DXIMGUI::Render( RENDERSTATE* RenderState, int MouseX, int MouseY )
{
	static int Counter = 0;

	ImGui::SetNextWindowPos( ImVec2( (float)m_DXUserWindow->PosX, (float)m_DXUserWindow->PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_DXUserWindow->Width, (float)m_DXUserWindow->Height ));

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar;

	ImGui::Begin(" Rendering State ", NULL, flags );
	if ( ImGui::BeginMenuBar() )
	{
		if ( ImGui::BeginMenu("File") )
		{
			if ( ImGui::MenuItem("Open") )
			{
				m_FileBrowser = true;
				m_DIFileBrowser->Frame();
			}
			if ( ImGui::MenuItem("Load") )
			{
			}
			if ( ImGui::MenuItem("Save") )
			{
			}
			if ( ImGui::MenuItem("Save as") )
			{
			}
			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu("Setting") )
		{
			if ( ImGui::MenuItem("Font") )
			{
				m_FileBrowser = true;
				m_DIFileBrowser->Frame();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if ( ImGui::CollapsingHeader("Rasterizer State") )
	{
		ImGui::Spacing();
		if  ( ImGui::Button("No Cull") )
		{
			RenderState->RasterizerState = NOCUL;
		}
		ImGui::SameLine();
		if ( ImGui::Button("CW Cull") )
		{
			RenderState->RasterizerState = CWCUL;
		}
		ImGui::SameLine();
		if ( ImGui::Button("CCW Cull") )
		{
			RenderState->RasterizerState = CCWCUL;
		}
		ImGui::SameLine();
		if ( ImGui::Button("Wire Frame") )
		{
			RenderState->RasterizerState = WIREFRAME;
		}
	}

	if ( ImGui::CollapsingHeader("Blend State") )
	{
		ImGui::Spacing();
		if ( ImGui::Button("No Blend") )
		{
			RenderState->BlendState = NONE;
		}
		ImGui::SameLine();
		if ( ImGui::Button("Active Alpha") )
		{
			RenderState->BlendState = ACTIVEALPHA;
		}
		ImGui::SameLine();
		if ( ImGui::Button("Deactive Alpha") )
		{
			RenderState->BlendState = DEACTIVEALPHA;
		}
	}

	if ( ImGui::CollapsingHeader("Depth/Stencil State") )
	{
		ImGui::Spacing();
		if ( ImGui::Button("Active Depth") )
		{
			RenderState->DepthStencilState = DEPTHACTIVE;
		}
		ImGui::SameLine();
		if ( ImGui::Button("Deactive Depth") )
		{
			RenderState->DepthStencilState = DEPTHDEACTIVE;
		}
	}

	if ( ImGui::CollapsingHeader("Mouse Position") )
	{
		ImGui::Spacing();
		ImGui::Text("Mouse X %d Y %d", MouseX, MouseY );
		for ( int I = 0; I < 1000; I++ )
		{
			ImGui::Text("Count %d", I );
		}
	}

	ImGui::End();

	if ( m_FileBrowser )
	{
		m_DIFileBrowser->Render( m_FileBrowser );
	}

	ImGui::SetNextWindowPos( ImVec2( (float)m_DXInfoWindow->PosX, (float)m_DXInfoWindow->PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_DXInfoWindow->Width, (float)m_DXInfoWindow->Height ) );

	ImGuiWindowFlags Infoflags = ImGuiWindowFlags_NoDecoration;

	ImGui::Begin(" Info ", NULL, Infoflags );
	ImGui::SameLine();
	if ( ImGui::BeginTabBar(" Information ") )
	{
		if ( ImGui::BeginTabItem(" ALL Log " ) )
		{
			ImGui::Text(" ALL Log...");
			ImGui::EndTabItem();
		}

		if ( ImGui::BeginTabItem(" Physics ") )
		{
			ImGui::Text(" Physics... ");
			ImGui::EndTabItem();
		}

		if ( ImGui::BeginTabItem(" Object ") )
		{
			ImGui::Text(" Object... ");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );

	return true;
}

void DXIMGUI::Release()
{
	LOG_INFO(" Release - IMGUI ");
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DXIMGUI::InitPointer()
{
	m_ImGuiIO = nullptr;
}

void DXIMGUI::SetFonts( char* Path )
{
	if ( Path != nullptr && strcmp( Path, m_FontAddr ) != 0)
	{
		std::string TempString(Path);
		if ( TempString.find(".ttf") != std::string::npos )
		{
			strcpy( m_FontAddr, Path );
			m_FontSize = 12.0f;
			m_ImGuiIO->Fonts->Clear();
			m_ImGuiIO->Fonts->AddFontFromFileTTF( m_FontAddr, m_FontSize, NULL, m_ImGuiIO->Fonts->GetGlyphRangesKorean() );
			ImGui_ImplDX11_InvalidateDeviceObjects();
			m_ImGuiIO->Fonts->Build();


		}
	}
}