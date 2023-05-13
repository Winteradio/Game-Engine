#include "ComponentManager.h"

ComponentManager::ComponentManager() {}
ComponentManager::~ComponentManager() {}

void ComponentManager::Init()
{
    Log::Info(" Init - Component Manager ");
}

void ComponentManager::Destroy()
{
    Log::Info(" Destroy - Component Manager ");
}

ComponentManager& ComponentManager::GetHandle() { return m_ComponentManager; }
ComponentManager::Data& ComponentManager::GetData() { return m_Data; }

ComponentManager ComponentManager::m_ComponentManager;