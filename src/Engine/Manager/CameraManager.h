#ifndef __CAMERAMANAGER_H__
#define __CAMERAMANAGER_H__

#include "Utils.h"
#include "Component.h"
#include "MyUUID.h"

class CameraManager
{
	private :
		CameraManager();
		~CameraManager();

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

		Camera*& Get( MyUUID ID );

	private :
		std::map< MyUUID, Camera*, MyUUIDCompare > m_CameraData;
};

#endif // __CAMERAMANAGER_H__