#ifndef __IUISYSTEM_H__
#define __IUISYSTEM_H__

#include "ImGuiAPI.h"
#include "ISystem.h"

class IUISystem
{
    public :
        IUISystem() {}
        virtual ~IUISystem() {}

    public :
        virtual void Update(){}
        void Begin() {}
        void End() {}

};

#endif // __IUISYSTEM_H__