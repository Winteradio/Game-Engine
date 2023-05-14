#ifndef __FILEBROWSERUIHANDLER_H__
#define __FILEBROWSERUIHANDLER_H__

#include "ImGuiAPI.h"
#include "Log.h"

#include "Manager.h"
#include "FileHandler.h"

class FileBrowserUIHandler
{
    using Data = std::vector< FS::path >;

    private :
        FileBrowserUIHandler();
        ~FileBrowserUIHandler();

    public :
        static FileBrowserUIHandler& GetHandle();
        void Init( int PosX, int PosY, int Width, int Height );
        void Destroy();
        void Update( float DeltaTime );
        void Begin( std::string Extension, bool Mode );
        void End();

    private :
        static FileBrowserUIHandler m_FileBrowserUIHandler;
        Data m_Directories;
        Data m_Files;

        std::set< FS::path > m_SelectedFiles;

        std::string m_Filter;
        std::string m_Extension;

        FS::path m_CurrentPath;
        FS::path m_MainPath;

        char m_Temp[ MAX_LENGTH ];

        int m_Columns;
        int m_ColumnElement;

        bool m_Mode;
        bool m_Rendering;

    public :
        int m_PosX;
        int m_PosY;
        int m_Width;
        int m_Height;
};

#endif // __FILEBROWSERUIHANDLER_H__