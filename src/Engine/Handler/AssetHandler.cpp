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

template<>
void AssetHandler::Load< AssetHandler::FILETYPE::OBJ >( const std::string& FilePath )
{

}

AssetHandler& AssetHandler::GetHandle() { return m_AssetHandler; }

AssetHandler AssetHandler::m_AssetHandler;