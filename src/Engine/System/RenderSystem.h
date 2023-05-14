#ifndef __RENDERSYSTEM_H__
#define __RENDERSYSTEM_H__

#include "DirectXAPI.h"

#include "ISystem.h"
#include "NodeManager.h"

class RenderSystem : public ISystem
{
    public :
        RenderSystem();
        RenderSystem( MyUUID ID );
        virtual ~RenderSystem();

    public :
        virtual void Update( float DeltaTime );
};

#endif // __RENDERSYSTEM_H__