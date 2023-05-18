#include "AssetHandler.h"

AssetHandler::AssetHandler()
{

}

AssetHandler::~AssetHandler()
{

}

void AssetHandler::Init()
{
    Log::Info(" Init - Asset Handler ");
}

void AssetHandler::Destroy()
{
    Log::Info(" Destroy - Asset Handler ");
}

void AssetHandler::Load( FS::path Path )
{
    std::string Extension = FileHandler::GetHandle().GetFileExtension( Path );

    if ( Extension == "yaml" ) return LoadYAML( Path );
    if ( Extension == "obj" ) return LoadOBJ( Path );
    if ( Extension == "log" ) return LoadLOG( Path );
    if ( Extension == "jpg" ) return LoadJPG( Path );
}

void AssetHandler::Save( FS::path Path )
{
    std::string Extension = FileHandler::GetHandle().GetFileExtension( Path );

    if ( Extension == "yaml" ) return SaveYAML( Path );
    if ( Extension == "obj" ) return SaveOBJ( Path );
    if ( Extension == "log" ) return SaveLOG( Path );
    if ( Extension == "jpg" ) return SaveJPG( Path );
}

void AssetHandler::LoadYAML( FS::path Path )
{
    Log::Info(" Load File %s ", Path.generic_string().c_str() );

    YAML::Node FileData = YAML::LoadFile( Path.generic_string() );

    for ( auto Data : FileData )
    {
        std::string Part = Data["Part"].as<std::string>();

        if ( Part == "Scene" ) LoadScene( Data );
        else if ( Part == "Entity" ) LoadEntity( Data );
        else if ( Part == "System" ) LoadSystem( Data );
    }
}

void AssetHandler::LoadScene( YAML::Node& FileData )
{
    for ( auto Data : FileData["Objects"] )
    {
        int Index = Data["Index"].as<int>();
        std::string Name = Data["Name"].as<std::string>();

        SceneManager::GetHandle().CreateScene( Index, Name );

        for ( auto entity : Data["Entity"] )
        {
            SceneManager::GetHandle().GetScene( Index ).RegisterEntity( entity["UUID"].as<std::string>() );
        }

        for ( auto system : Data["System"] )
        {
            SceneManager::GetHandle().GetScene( Index ).RegisterSystem( system["UUID"].as<std::string>() );
        }
    }
}

void AssetHandler::LoadEntity( YAML::Node& FileData )
{
    for ( auto Data : FileData["Objects"] )
    {
        std::string Name = Data["Name"].as<std::string>();
        MyUUID ID = MyUUID( Data["UUID"].as<std::string>() );

        EntityManager::GetHandle().CreateEntity( ID, Name );
    }
}

void AssetHandler::LoadSystem( YAML::Node& FileData )
{
    for ( auto Data : FileData["Objects"] )
    {
        std::string Type = Data["Type"].as<std::string>();
        MyUUID ID = MyUUID( Data["UUID"].as<std::string>() ); 

        if ( Type == "RenderSystem" )
        {
            SystemManager::GetHandle().Create( new RenderSystem(), ID );
        }
        else if ( Type == "CameraSystem" )
        {
            SystemManager::GetHandle().Create( new CameraSystem(), ID );
        }
    }
}

void AssetHandler::SaveYAML( FS::path Path )
{
    Log::Info(" Save File %s ", Path.generic_string().c_str() );

    YAML::Emitter Data;
    Data << YAML::BeginSeq;

    // Save Scene Objects
    Data << YAML::BeginMap;
    Data << YAML::Key << "Part" << YAML::Value << "Scene";
    Data << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;
    for ( auto scene : SceneManager::GetHandle().GetData() )
    {
        Data << YAML::BeginMap;

        Data << YAML::Key << "Name" << YAML::Value << scene.GetName();
        Data << YAML::Key << "Index" << YAML::Value << scene.GetIndex();

        Data << YAML::Key << "Entity" << YAML::Value;
        Data << YAML::BeginSeq;
        for ( auto ID : scene.GetRegisteredEntities() )
        {
            Data << YAML::BeginMap;
            Data << YAML::Key << "UUID" << YAML::Value << ID.GetString();
            Data << YAML::EndMap;
        }
        Data << YAML::EndSeq;

        Data << YAML::Key << "System" << YAML::Value;
        Data << YAML::BeginSeq;
        for ( auto ID : scene.GetRegisteredSystems() )
        {
            Data << YAML::BeginMap;
            Data << YAML::Key << "UUID" << YAML::Value << ID.GetString();
            Data << YAML::EndMap;        
        }
        Data << YAML::EndSeq;

        Data << YAML::EndMap;
    }
    Data << YAML::EndSeq;
    Data << YAML::EndMap;

    Data << YAML::Newline;

    // Save Entity Objects
    Data << YAML::BeginMap;
    Data << YAML::Key << "Part" << YAML::Value << "Entity";
    Data << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;
    for ( auto [ ID, entity ] : EntityManager::GetHandle().GetData() )
    {
        Data << YAML::BeginMap;

        Data << YAML::Key << "Name" << YAML::Value << entity.GetName();
        Data << YAML::Key << "UUID" << YAML::Value << entity.GetID().GetString();

        Data << YAML::EndMap;
    }
    Data << YAML::EndSeq;
    Data << YAML::EndMap;

    Data << YAML::Newline;

    // Save System Objects
    Data << YAML::BeginMap;
    Data << YAML::Key << "Part" << YAML::Value << "System";
    Data << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;
    for ( auto [ ID, system ] : SystemManager::GetHandle().GetData() )
    {
        Data << YAML::BeginMap;

        Data << YAML::Key << "Type" << YAML::Value << Name::Get( typeid( *system ) );
        Data << YAML::Key << "UUID" << YAML::Value << system->GetID().GetString();

        Data << YAML::EndMap;
    }
    Data << YAML::EndSeq;
    Data << YAML::EndMap;

    Data << YAML::EndSeq;

    std::ofstream fout( Path, std::ios_base::out );
    fout << Data.c_str();
}

void AssetHandler::LoadLOG( FS::path Path )
{
    Log::Info(" Load File %s ", Path.generic_string().c_str() );

    YAML::Node FileData = YAML::LoadFile( Path.generic_string() );
}

void AssetHandler::SaveLOG( FS::path Path )
{
    Log::Info(" Save File %s ", Path.generic_string().c_str() );
}

void AssetHandler::LoadOBJ( FS::path Path )
{
    Log::Info(" Load File %s ", Path.generic_string().c_str() );
}

void AssetHandler::SaveOBJ( FS::path Path )
{
    Log::Info(" Save File %s ", Path.generic_string().c_str() );
}

void AssetHandler::LoadJPG( FS::path Path )
{
    Log::Info(" Load File %s ", Path.generic_string().c_str() );
}

void AssetHandler::SaveJPG( FS::path Path )
{
    Log::Info(" Save File %s ", Path.generic_string().c_str() );
}

AssetHandler& AssetHandler::GetHandle() { return m_AssetHandler; }

AssetHandler AssetHandler::m_AssetHandler;