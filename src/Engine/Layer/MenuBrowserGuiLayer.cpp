#include "MenuBrowserGuiLayer.h"

MenuBrowserGuiLayer::MenuBrowserGuiLayer()
{
	m_Name = "Menu";
	m_Rendering = true;
}

MenuBrowserGuiLayer::MenuBrowserGuiLayer( const MenuBrowserGuiLayer& Other )
{
	m_PosX = Other.m_PosX;
	m_PosY = Other.m_PosY;
	m_Width = Other.m_Width;
	m_Height = Other.m_Height;
	m_Name = Other.m_Name;
	m_Rendering = Other.m_Rendering;
}

MenuBrowserGuiLayer::~MenuBrowserGuiLayer()
{
	Destroy();
}

void MenuBrowserGuiLayer::Init( int PosX, int PosY, int Width, int Height )
{
	m_PosX = PosX;
	m_PosY = PosY;
	m_Width = Width;
	m_Height = Height;
}

void MenuBrowserGuiLayer::Destroy()
{
	m_Rendering = false;
}

void MenuBrowserGuiLayer::Update()
{
	
}

void MenuBrowserGuiLayer::Render()
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

	ImGui::Begin( m_Name.c_str(), NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground );
	if ( ImGui::BeginMenuBar() )
	{
		if ( ImGui::BeginMenu( "Menu" ) )
		{
			if ( ImGui::MenuItem(" Open ", "Ctrl + O") )
			{
				m_Mode = true;
				FileBrowserGuiLayer::Get().Begin( this, ".yaml", m_Mode );
			}

			if ( ImGui::MenuItem(" Save ", "Ctrl + S") )
			{
				m_Mode = false;
				if ( m_FilePath.empty() )
				{
					FileBrowserGuiLayer::Get().Begin( this, ".yaml", m_Mode );
				}
				else
				{
					SetPath( m_FilePath );
				}
			}

			if ( ImGui::MenuItem(" Save as ", "Ctrl + S") )
			{
				m_Mode = false;
				FileBrowserGuiLayer::Get().Begin( this, ".yaml", m_Mode );
			}

			ImGui::Separator();

			if ( ImGui::MenuItem(" Setting ", "Ctrl + Q") )
			{
				SettingBrowserGuiLayer::Get().Begin();
			}

			if ( ImGui::MenuItem(" Quit ", "Alt + F4") )
			{
				PostQuitMessage( 0 );
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();
}

void MenuBrowserGuiLayer::SetPath( FS::path Path )
{
	m_FilePath = Path;

	if ( m_Mode == true )
	{
		ResourceManager::Destroy();
		ProjectManager::Get().LoadFile( m_FilePath );
	}
	else if ( m_Mode == false )
	{
		ProjectManager::Get().SaveFile( m_FilePath );
	}
}