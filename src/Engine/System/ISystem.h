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
        ISystem( MyUUID ID ) : m_ID( ID ) {};
        virtual ~ISystem() {}

    public :
        virtual void Update( float DeltaTime ) = 0;
        MyUUID& GetID() { return m_ID; }
        void SetID( MyUUID ID ) { m_ID = ID; }

    protected :
        MyUUID m_ID;
};

#endif // __ISYSTEM_H__