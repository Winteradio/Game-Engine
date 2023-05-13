#ifndef __MAINUIHANDLER_H__
#define __MAINUIHANDLER_H__

#include "ImGuiAPI.h"
#include "Log.h"
#include "Name.h"
#include "Manager.h"

class MainUIHandler
{
    private :
        MainUIHandler();
        ~MainUIHandler();

    public :
        static MainUIHandler& GetHandle();
        void Init( int PosX, int PosY, int Width, int Height );
        void Destroy();
        void Update( float DeltaTime );

    private :
        static MainUIHandler m_MainUIHandler;

    public :
        int m_PosX;
        int m_PosY;
        int m_Width;
        int m_Height;
};

#endif // __MAINUIHANDLER_H__