#include <Renderer/Proxy/SceneProxy.h>

namespace wtr
{
    SceneProxy::SceneProxy()
        : m_position(0.f)
        , m_rotation(0.f)
        , m_scale(1.f)
    {}

    SceneProxy::~SceneProxy()
    {}

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

    const fmat4 SceneProxy::GetTransform() const
    {
		const fmat4 translation = glm::translate(fmat4(1.f), m_position);

        const fquat quaternion = glm::quat(m_rotation);
		const fmat4 rotation = glm::toMat4(quaternion);
		const fmat4 scale = glm::scale(fmat4(1.f), m_scale);

		const fmat4 transform = translation * rotation * scale;
        return transform;
    }
}