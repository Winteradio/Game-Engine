#include "ComponentManager.h"

ComponentManager::ComponentManager() {}
ComponentManager::~ComponentManager() {}

void ComponentManager::Init()
{}

void ComponentManager::Destroy()
{}

ComponentManager& ComponentManager::GetHandle() { return m_ComponentManager; }
ComponentManager::Data& ComponentManager::GetData() { return m_Data; }

ComponentManager ComponentManager::m_ComponentManager;