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
}

void AssetHandler::SaveYAML( FS::path Path )
{
    Log::Info(" Save File %s ", Path.generic_string().c_str() );

    YAML::Emitter Data;
    Data << YAML::BeginSeq;
    for ( auto scene : SceneManager::GetHandle().GetData() )
    {
        Data << YAML::BeginMap;
        Data << YAML::Key << "Index" << YAML::Value << scene.GetIndex();
        Data << YAML::Key << "Name" << YAML::Value << scene.GetName();
        Data << YAML::EndMap;
    }
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