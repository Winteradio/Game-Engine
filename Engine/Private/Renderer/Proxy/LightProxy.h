#ifndef __WTR_LIGHTPROXY_H__
#define __WTR_LIGHTPROXY_H__

#include <Renderer/Proxy/SceneProxy.h>

namespace wtr
{
    class LightProxy : public SceneProxy
    {
        GENERATE(LightProxy);
    public :
        LightProxy();
        virtual ~LightProxy();

    public :
    };
};

#endif // __WTR_LIGHTPROXY_H__