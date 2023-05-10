#ifndef __ISYSTEM_H__
#define __ISYSTEM_H__

#include "Log.h"
#include "Component.h"
#include "Node.h"
#include "MyUUID.h"

class ISystem
{
    public :
        ISystem() {}
        virtual ~ISystem() {}

    public :
        virtual void Update( float DeltaTime ) = 0;
};

#endif // __ISYSTEM_H__