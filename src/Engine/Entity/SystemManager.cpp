#include "SystemManager.h"

SystemManager::SystemManager()
{

}

SystemManager::~SystemManager()
{

}

void SystemManager::Init()
{

}

void SystemManager::Destroy()
{

}

SystemManager& SystemManager::GetHandle() { return m_SystemManager; }
SystemManager::Data& SystemManager::GetData() { return m_Data; }

SystemManager SystemManager::m_SystemManager;
