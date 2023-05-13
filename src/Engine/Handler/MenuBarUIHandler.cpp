#include "MenuBarUIHandler.h"

MenuBarUIHandler::MenuBarUIHandler()
{

}

MenuBarUIHandler::~MenuBarUIHandler()
{

}

void MenuBarUIHandler::Init( int PosX, int PosY, int Width, int Height )
{
    m_PosX = PosX;
    m_PosY = PosY;
    m_Width = Width;
    m_Height = Height;

    Log::Info(" Init - MenuBar UI Handler ");
}

void MenuBarUIHandler::Destroy()
{
    Log::Info(" Destroy - MenuBar UI Handler ");
}

void MenuBarUIHandler::Update( float DeltaTime )
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

	ImGui::Begin( "MenuBar", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground );
	if ( ImGui::BeginMenuBar() )
	{
		if ( ImGui::BeginMenu( "File" ) )
		{
			if ( ImGui::MenuItem(" Open ", "Ctrl + O") )
			{
			}

			if ( ImGui::MenuItem(" Save ", "Ctrl + S") )
			{
			}

			if ( ImGui::MenuItem(" Save as ", "Ctrl + S") )
			{
			}

			ImGui::EndMenu();
		}

        if ( ImGui::BeginMenu( "Setting" ) )
        {
            if ( ImGui::MenuItem(" Font ") )
            {

            }

            ImGui::EndMenu();
        }

		ImGui::EndMenuBar();
	}

	ImGui::End();
}

MenuBarUIHandler& MenuBarUIHandler::GetHandle() { return m_MenuBarUIHandler; }

MenuBarUIHandler MenuBarUIHandler::m_MenuBarUIHandler;