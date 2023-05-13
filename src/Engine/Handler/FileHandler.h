#ifndef __FILEHANDLER_H__
#define __FILEHANDLER_H__

#include "Log.h"

namespace FS = std::filesystem;

class FileHandler
{
    private :
        FileHandler();
        ~FileHandler();

    public :
        static FileHandler& GetHandle();
        void Init();
        void Destroy();

    public :
		void PathGoUp( FS::path& Path );
		void PathGoDown( FS::path FolderPath, FS::path& MainPath );
		std::vector< FS::path > GetDirectories( FS::path Path );
		std::vector< FS::path > GetFiles( std::string Filter, FS::path Path );
		bool CheckFilter( std::string Filter, FS::path Path );
		std::string GetFileName( FS::path Path );
		bool IsPathFile( FS::path Path );
		bool Compare( FS::path OnePath, FS::path OtherPath );


    public :
        static FileHandler m_FileHandler;
        std::vector< FS::path > m_Directories;
        std::vector< FS::path > m_Files;        
};

#endif // __FILEHANDLER_H__