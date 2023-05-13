#ifndef __PROPERTYUIHANDLER_H__
#define __PROPERTYUIHANDLER_H__

#include "ImGuiAPI.h"
#include "Log.h"
#include "Manager.h"

class PropertyUIHandler
{
    private :
        PropertyUIHandler();
        ~PropertyUIHandler();

    public :
        static PropertyUIHandler& GetHandle();
        void Init( int PosX, int PosY, int Width, int Height );
        void Destroy();
        void Update( float DeltaTime );

    private :
        static PropertyUIHandler m_PropertyUIHandler;
    
    public :
        int m_PosX;
        int m_PosY;
        int m_Width;
        int m_Height;
};

#endif // __PROPERTYUIHANDLER_H__