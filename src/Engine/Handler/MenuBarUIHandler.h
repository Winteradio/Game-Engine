#ifndef __MENUBARUIHANDLER_H__
#define __MENUBARUIHANDLER_H__

#include "ImGuiAPI.h"
#include "Log.h"
#include "Manager.h"

class MenuBarUIHandler
{
    private :
        MenuBarUIHandler();
        ~MenuBarUIHandler();

    public :
        static MenuBarUIHandler& GetHandle();
        void Init( int PosX, int PosY, int Width, int Height );
        void Destroy();
        void Update( float DeltaTime );

    private :
        static MenuBarUIHandler m_MenuBarUIHandler;

    public :
        int m_PosX;
        int m_PosY;
        int m_Width;
        int m_Height;
};

#endif // __MENUBARUIHANDLER_H__