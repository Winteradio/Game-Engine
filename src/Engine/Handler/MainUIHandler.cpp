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
            ImGui::SeparatorText(" Registered Entities ... ");
            for ( auto ID : scene.GetRegisteredEntities() )
            {
                ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 155, 155, 155, 255 ) );
                ImGui::Text( ID.GetString().substr( 0, 6 ).c_str() );
                ImGui::PopStyleColor();               
                ImGui::SameLine();
                ImGui::Text( EntityManager::GetHandle().GetEntity( ID )->GetName().c_str() );
            }
            ImGui::SeparatorText(" Registered Systems ... ");

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

        if ( ImGui::BeginPopupContextItem() )
        {
            ImGui::SeparatorText(" Register to Scene ... ");
            for ( auto& scene : SceneManager::GetHandle().GetData() )
            {
                ImGui::PushID( ( scene.GetName() + std::to_string( scene.GetIndex() ) ).c_str() );
                if ( ImGui::Selectable( ( "#" + std::to_string( scene.GetIndex() ) + " " + scene.GetName() ).c_str() ) )
                {
                    scene.RegisterEntity( ID );
                }
                ImGui::PopID();
            } 
            ImGui::EndPopup();
        }
        ImGui::PopID();
    }


    ImGui::SeparatorText( " System " );
    for ( auto& [ ID, System ] : SystemManager::GetHandle().GetData() )
    {
        ImGui::PushID( System->GetID().GetString().c_str() );
        ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 155, 155, 155, 255 ) );
        ImGui::Text( System->GetID().GetString().substr( 0, 6 ).c_str() );
        ImGui::PopStyleColor();
        ImGui::SameLine();

        if ( ImGui::Selectable( Name::Get( typeid( *System ) ).c_str() ) )
        {
            PropertyUIHandler::GetHandle().SetID( System->GetID() );
        }

        if ( ImGui::BeginPopupContextItem() )
        {
            ImGui::SeparatorText(" Register to Scene ... ");
            for ( auto& scene : SceneManager::GetHandle().GetData() )
            {
                ImGui::PushID( ( scene.GetName() + std::to_string( scene.GetIndex() ) ).c_str() );
                if ( ImGui::Selectable( ( "#" + std::to_string( scene.GetIndex() ) + " " + scene.GetName() ).c_str() ) )
                {
                    scene.RegisterSystem( ID );
                }
                ImGui::PopID();
            } 
            ImGui::EndPopup();
        }
        ImGui::PopID();
    }
	ImGui::End();

}

MainUIHandler& MainUIHandler::GetHandle() { return m_MainUIHandler; }

MainUIHandler MainUIHandler::m_MainUIHandler;