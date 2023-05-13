#include "PropertyUIHandler.h"

PropertyUIHandler::PropertyUIHandler()
{

}

PropertyUIHandler::~PropertyUIHandler()
{

}

void PropertyUIHandler::Init( int PosX, int PosY, int Width, int Height )
{
    m_PosX = PosX;
    m_PosY = PosY;
    m_Width = Width;
    m_Height = Height;
    
    Log::Info(" Init - Property UI Handler ");
}

void PropertyUIHandler::Destroy()
{
    Log::Info(" Destroy - Property UI Handler ");
}

void PropertyUIHandler::Update( float DeltaTime )
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

	ImGui::Begin( "Property" );
	{
        if ( !m_ID.Empty() )
        {
            ImGui::SeparatorText(" Name ");
            ImGui::SeparatorText(" UUID ");
            ImGui::SeparatorText(" Components Info ");
            ImGui::SeparatorText(" Nodes Info ");
        }
	}
	ImGui::End();
}

void PropertyUIHandler::SetID( MyUUID ID ) { m_ID = ID; }

PropertyUIHandler& PropertyUIHandler::GetHandle() { return m_PropertyUIHandler; }

PropertyUIHandler PropertyUIHandler::m_PropertyUIHandler;