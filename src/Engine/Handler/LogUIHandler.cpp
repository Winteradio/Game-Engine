#include "LogUIHandler.h"

LogUIHandler::LogUIHandler()
{

}

LogUIHandler::~LogUIHandler()
{

}

void LogUIHandler::Init( int PosX, int PosY, int Width, int Height )
{
    m_PosX = PosX;
    m_PosY = PosY;
    m_Width = Width;
    m_Height = Height;

    Log::Info(" Init - Log UI Handler ");
}

void LogUIHandler::Destroy()
{
    Log::Info(" Destroy - Log UI Handler ");
}

void LogUIHandler::Update( float DeltaTime )
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

	ImGui::Begin( "Log" );
	for ( auto ITRM = Log::GetMessage().rbegin(); ITRM != Log::GetMessage().rend(); ITRM++ )
	{
		ImGui::Text( (*ITRM).c_str() );
	}
	ImGui::End();
}

LogUIHandler& LogUIHandler::GetHandle() { return m_LogUIHandler; }

LogUIHandler LogUIHandler::m_LogUIHandler;