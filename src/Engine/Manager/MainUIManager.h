#ifndef __MAINUIMANAGER_H__
#define __MAINUIMANAGER_H__

class MainUIManager
{
    private :
        MainUIManager();
        ~MainUIManager();

    public :
        static MainUIManager& GetHandle();
        void Init();
        void Destroy();

    private :
        static MainUIManager m_MainUIManager;
};

#endif // __MAINUIMANAGER_H__