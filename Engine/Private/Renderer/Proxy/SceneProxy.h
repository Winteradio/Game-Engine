#ifndef __WTR_SCENEPROXY_H__
#define __WTR_SCENEPROXY_H__

#include <ECS/include/Object/Object.h>

namespace wtr
{
    class SceneProxy : public ECS::Object
    {
    public :
        SceneProxy();
        virtual ~SceneProxy();

    public :
        void Update();
    };
};

#endif // __WTR_SCENEPROXY_H__