#ifndef __ASSETHANDLER_H__
#define __ASSETHANDLER_H__

#include "Log.h"

class AssetHandler
{
    enum class FILETYPE
    {
        OBJ,
        YAML,
    };

    private :
        AssetHandler();
        ~AssetHandler();

    public :
        static AssetHandler& GetHandle();
        void Init();
        void Destroy();

    public :
        template < FILETYPE Type >
        void Load( const std::string& FilePath );

    private :
        static AssetHandler m_AssetHandler;
};

#endif // __ASSETHANDLER_H__