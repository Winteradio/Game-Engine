#include <Renderer/Proxy/SceneProxy.h>

#include <Renderer/RenderScene.h>
#include <RHI/RHIResources.h>

namespace wtr
{
    SceneProxy::SceneProxy(const ECS::UUID& id)
        : ECS::Object(id)
        , m_position(0.f)
        , m_rotation(1.f, 0.f, 0.f, 0.f)
        , m_scale(1.f)
        , m_transform(1.f)
        , m_owner(nullptr)
        , m_dirty(eRenderDirty::eNone)
    {}

    SceneProxy::~SceneProxy()
    {}

    void SceneProxy::OnAttached(RenderScene* owner)
    {
        m_owner = owner;
    }

    void SceneProxy::OnDetached()
    {
        m_owner = nullptr;
    }

    void SceneProxy::UpdatePosition(const fvec3 position)
    {
        if ((std::abs(position.x - this->m_position.x) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(position.y - this->m_position.y) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(position.z - this->m_position.z) >= std::numeric_limits<float>::epsilon()))
        {
            this->m_position = position;

            UpdateTransform();
            OnUpdate();
        }
    }

    void SceneProxy::UpdateRotation(const fquat rotation)
    {
        if ((std::abs(rotation.x - this->m_rotation.x) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(rotation.y - this->m_rotation.y) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(rotation.z - this->m_rotation.z) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(rotation.w - this->m_rotation.w) >= std::numeric_limits<float>::epsilon()))
        {
            this->m_rotation = rotation;

            UpdateTransform();
            OnUpdate();
        }
    }

    void SceneProxy::UpdateRotation(const fvec3 rotation)
    {
        const fvec3 eulerAngles = glm::eulerAngles(this->m_rotation);
        if ((std::abs(rotation.x - eulerAngles.x) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(rotation.y - eulerAngles.y) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(rotation.z - eulerAngles.z) >= std::numeric_limits<float>::epsilon()))
        {
            this->m_rotation = glm::quat(rotation);

            UpdateTransform();
            OnUpdate();
        }
    }

    void SceneProxy::UpdateScale(const fvec3 scale)
    {
        if ((std::abs(scale.x - this->m_scale.x) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(scale.y - this->m_scale.y) >= std::numeric_limits<float>::epsilon()) ||
            (std::abs(scale.z - this->m_scale.z) >= std::numeric_limits<float>::epsilon()))
        {
            this->m_scale = scale;

            UpdateTransform();
            OnUpdate();
        }
    }

    void SceneProxy::UpdateTransform()
    {
		const fmat4 translation = glm::translate(fmat4(1.f), m_position);
		const fmat4 rotation = glm::toMat4(m_rotation);
		const fmat4 scale = glm::scale(fmat4(1.f), m_scale);

		m_transform = translation * rotation * scale;

        SetDirty(eRenderDirty::eTransform);
	}

    void SceneProxy::SetDirty(const eRenderDirty dirty)
    {
        m_dirty |= dirty;
    }

    void SceneProxy::ClearDirty()
    {
        m_dirty = eRenderDirty::eNone;
    }

    const eRenderDirty SceneProxy::GetDirty() const
    {
        return m_dirty;
    }

    void SceneProxy::OnUpdate()
    {
        if (nullptr != m_owner)
        {
            m_owner->UpdateProxy(this->GetID());
        }
    }

    const fvec3 SceneProxy::GetPosition() const
    {
        return m_position;
    }

    const fquat SceneProxy::GetRotation() const
    {
        return m_rotation;
    }

    const fvec3 SceneProxy::GetScale() const
    {
        return m_scale;
    }

    const fmat4 SceneProxy::GetTransform() const
    {
        return m_transform;
	}

    size_t SceneProxyHasher::operator()(const SceneProxy& proxy) const
    {
        return std::hash<ECS::UUID>()(proxy.GetID());
    }

    size_t SceneProxyHasher::operator()(const Memory::RefPtr<SceneProxy>& refProxy) const
    {
        if (!refProxy)
        {
            return 0;
        }

        return std::hash<ECS::UUID>()(refProxy->GetID());
    }
}