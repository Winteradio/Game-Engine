#ifndef __FILEHANDLER_H__
#define __FILEHANDLER_H__

#include "Log.h"
#include <stdexcept> // For std::runtime_error
#include <system_error> // For std::filesystem::filesystem_error

namespace FS = std::filesystem;

class FileHandler
{
    using Data = std::vector< FS::path >;

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

		bool CheckFilter( std::string Filter, FS::path Path );
		bool IsPathFile( FS::path Path );

        std::vector< FS::path > UpdateDirectories( FS::path Path );
        std::vector< FS::path > UpdateFiles( std::string Filter, FS::path Path );

		Data GetDirectories();
		Data GetFiles();
		std::string GetFileName( FS::path Path );
        std::string GetFileExtension( FS::path Path );

    public :
        static FileHandler m_FileHandler;
        Data m_Directories;
        Data m_Files;        
};

static	bool Compare( FS::path OnePath, FS::path OtherPath );

#endif // __FILEHANDLER_H__