#include "Scene.h"

Scene::Scene() 
{
	m_Name = "Default";
}

Scene::Scene( int Index ) : m_Index( Index ) {}
Scene::Scene( int Index, std::string Name ) : m_Index( Index ), m_Name( Name ) {}
Scene::Scene( std::string Name ) : m_Name( Name ) {}

Scene::~Scene() {}

void Scene::Init()
{

}

void Scene::Destroy()
{
	ClearEntity();
	ClearSystem();
}

void Scene::Update( float DeltaTime )
{
	
}

void Scene::RegisterEntity( MyUUID ID )
{
	bool Check = HasEntity( ID );
	if ( !Check ) m_EntityData.insert( ID );
}

void Scene::RegisterSystem( MyUUID ID )
{
	bool Check = HasSystem( ID );
	if ( !Check ) m_SystemData.push_back( ID );
}

void Scene::RemoveEntity( MyUUID ID )
{
	bool Check = HasEntity( ID );
	if ( Check ) 
	{
		auto ITR = m_EntityData.find( ID );
		m_EntityData.erase( ITR );
	}
}

void Scene::RemoveSystem( MyUUID ID )
{
	bool Check = HasSystem( ID );
	if ( Check ) 
	{
		auto ITR = std::remove_if( 
			m_SystemData.begin(), m_SystemData.end(), 
			[&]( const MyUUID& Other ) { return Other == ID; } );
		m_SystemData.erase( ITR, m_SystemData.end() );
	}
}

bool Scene::HasEntity( MyUUID ID )
{
	auto ITR = m_EntityData.find( ID );
	if ( ITR != m_EntityData.end() ) return true;
	else return false;
}

bool Scene::HasSystem( MyUUID ID )
{
	auto ITR = std::find( m_SystemData.begin(), m_SystemData.end(), ID );
	if ( ITR != m_SystemData.end() ) return true;
	else return false;
}

void Scene::ClearEntity() { m_EntityData.clear(); }
void Scene::ClearSystem() { m_SystemData.clear(); }

Scene::EntityData& Scene::GetRegisteredEntities() { return m_EntityData; }
Scene::SystemData& Scene::GetRegisteredSystems() { return m_SystemData; }

std::string& Scene::GetName() { return m_Name; };
int& Scene::GetIndex() { return m_Index; };

void Scene::SetName( std::string Name ) { m_Name = Name; };
void Scene::SetIndex( int Index ) { m_Index = Index; };