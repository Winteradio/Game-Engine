#include "NodeManager.h"

NodeManager::NodeManager() {}
NodeManager::~NodeManager() {}

void NodeManager::Init()
{
    Log::Info(" Init - Node Manager ");	
}

void NodeManager::Destroy()
{
	m_Data.clear();
	Log::Info(" Destroy - Node Manager ");
}

NodeManager& NodeManager::GetHandle() { return m_NodeManager; }
NodeManager::Data& NodeManager::GetData() { return m_Data; }

NodeManager NodeManager::m_NodeManager;