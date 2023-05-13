#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Log.h"
#include "MyUUID.h"
#include "Entity.h"

class EntityManager
{
    using Data = std::map< MyUUID, Entity, MyUUIDCompare >;

    private :
        EntityManager();
        ~EntityManager();

    public :
        static EntityManager& GetHandle();
        void Init();
        void Destroy();

    public :
        void CreateEntity();
        void CreateEntity( MyUUID ID );
        void CreateEntity( std::string Name );
        void CreateEntity( MyUUID ID, std::string Name );

        void AddEntity( Entity Other );
        void RemoveEntity( MyUUID ID );
        bool HasEntity( MyUUID ID );

        Entity* GetEntity( MyUUID ID );
        Data& GetData();

    public :
        static EntityManager m_EntityManager;
        Data m_Data;  
};

#endif // __ENTITYMANAGER_H__