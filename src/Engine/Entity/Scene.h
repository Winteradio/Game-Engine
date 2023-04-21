#ifndef __SCENE_H__
#define __SCENE_H__

#include "Log.h"
#include "Entity.h"

class Scene
{
	using EntityData = std::map< MyUUID, Entity, MyUUIDCompare >;

	public :
		Scene();
		Scene( MyUUID ID );
		Scene( std::string Name );
		Scene( std::string Name, MyUUID ID );
		~Scene();

	// Entity
	public :
		void AddEntity( Entity Object );
		void RemoveEntity( MyUUID ID );

		EntityData& GetEntities();
		Entity& GetEntity( MyUUID ID );

		void SetID( MyUUID ID );
		void SetName( std::string Name );

		std::string& GetID();
		std::string& GetName();

	private :
		MyUUID m_ID;
		std::string m_Name;
		EntityData m_Data;
};

#endif // __SCENE_H__