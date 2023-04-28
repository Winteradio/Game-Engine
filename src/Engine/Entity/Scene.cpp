#include "Scene.h"

Scene::Scene() {}

Scene::Scene( MyUUID ID ) : m_ID( ID )
{}

Scene::Scene( std::string Name ) : m_Name( Name )
{}

Scene::Scene( std::string Name, MyUUID ID ) : m_Name( Name ), m_ID( ID )
{}

Scene::~Scene() {}

void Scene::AddEntity( Entity Object )
{
	m_Data[ Object.GetID() ] = Object;
}

void Scene::RemoveEntity( MyUUID ID )
{
	
}

void Scene::SetID( MyUUID ID ) { m_ID = ID; }
void Scene::SetName( std::string Name ) { m_Name = Name; }

void Scene::Destroy()
{
	for ( auto ITR : m_Data )
	{
		ITR.second.Destroy();
	}

	m_Data.clear();
}

Scene::Data& Scene::GetData() { return m_Data; }
Entity& Scene::GetEntity( MyUUID ID ) { return m_Data[ ID ]; }
std::string& Scene::GetID() { return m_ID.GetID(); }
std::string& Scene::GetName() { return m_Name; }