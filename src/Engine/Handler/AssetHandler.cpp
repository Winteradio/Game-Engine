#include "AssetHandler.h"

AssetHandler::AssetHandler()
{

}

AssetHandler::~AssetHandler()
{

}

void AssetHandler::Init()
{

}

void AssetHandler::Destroy()
{
    
}

template<>
void AssetHandler::Load< AssetHandler::FILETYPE::OBJ >( const std::string& FilePath )
{

}

AssetHandler& AssetHandler::GetHandle() { return m_AssetHandler; }

AssetHandler AssetHandler::m_AssetHandler;