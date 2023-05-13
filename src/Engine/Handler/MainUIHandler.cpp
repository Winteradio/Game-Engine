#include "MainUIHandler.h"

#include "PropertyUIHandler.h"

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
    ImGui::PushID( "Create Scene " );
    ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 155, 155, 155, 255 ) );
    if ( ImGui::Selectable( " New ... " ) )
    {
        SceneManager::GetHandle().CreateScene();
    }
    ImGui::PopStyleColor();
    ImGui::PopID();
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

            for ( auto& [ ID, entity ] : scene.GetData() )
            {
                ImGui::PushID( entity.GetID().GetString().c_str() );
				ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 155, 155, 155, 255 ) );
                ImGui::Text( entity.GetID().GetString().substr( 0, 6 ).c_str() );
				ImGui::PopStyleColor();
				ImGui::SameLine();
                if ( ImGui::Selectable( entity.GetName().c_str() ) )
                {
                    PropertyUIHandler::GetHandle().SetID( entity.GetID() );
                }
                ImGui::PopID();
            }

            ImGui::TreePop();
        }
        ImGui::PopID();
    }


    ImGui::SeparatorText(" Entity ");
    ImGui::PushID( "Create Entity " );
    ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 155, 155, 155, 255 ) );
    if ( ImGui::Selectable( " New ... " ) )
    {
        EntityManager::GetHandle().CreateEntity();
    }
    ImGui::PopStyleColor();
    ImGui::PopID();
    for ( auto& [ ID, entity ] : EntityManager::GetHandle().GetData() )
    {
        ImGui::PushID( entity.GetID().GetString().c_str() );
        ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 155, 155, 155, 255 ) );
        ImGui::Text( entity.GetID().GetString().substr( 0, 6 ).c_str() );
        ImGui::PopStyleColor();
        ImGui::SameLine();
        if ( ImGui::Selectable( entity.GetName().c_str() ) )
        {
            PropertyUIHandler::GetHandle().SetID( entity.GetID() );
        }
        ImGui::PopID();
    }


    ImGui::SeparatorText( " System " );
    for ( auto system : SystemManager::GetHandle().GetData() )
    {
        ImGui::Text( Name::Get( typeid( *system ) ).c_str() );
    }
	ImGui::End();

}

MainUIHandler& MainUIHandler::GetHandle() { return m_MainUIHandler; }

MainUIHandler MainUIHandler::m_MainUIHandler;