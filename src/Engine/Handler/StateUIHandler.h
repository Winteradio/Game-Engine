#ifndef __STATEUIHANDLER_H__
#define __STATEUIHANDLER_H__

#include "ImGuiAPI.h"
#include "Log.h"
#include "Manager.h"

class StateUIHandler
{
    private :
        StateUIHandler();
        ~StateUIHandler();

    public :
        static StateUIHandler& GetHandle();
        void Init( int PosX, int PosY, int Width, int Height );
        void Destroy();
        void Update( float DeltaTime );

    private :
        static StateUIHandler m_StateUIHandler;

    public :
        int m_PosX;
        int m_PosY;
        int m_Width;
        int m_Height;
};

#endif // __STATEUIHANDLER_H__