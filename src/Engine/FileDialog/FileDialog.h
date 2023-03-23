#ifndef __FILEDIALOG_H__
#define __FILEDIALOG_H__

#include "Utils.h"

namespace FS = std::filesystem;

class FileDialog
{
	private :
		FileDialog();
		~FileDialog();

	public :
		static void PathGoUp( FS::path& Path );
		static void PathGoDown( FS::path FolderPath, FS::path& MainPath );
		static std::vector< FS::path > GetDirectories( FS::path Path );
		static std::vector< FS::path > GetFiles( std::string Filter, FS::path Path );
		static bool CheckFilter( std::string Filter, FS::path Path );
		static std::string GetFileName( FS::path Path );
		static bool IsPathFile( FS::path Path );
		static bool Compare( FS::path OnePath, FS::path OtherPath );
	
	private :
		static FileDialog m_FileDialog;
		static std::vector< FS::path > m_Directories;
		static std::vector< FS::path > m_Files;
};

#endif // __FILEDIALOG_H__