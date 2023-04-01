#ifndef __OBJECTMANAGER_H__
#define __OBJECTMANAGER_H__

#include "Utils.h"
#include "Component.h"
#include "MyUUID.h"

class ObjectManager
{
	private :
		ObjectManager();
		~ObjectManager();

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

		Object*& Get( MyUUID ID );

	private :
		std::map< MyUUID, Object*, MyUUIDCompare > m_ObjectData;
};

#endif // __OBJECTMANAGER_H__