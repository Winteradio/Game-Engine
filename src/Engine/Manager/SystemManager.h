#ifndef __SYSTEMMANAGER_H__
#define __SYSTEMMANAGER_H__

#include "Log.h"
#include "ISystem.h"

#include "RenderSystem.h"
#include "CameraSystem.h"

class SystemManager
{
    using Data = std::vector< ISystem* >;

	private :
        SystemManager();
        ~SystemManager();

    public :
        static SystemManager& GetHandle();

        void Add( ISystem* Other );
        void SwapByIndex( int First, int Last );
        void Remove( ISystem* Other );

        void Init();
        void Destroy();
        void Update( float DeltaTime );

        Data& GetData();

    private :
        static SystemManager m_SystemManager;
        Data m_Data;
};

#endif // __SYSTEMMANAGER_H__