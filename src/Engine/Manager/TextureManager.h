#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include "Utils.h"
#include "Component.h"
#include "MyUUID.h"

class TextureManager
{
	private :
		TextureManager();
		~TextureManager();

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

		Texture*& Get( MyUUID ID );

	private :
		std::map< MyUUID, Texture*, MyUUIDCompare > m_TextureData;
};

#endif // __TEXTUREMANAGER_H__