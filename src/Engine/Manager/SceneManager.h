#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Scene.h"

class SceneManager
{
	using Data = std::vector< Scene >;

	private :
		SceneManager();
		~SceneManager();

	public :
		static SceneManager& GetHandle();
		void Init();
		void Destroy();

	public :
		void CreateScene();
		void CreateScene( int Index );
		void CreateScene( std::string Name );
		void CreateScene( int Index, std::string Name);
		void RemoveScene( int Index );
		Scene& GetScene( int Index );

		Data& GetData();
		size_t GetCount();

	private :
		static SceneManager m_SceneManager;
		Data m_Data;
};

#endif __SCENEMANAGER_H__