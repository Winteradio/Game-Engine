#include "SystemManager.h"

SystemManager::SystemManager()
{

}

SystemManager::~SystemManager()
{

}

void SystemManager::Init()
{
    Create( new CameraSystem() );
    Create( new RenderSystem() );

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
    MyUUID ID;
    ID.Init();
    Create( Other, ID );
}

void SystemManager::Create( ISystem* Other, MyUUID ID )
{
    bool Check = Has( ID );
    if ( !Check ) 
    {
        Other->SetID( ID );
        m_Data[ ID ] = Other;
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
