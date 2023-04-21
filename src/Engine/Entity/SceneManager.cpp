#include "SceneManager.h"

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

void SceneManager::Create( std::string Name )
{
	MyUUID ID;
	ID.Init();
	m_Data[ ID ] = Scene( ID, Name );

	Log::Info(" Create Scene ");
	Log::Info(" Name : %s ", Name );
	Log::Info(" ID : %s ", Name, ID.GetID() );
}

void SceneManager::Create( std::string Name, MyUUID ID )
{
	m_Data[ ID ] = Scene( ID, Name );

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

Scene& SceneManager::GetScene( MyUUID ID )
{
	return m_Data[ ID ];
}

SceneData& SceneManager::GetScenes()
{
	return m_Data;
}