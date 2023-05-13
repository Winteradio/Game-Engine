#include "EntityManager.h"

EntityManager::EntityManager() {}

EntityManager::~EntityManager() {}

void EntityManager::Init()
{
    Log::Info(" Init - Entity Manager ");
}

void EntityManager::Destroy()
{
    for ( auto EntityITR : m_Data )
    {
        EntityITR.second.Destroy();
    }

    m_Data.clear();
}

void EntityManager::CreateEntity()
{
    MyUUID ID;
    ID.Init();
    CreateEntity( ID, "Default" );
}

void EntityManager::CreateEntity( MyUUID ID )
{
    CreateEntity( ID, "Default" );
}

void EntityManager::CreateEntity( std::string Name )
{
    MyUUID ID;
    ID.Init();
    CreateEntity( ID, "Default" );
}

void EntityManager::CreateEntity( MyUUID ID, std::string Name )
{
    m_Data[ ID ] = Entity( ID, Name );
}

void EntityManager::AddEntity( Entity Other )
{
    m_Data[ Other.GetID() ] = Other;
}

void EntityManager::RemoveEntity( MyUUID ID )
{
    bool Check = HasEntity( ID );
    if ( Check )
    {
        auto EntityITR = m_Data.find( ID );
        m_Data[ID].Destroy();
        m_Data.erase( EntityITR );
    }

}

bool EntityManager::HasEntity( MyUUID ID )
{
    if ( m_Data.find( ID ) != m_Data.end() ) return true;
    else return false;
}

Entity* EntityManager::GetEntity( MyUUID ID )
{
    bool Check = HasEntity( ID );
    if ( Check )
    {
        return &m_Data[ ID ];
    }
    else
    {
        Log::Warn(" There is no Entity of this ID ", ID.GetString().c_str() );
        return nullptr;
    }
}

EntityManager::Data& EntityManager::GetData() { return m_Data; }

EntityManager& EntityManager::GetHandle() { return m_EntityManager; }

EntityManager EntityManager::m_EntityManager;