#ifndef __SYSTEMMANAGER_H__
#define __SYSTEMMANAGER_H__

#include "Log.h"
#include "ISystem.h"

#include "RenderSystem.h"
#include "CameraSystem.h"

class SystemManager
{
    using Data = std::map< MyUUID, ISystem*, MyUUIDCompare >;

	private :
        SystemManager();
        ~SystemManager();

    public :
        static SystemManager& GetHandle();
        void Init();
        void Destroy();

    public :
        void Create( ISystem* Other );
        void Create( ISystem* Other, MyUUID ID );
        
        void Remove( MyUUID ID );
        bool Has( MyUUID ID );

        ISystem* Get( MyUUID ID );
        Data& GetData();

    private :
        static SystemManager m_SystemManager;
        Data m_Data;
};

#endif // __SYSTEMMANAGER_H__