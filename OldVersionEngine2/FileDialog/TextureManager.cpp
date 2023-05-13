#include "TextureManager.h"

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {}

void TextureManager::LoadFile( FS::path Path ) {}

void TextureManager::SaveFile( FS::path Path ) {}

void TextureManager::Start( bool Mode ) {}

TextureManager& TextureManager::Get() { return m_TextureManager; }

TextureManager TextureManager::m_TextureManager;