#include "SceneManager.h"

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

void SceneManager::Init()
{
    Log::Info(" Init - Scene Manager ");
}

void SceneManager::CreateScene()
{
	CreateScene( (int)m_Data.size() - 1, "Default" );
}

void SceneManager::CreateScene( int Index )
{
	CreateScene( Index, "Default" );
}

void SceneManager::CreateScene( std::string Name )
{
	CreateScene( (int)m_Data.size() -1 , Name );
}

void SceneManager::CreateScene( int Index, std::string Name )
{
	m_Data.emplace_back( Scene( Index, Name ) );

	Log::Info(" Create Scene Name : %s / Index : %d ", m_Data.back().GetName().c_str(), m_Data.back().GetIndex() );
}

void SceneManager::RemoveScene( int Index )
{
	m_Data.erase( m_Data.begin() + Index );

	Log::Info(" Remove Scene ");
	Log::Info(" Name : %s ", Index );
}

void SceneManager::Destroy()
{
	for ( Scene Other : m_Data )
	{
		Other.Destroy();
	}

	m_Data.clear();
}

SceneManager& SceneManager::GetHandle() { return m_SceneManager; }
Scene& SceneManager::GetScene( int Index ){ return m_Data[ Index ]; }
size_t SceneManager::GetCount() { return m_Data.size(); }
SceneManager::Data& SceneManager::GetData() { return m_Data; }

SceneManager SceneManager::m_SceneManager;