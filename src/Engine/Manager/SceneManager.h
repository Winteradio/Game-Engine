#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Utils.h"
#include "Scene.h"
#include "MyUUID.h"

class SceneManager
{
	public :
		SceneManager();
		~SceneManager();

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

		Scene*& Get( MyUUID ID );

	private :
		std::map< MyUUID, Scene*, MyUUIDCompare > m_SceneData;
};

#endif // __SCENEMANAGER_H__