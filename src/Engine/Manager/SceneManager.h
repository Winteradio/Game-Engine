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
		void Init();
		void Create();
		void Remove( int Index );
		void Destroy();

		static SceneManager& GetHandle();
		Scene& GetScene( int Index );
		Data& GetData();
		size_t GetCount();

	private :
		static SceneManager m_SceneManager;
		Data m_Data;
};

#endif __SCENEMANAGER_H__