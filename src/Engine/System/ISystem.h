#ifndef __ISYSTEM_H__
#define __ISYSTEM_H__

#include "Component.h"
#include "Node.h"
#include "MyUUID.h"

class ISystem
{
    public :
        ISystem() {}
        virtual ~ISystem() {}

    public :
        virtual void Init() = 0;
};

#endif // __ISYSTEM_H__