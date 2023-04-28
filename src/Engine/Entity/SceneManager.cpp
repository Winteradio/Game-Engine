#include "SceneManager.h"

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

void SceneManager::Init()
{

}

void SceneManager::Create( std::string Name )
{
	MyUUID ID;
	ID.Init();
	m_Data[ ID ] = Scene( Name, ID );

	Log::Info(" Create Scene ");
	Log::Info(" Name : %s ", Name );
	Log::Info(" ID : %s ", Name, ID.GetID() );
}

void SceneManager::Create( std::string Name, MyUUID ID )
{
	m_Data[ ID ] = Scene( Name, ID );

	Log::Info(" Create Scene ");
	Log::Info(" Name : %s ", Name );
	Log::Info(" ID : %s ", Name, ID.GetID() );
}

void SceneManager::Remove( MyUUID ID )
{
	Scene& TempScene = m_Data[ ID ];

	Log::Info(" Remove Scene ");
	Log::Info(" Name : %s ", TempScene.GetName() );
	Log::Info(" ID : %s ", TempScene.GetID() );
}

void SceneManager::Destroy()
{
	for ( auto ITR : m_Data )
	{
		ITR.second.Destroy();
	}

	m_Data.clear();
}

SceneManager& SceneManager::GetHandle() { return m_SceneManager; }
Scene& SceneManager::GetScene( MyUUID ID ){ return m_Data[ ID ]; }
SceneManager::Data& SceneManager::GetData() { return m_Data; }

SceneManager SceneManager::m_SceneManager;