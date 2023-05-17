#ifndef __ASSETHANDLER_H__
#define __ASSETHANDLER_H__

#include "Log.h"

#define YAML_CPP_STATIC_DEFINE

#include "yaml-cpp/yaml.h"
#include "yaml-cpp/Parser.h"

#include "Manager.h"
#include "FileHandler.h"

class AssetHandler
{
    private :
        AssetHandler();
        ~AssetHandler();

    public :
        static AssetHandler& GetHandle();
        void Init();
        void Destroy();

    public :
        void Load( FS::path Path );
        void Save( FS::path Path );

        void LoadYAML( FS::path Path );
        void SaveYAML( FS::path Path );

        void LoadLOG( FS::path Path );
        void SaveLOG( FS::path Path );

        void LoadOBJ( FS::path Path );
        void SaveOBJ( FS::path Path );

        void LoadJPG( FS::path Path );
        void SaveJPG( FS::path Path );

    public :

    private :
        static AssetHandler m_AssetHandler;
};

#endif // __ASSETHANDLER_H__