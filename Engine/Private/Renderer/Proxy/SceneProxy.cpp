#include <Renderer/Proxy/SceneProxy.h>

#include <Renderer/RenderScene.h>
#include <RHI/RHIResources.h>

namespace wtr
{
    SceneProxy::SceneProxy(const ECS::UUID& id)
        : RenderProxy(id)
        , m_transform()
    {}

    SceneProxy::~SceneProxy()
    {}

    void SceneProxy::UpdatePosition(const fvec3 position)
    {
        if ((std::abs(position.x - m_transform.position.x) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(position.y - m_transform.position.y) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(position.z - m_transform.position.z) >= std::numeric_limits<float>::epsilon()))
        {
            this->m_transform.position = position;

            const bool needDirty = this->GetDirty() != eRenderDirty::eTransform;
            if (needDirty)
            {
                this->SetDirty(eRenderDirty::eTransform);
                this->OnUpdate();
            }
        }
    }

    void SceneProxy::UpdateRotation(const fquat rotation)
    {
        if ((std::abs(rotation.x - m_transform.rotation.x) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(rotation.y - m_transform.rotation.y) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(rotation.z - m_transform.rotation.z) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(rotation.w - m_transform.rotation.w) >= std::numeric_limits<float>::epsilon()))
        {
            this->m_transform.rotation = rotation;

            const bool needDirty = this->GetDirty() != eRenderDirty::eTransform;
            if (needDirty)
            {
                this->SetDirty(eRenderDirty::eTransform);
                this->OnUpdate();
            }
        }
    }

    void SceneProxy::UpdateRotation(const fvec3 rotation)
    {
        const fvec3 eulerAngles = glm::eulerAngles(m_transform.rotation);
        if ((std::abs(rotation.x - eulerAngles.x) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(rotation.y - eulerAngles.y) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(rotation.z - eulerAngles.z) >= std::numeric_limits<float>::epsilon()))
        {
            this->m_transform.rotation = glm::quat(rotation);

            const bool needDirty = this->GetDirty() != eRenderDirty::eTransform;
            if (needDirty)
            {
                this->SetDirty(eRenderDirty::eTransform);
                this->OnUpdate();
            }
        }
    }

    void SceneProxy::UpdateScale(const fvec3 scale)
    {
        if ((std::abs(scale.x - m_transform.scale.x) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(scale.y - m_transform.scale.y) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(scale.z - m_transform.scale.z) >= std::numeric_limits<float>::epsilon()))
        {
            this->m_transform.scale = scale;

            const bool needDirty = this->GetDirty() != eRenderDirty::eTransform;
            if (needDirty)
            {
                this->SetDirty(eRenderDirty::eTransform);
                this->OnUpdate();
            }
        }
    }

    const fvec3& SceneProxy::GetPosition() const
    {
        return m_transform.position;
    }

    const fquat& SceneProxy::GetRotation() const
    {
        return m_transform.rotation;
    }

    const fvec3& SceneProxy::GetScale() const
    {
        return m_transform.scale;
    }

    const ftransform& SceneProxy::GetTransform() const
    {
        return m_transform;
    }
}