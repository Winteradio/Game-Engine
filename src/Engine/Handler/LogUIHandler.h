#ifndef __LOGUIHANDLER_H__
#define __LOGUIHANDLER_H__

#include "ImGuiAPI.h"
#include "Log.h"

class LogUIHandler
{
    private :
        LogUIHandler();
        ~LogUIHandler();

    public :
        static LogUIHandler& GetHandle();
        void Init( int PosX, int PosY, int Width, int Height );
        void Destroy();
        void Update( float DeltaTime );

    private :
        static LogUIHandler m_LogUIHandler;

    public :
        int m_PosX;
        int m_PosY;
        int m_Width;
        int m_Height;
};

#endif // __LOGUIHANDLER_H__