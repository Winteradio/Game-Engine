#include "SystemManager.h"

SystemManager::SystemManager()
{

}

SystemManager::~SystemManager()
{

}

void SystemManager::Add( ISystem* Other )
{
    m_Data.emplace_back( Other );
}

void SystemManager::SwapByIndex( int First, int Last )
{
    std::swap( m_Data[ First ], m_Data[ Last ] );
}

void SystemManager::Remove( ISystem* Other )
{
    m_Data.erase( 
        std::remove_if( m_Data.begin(), m_Data.end(), 
        [Other]( ISystem* pISystem ) { return pISystem == Other; } ), 
        m_Data.end() 
    );
}

void SystemManager::Init()
{
    Destroy();
    Add( new CameraSystem() );
    Add( new RenderSystem() );

    Log::Info(" Init - System Manager ");
}

void SystemManager::Destroy()
{
    for ( auto System : m_Data )
    {
        delete System;
    }

    m_Data.clear();
}

void SystemManager::Update( float DeltaTime )
{
    for ( auto System : m_Data )
    {
        System->Update( DeltaTime );
    }
}

SystemManager& SystemManager::GetHandle() { return m_SystemManager; }
SystemManager::Data& SystemManager::GetData() { return m_Data; }

SystemManager SystemManager::m_SystemManager;
