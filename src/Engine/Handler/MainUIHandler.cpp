#include "MainUIHandler.h"

MainUIHandler::MainUIHandler()
{

}

MainUIHandler::~MainUIHandler()
{

}

void MainUIHandler::Init( int PosX, int PosY, int Width, int Height )
{
    m_PosX = PosX;
    m_PosY = PosY;
    m_Width = Width;
    m_Height = Height;
    
    Log::Info(" Init - Main UI Handler ");
}

void MainUIHandler::Destroy()
{
    Log::Info(" Destroy - Main UI Handler ");
}

void MainUIHandler::Update( float DeltaTime )
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

	ImGui::Begin( "Main" );
    ImGui::SeparatorText(" Scene ");
    if ( ImGui::Selectable( " Create New Scene ... " ) )
    {
        SceneManager::GetHandle().Create();
    }
    for ( auto& scene : SceneManager::GetHandle().GetData() )
    {
        ImGui::PushID( ( scene.GetName() + std::to_string( scene.GetIndex() ) ).c_str() );
        ImGui::Text( ( "#" + std::to_string( scene.GetIndex() ) ).c_str() );
        ImGui::SameLine();
        if ( ImGui::TreeNode( scene.GetName().c_str() ) )
        {
            if ( ImGui::Selectable( " Create New Entity ... " ) )
            {
                scene.CreateEntity();
            }

            /*
            for ( auto& Tag : ComponentManager::GetHandle().GetVector<TagComponent>() )
            {
                ImGui::Text("%s", Tag.Tag.c_str() );
            }

            for ( auto& Tag : ComponentManager::GetHandle().GetVector<IDComponent>() )
            {
                ImGui::Text("%s", Tag.ID.GetString().c_str() );
            }
            */

            ImGui::TreePop();
        }
        ImGui::PopID();
    }

    ImGui::SeparatorText( " System " );
    for ( auto system : SystemManager::GetHandle().GetData() )
    {
        ImGui::Text( Name::Get( typeid( *system ) ).c_str() );
    }
	ImGui::End();

    ImGui::ShowDemoWindow();
}

MainUIHandler& MainUIHandler::GetHandle() { return m_MainUIHandler; }

MainUIHandler MainUIHandler::m_MainUIHandler;