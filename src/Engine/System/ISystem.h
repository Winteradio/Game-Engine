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
        std::string& GetName() { return m_Name; }
        void SetID( MyUUID ID ) { m_ID = ID; }
        void SetName( std::string Name ) { m_Name = Name; }

    protected :
        MyUUID m_ID;
        std::string m_Name;
};

#endif // __ISYSTEM_H__