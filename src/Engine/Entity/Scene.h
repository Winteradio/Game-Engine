#ifndef __SCENE_H__
#define __SCENE_H__

#include "Log.h"
#include "Entity.h"
#include "MyUUID.h"

class Scene
{
	using Data = std::map< MyUUID, Entity, MyUUIDCompare >;

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

		Data& GetData();
		Entity& GetEntity( MyUUID ID );

		void SetID( MyUUID ID );
		void SetName( std::string Name );

		void Destroy();

		std::string& GetID();
		std::string& GetName();

	private :
		MyUUID m_ID;
		std::string m_Name;
		Data m_Data;
};

#endif // __SCENE_H__