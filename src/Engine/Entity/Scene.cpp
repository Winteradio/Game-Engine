#include "Scene.h"

Scene::Scene() 
{
	m_Name = "Default";
}
Scene::~Scene() {}

void Scene::CreateEntity()
{
	MyUUID ID;
	ID.Init();
	CreateEntity( "Default", ID );
}

void Scene::CreateEntity( MyUUID ID )
{
	CreateEntity( "Default", ID );
}

void Scene::CreateEntity( std::string Name )
{
	MyUUID ID;
	ID.Init();
	CreateEntity( Name, ID );
}

void Scene::CreateEntity( std::string Name, MyUUID ID )
{
	m_Data[ ID ] = Entity( ID, Name );
}

void Scene::AddEntity( Entity Object )
{
	m_Data[ Object.GetID() ] = Object;
}

void Scene::RemoveEntity( MyUUID ID )
{
	bool Check = HasEntity( ID );
	if ( Check )
	{
		auto ITR = m_Data.find( ID );
		m_Data.erase( ITR );
	}
}

bool Scene::HasEntity( MyUUID ID )
{
	if ( m_Data.find( ID ) == m_Data.end() ) return false;
	return true;
}

void Scene::Destroy()
{
	for ( auto ITR : m_Data )
	{
		ITR.second.Destroy();
	}

	m_Data.clear();
}

Entity& Scene::GetEntity( MyUUID ID ) { return m_Data[ ID ]; }
Scene::Data& Scene::GetData() { return m_Data; };
std::string& Scene::GetName() { return m_Name; };
int& Scene::GetIndex() { return m_Index; };

void Scene::SetName( std::string Name ) { m_Name = Name; };
void Scene::SetIndex( int Index ) { m_Index = Index; };