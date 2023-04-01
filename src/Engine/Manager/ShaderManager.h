#ifndef __SHADERMANAGER_H__
#define __SHADERMANAGER_H__

#include "Utils.h"
#include "Component.h"
#include "MyUUID.h"

class ShaderManager
{
	private :
		ShaderManager();
		~ShaderManager();

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

		Shader*& Get( MyUUID ID );

	private :
		std::map< MyUUID, Shader*, MyUUIDCompare > m_ShaderData;
};

#endif // __SHADERMANAGER_H__