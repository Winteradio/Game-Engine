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

	}
	ImGui::End();
}

PropertyUIHandler& PropertyUIHandler::GetHandle() { return m_PropertyUIHandler; }

PropertyUIHandler PropertyUIHandler::m_PropertyUIHandler;