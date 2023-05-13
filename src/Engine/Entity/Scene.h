#ifndef __SCENE_H__
#define __SCENE_H__

#include "Log.h"
#include "Entity.h"
#include "MyUUID.h"

#include "ComponentManager.h"
#include "NodeManager.h"

class Scene
{
	using Data = std::map< MyUUID, Entity, MyUUIDCompare >;

	public :
		Scene();
		~Scene();

	public :
		void Init();
		void Destroy();
	
	public :
		void CreateEntity();
		void CreateEntity( MyUUID ID );
		void CreateEntity( std::string Name );
		void CreateEntity( std::string Name, MyUUID ID );
		
		void AddEntity( Entity Object );
		void RemoveEntity( MyUUID ID );
		bool HasEntity( MyUUID ID );


		Entity& GetEntity( MyUUID ID );
		Data& GetData();
		std::string& GetName();
		int& GetIndex();

		void SetName( std::string Name );
		void SetIndex( int Index );

	private :
		Data m_Data;
		std::string m_Name;
		int m_Index;
};

#endif // __SCENE_H__