#include <Renderer/Proxy/RenderProxy.h>

#include <Renderer/RenderScene.h>
#include <Renderer/RenderTypes.h>

namespace wtr
{
    RenderProxy::RenderProxy(const ECS::UUID& id)
        : ECS::Object(id)
        , m_owner(nullptr)
        , m_dirty(eRenderDirty::eNone)
    {
    }

    RenderProxy::~RenderProxy()
    {
    }

    void RenderProxy::OnAttached(RenderScene* owner)
    {
        m_owner = owner;
    }

    void RenderProxy::OnDetached()
    {
        m_owner = nullptr;
    }

    void RenderProxy::SetDirty(const eRenderDirty dirty)
    {
        m_dirty |= dirty;
    }

    void RenderProxy::ClearDirty()
    {
        m_dirty = eRenderDirty::eNone;
    }

    const eRenderDirty RenderProxy::GetDirty() const
    {
        return m_dirty;
    }

    void RenderProxy::OnUpdate()
    {
        if (nullptr != m_owner)
        {
            m_owner->UpdateProxy(this->GetID());
        }
    }

    size_t ProxyHasher::operator()(const Memory::RefPtr<RenderProxy>& refProxy) const
    {
        if (!refProxy)
        {
            return 0;
        }

        const Reflection::TypeInfo* typeInfo = refProxy->GetTypeInfo();
        size_t seed = std::hash<ECS::UUID>()(refProxy->GetID());
        seed ^= typeInfo->GetTypeHash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        return seed;
    }
}