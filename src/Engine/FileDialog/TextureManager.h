#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include "FileDialog.h"
#include "ResourceManager.h"

class TextureManager
{
	private :
		TextureManager();
		~TextureManager();

	public :
		static TextureManager& Get();
		void LoadFile( FS::path Path );
		void SaveFile( FS::path Path );
		void Start( bool Mode );

	private :
		static TextureManager m_TextureManager;
};

#endif // __TEXTUREMANAGER_H__