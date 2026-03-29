#include <Renderer/Proxy/SceneProxy.h>

namespace wtr
{
    SceneProxy::SceneProxy()
        : m_state(eProxyState::eNone)
        , m_position(0.f)
        , m_rotation(0.f)
        , m_scale(1.f)
    {}

    SceneProxy::~SceneProxy()
    {}

    void SceneProxy::SetState(const eProxyState state)
    {
        m_state = state;
    }

    void SceneProxy::UpdatePosition(const fvec3 position)
    {
        m_position = position;
    }

    void SceneProxy::UpdateRotation(const fvec3 rotation)
    {
        m_rotation = rotation;
    }

    void SceneProxy::UpdateScale(const fvec3 scale)
    {
        m_scale = scale;
    }

    eProxyState SceneProxy::GetState() const
    {
        return m_state;
    }

    const fvec3 SceneProxy::GetPosition() const
    {
        return m_position;
    }

    const fvec3 SceneProxy::GetRotation() const
    {
        return m_rotation;
    }

    const fvec3 SceneProxy::GetScale() const
    {
        return m_scale;
    }
}