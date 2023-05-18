#include "SystemManager.h"

SystemManager::SystemManager()
{

}

SystemManager::~SystemManager()
{

}

void SystemManager::Init()
{
    Log::Info(" Init - System Manager ");
}

void SystemManager::Destroy()
{
    for ( auto& [ ID, System ] : m_Data )
    {
        delete System;
    }

    m_Data.clear();
}

void SystemManager::Create( ISystem* Other )
{
    if ( Other->GetID().GetString().empty() )
    {
        MyUUID ID;
        ID.Init();
        Create( Other, ID );
    }
    else
    {
        Create( Other, Other->GetID() );
    }
}

void SystemManager::Create( ISystem* Other, MyUUID ID )
{
    bool Check = Has( ID );
    if ( !Check ) 
    {
        Other->SetID( ID );
        m_Data[ ID ] = Other;
        Log::Info(" Create System %s ", ID.GetString().c_str() );
    }
}

void SystemManager::Remove( MyUUID ID )
{
    bool Check = Has( ID );
    if ( Check )
    {
        auto SystemITR = m_Data.find( ID );
        delete m_Data[ ID ];
        m_Data.erase( SystemITR );
    }
}

bool SystemManager::Has( MyUUID ID )
{
    if ( m_Data.find( ID ) != m_Data.end() ) return true;
    else return false;
}

ISystem* SystemManager::Get( MyUUID ID )
{
    bool Check = Has( ID );
    if ( Check )
    {
        Log::Info( Name::Get( typeid( *m_Data[ID]) ).c_str() );
        return m_Data[ ID ];
    }
    else
    {
        Log::Warn(" There is no System of this ID ", ID.GetString().c_str() );
        return nullptr;
    }
}

SystemManager& SystemManager::GetHandle() { return m_SystemManager; }
SystemManager::Data& SystemManager::GetData() { return m_Data; }

SystemManager SystemManager::m_SystemManager;
