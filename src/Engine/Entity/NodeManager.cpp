#include "NodeManager.h"

NodeManager::NodeManager() {}
NodeManager::~NodeManager() {}

void NodeManager::Init()
{}

void NodeManager::Destroy()
{
	m_Data.clear();
}

NodeManager& NodeManager::GetHandle() { return m_NodeManager; }
NodeManager::Data& NodeManager::GetData() { return m_Data; }

NodeManager NodeManager::m_NodeManager;