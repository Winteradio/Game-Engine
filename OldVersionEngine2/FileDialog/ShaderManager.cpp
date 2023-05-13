#include "ShaderManager.h"

ShaderManager::ShaderManager() {}

ShaderManager::~ShaderManager() {}

void ShaderManager::LoadFile( FS::path Path ) {}

void ShaderManager::SaveFile( FS::path Path ) {}

void ShaderManager::Start( bool Mode ) {}

ShaderManager& ShaderManager::Get() { return m_ShaderManager; }

ShaderManager ShaderManager::m_ShaderManager;