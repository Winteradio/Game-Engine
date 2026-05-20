#ifndef __WTR_RENDERPROXY_H__
#define __WTR_RENDERPROXY_H__

#include <ECS/include/Object/Object.h>
#include <Reflection/include/Type/TypeMacro.h>
#include <Memory/include/Pointer/RefPtr.h>

#include <cstdint>

namespace wtr
{
	enum class eRenderDirty : uint8_t;

	class RenderScene;
};

namespace wtr
{
	class RenderProxy : public ECS::Object
	{
		GENERATE(RenderProxy);
	public :
		RenderProxy(const ECS::UUID& id);
		virtual ~RenderProxy();

	public :
		virtual bool IsUploadable() const = 0;
		virtual bool IsSyncable() const = 0;

	public :
		void OnAttached(RenderScene* owner);
		void OnDetached();

	protected :
		void OnUpdate();

		void SetDirty(const eRenderDirty dirty);
		void ClearDirty();
		const eRenderDirty GetDirty() const;

	private :
		RenderScene* m_owner;

		eRenderDirty m_dirty;
	};

	struct ProxyHasher
	{
		size_t operator()(const RenderProxy& proxy) const;
		size_t operator()(const Memory::RefPtr<RenderProxy>& refProxy) const;
	};
};

#endif // __WTR_RENDERPROXY_H__