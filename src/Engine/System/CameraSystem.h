#ifndef __CAMERASYSTEM_H__
#define __CAMERASYSTEM_H__

#include "DirectXAPI.h"

#include "ISystem.h"
#include "NodeManager.h"

class CameraSystem : public ISystem
{
    public :
        CameraSystem();
        CameraSystem( MyUUID );
        virtual ~CameraSystem();

    public :
        virtual void Update( float DeltaTime );
};

#endif // __CAMERASYSTEM_H__