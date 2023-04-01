#ifndef __MESHMANAGER_H__
#define __MESHMANAGER_H__

#include "Utils.h"
#include "Component.h"
#include "MyUUID.h"

class MeshManager
{
	private :
		MeshManager();
		~MeshManager();

	public :
		void Init();
		void Render();

	public :
		void Create( std::string Name );
		void Create( std::string Name, MyUUID ID );

		void Destroy( MyUUID ID );

		template< class T >
		void AddComponent( T* Value )
		{

		}

		template<class T >
		void DeleteComponent( MyUUID ID )
		{

		}

		Mesh*& Get( MyUUID ID );

	private :
		std::map< MyUUID, Mesh*, MyUUIDCompare > m_MeshData;
};

#endif // __MESHMANAGER_H__