#ifndef __LIGHTMANAGER_H__
#define __LIGHTMANAGER_H__

#include "Utils.h"
#include "Component.h"
#include "MyUUID.h"

class LightManager
{
	private :
		LightManager();
		~LightManager();

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

		Light*& Get( MyUUID ID );

	private :
		std::map< MyUUID, Light*, MyUUIDCompare > m_LightData;
};

#endif // __LIGHTMANAGER_H__