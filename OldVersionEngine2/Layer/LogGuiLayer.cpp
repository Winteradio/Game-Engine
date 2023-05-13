#include "LogGuiLayer.h"

LogGuiLayer::LogGuiLayer()
{
	m_Name = "Log";
	m_Rendering = true;
}

LogGuiLayer::LogGuiLayer( const LogGuiLayer& Other )
{
	m_PosX = Other.m_PosX;
	m_PosY = Other.m_PosY;
	m_Width = Other.m_Width;
	m_Height = Other.m_Height;
	m_Rendering = Other.m_Rendering;
	m_Name = Other.m_Name;
}

LogGuiLayer::~LogGuiLayer()
{
	Destroy();
}

void LogGuiLayer::Init( int PosX, int PosY, int Width, int Height )
{
	m_PosX = PosX;
	m_PosY = PosY;
	m_Width = Width;
	m_Height = Height;
}

void LogGuiLayer::Destroy()
{

}

void LogGuiLayer::Update()
{

}

void LogGuiLayer::Render()
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

	ImGui::Begin( m_Name.c_str() );
	for ( auto Message : Log::GetMessage() )
	{
		ImGui::Text( Message.c_str() );
	}
	ImGui::End();
}