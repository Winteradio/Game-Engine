#ifndef __SCENE_H__
#define __SCENE_H__

#include "Log.h"
#include "MyUUID.h"

#include "SystemManager.h"

class Scene
{
	using EntityData = std::set< MyUUID, MyUUIDCompare >;
	using SystemData = std::vector< MyUUID >;

	public :
		Scene();
		Scene( int Index );
		Scene( std::string Name );
		Scene( int Index, std::string Name );
		~Scene();

	public :
		void Init();
		void Destroy();
		void Update( float DeltaTime );
	
	public :
		void RegisterEntity( MyUUID ID );
		void RegisterSystem( MyUUID ID );

		void RemoveEntity( MyUUID ID );
		void RemoveSystem( MyUUID ID );
		
		bool HasEntity( MyUUID ID );
		bool HasSystem( MyUUID ID );

		void ClearEntity();
		void ClearSystem();

	public :
		EntityData& GetRegisteredEntities();
		SystemData& GetRegisteredSystems();
		std::string& GetName();
		int& GetIndex();

		void SetName( std::string Name );
		void SetIndex( int Index );

	private :
		EntityData m_EntityData;
		SystemData m_SystemData;
		std::string m_Name;
		int m_Index;
};

#endif // __SCENE_H__