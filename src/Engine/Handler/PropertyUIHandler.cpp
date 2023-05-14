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

    memset( m_Temp, 0, MAX_LENGTH );
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
            ImGui::SetNextItemWidth( 200.0f );
            ImGui::InputText("##Input", m_Temp, MAX_LENGTH );
            ImGui::SameLine();
            if ( ImGui::Button("Set") )
            {
                EntityManager::GetHandle().GetEntity( m_ID )->SetName( m_Temp );
            }
            ImGui::SeparatorText(" UUID ");
            ImGui::Text( m_ID.GetString().c_str() );
            ImGui::SeparatorText(" Components Info ");
            if ( ImGui::Selectable( " New ... " ) )
            {

            }
            ImGui::SeparatorText(" Nodes Info ");
        }
	}
	ImGui::End();
}

void PropertyUIHandler::SetID( MyUUID ID ) 
{ 
    m_ID = ID; 
    strcpy( m_Temp, EntityManager::GetHandle().GetEntity( ID )->GetName().c_str() );
}

PropertyUIHandler& PropertyUIHandler::GetHandle() { return m_PropertyUIHandler; }

PropertyUIHandler PropertyUIHandler::m_PropertyUIHandler;