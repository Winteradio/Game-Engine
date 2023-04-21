#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Scene.h"

class SceneManager
{
	using SceneData = std::map< MyUUID, Scene, MyUUIDCompare >;

	public :
		SceneManager();
		~SceneManager();

	public :
		void Create( std::string Name );
		void Create( MyUUID ID, std::string Name );

		void Remove( MyUUID ID );
		Scene& GetScene( MyUUID ID );
		SceneData& GetScenes();

	private :
		SceneData m_Data;
};

#endif __SCENEMANAGER_H__