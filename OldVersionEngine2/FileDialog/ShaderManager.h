#ifndef __SHADERMANAGER_H__
#define __SHADERMANAGER_H__

#include "FileDialog.h"
#include "ResourceManager.h"

class ShaderManager
{
	private :
		ShaderManager();
		~ShaderManager();

	public :
		static ShaderManager& Get();
		void LoadFile( FS::path Path );
		void SaveFile( FS::path Path );
		void Start( bool Mode );

	private :
		static ShaderManager m_ShaderManager;
};

#endif // __SHADERMANAGER_H__