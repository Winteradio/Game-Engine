#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Scene.h"

class SceneManager
{
	using Data = std::map< MyUUID, Scene, MyUUIDCompare >;

	private :
		SceneManager();
		~SceneManager();

	public :
		void Init();
		void Create( std::string Name );
		void Create( std::string Name, MyUUID ID );
		void Remove( MyUUID ID );
		void Destroy();

		static SceneManager& GetHandle();
		Scene& GetScene( MyUUID ID );
		Data& GetData();

	private :
		static SceneManager m_SceneManager;
		Data m_Data;
};

#endif __SCENEMANAGER_H__