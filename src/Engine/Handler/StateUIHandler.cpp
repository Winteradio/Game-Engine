#include "StateUIHandler.h"

StateUIHandler::StateUIHandler()
{

}

StateUIHandler::~StateUIHandler()
{

}

void StateUIHandler::Init( int PosX, int PosY, int Width, int Height )
{
    m_PosX = PosX;
    m_PosY = PosY;
    m_Width = Width;
    m_Height = Height;

    Log::Info(" Init - State UI Handler ");
}

void StateUIHandler::Destroy()
{
    Log::Info(" Destroy - State UI Handler ");
}

void StateUIHandler::Update( float DeltaTime )
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

	ImGui::Begin( "State", NULL, ImGuiWindowFlags_NoDecoration );
	ImGui::End();
}

StateUIHandler& StateUIHandler::GetHandle() { return m_StateUIHandler; }

StateUIHandler StateUIHandler::m_StateUIHandler;