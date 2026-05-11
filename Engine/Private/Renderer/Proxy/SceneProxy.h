#ifndef __WTR_SCENEPROXY_H__
#define __WTR_SCENEPROXY_H__

#include <ECS/include/Object/Object.h>
#include <Framework/Math/MathTypes.h>
#include <Reflection/include/Type/TypeMacro.h>
#include <Memory/include/Pointer/RefPtr.h>

#include <cstdint>

namespace wtr
{
	enum class eRenderDirty : uint8_t;

	struct RawData;
	class RenderScene;
};

namespace wtr
{
	class SceneProxy : public ECS::Object
	{
		GENERATE(SceneProxy);
	public :
		SceneProxy(const ECS::UUID& id);
		virtual ~SceneProxy();

	public :
		virtual bool IsUploadable() const = 0;
		virtual bool IsSyncable() const = 0;

	public :
		void OnAttached(RenderScene* owner);
		void OnDetached();

		void UpdatePosition(const fvec3 position);
		void UpdateRotation(const fquat rotation);
		void UpdateRotation(const fvec3 rotation);
		void UpdateScale(const fvec3 scale);

		const fvec3 GetPosition() const;
		const fquat GetRotation() const;
		const fvec3 GetScale() const;
		const fmat4 GetTransform() const;

	protected :
		void OnUpdate();
		void UpdateTransform();

		void SetDirty(const eRenderDirty dirty);
		void ClearDirty();
		const eRenderDirty GetDirty() const;

	private :
		fvec3 m_position;
		fquat m_rotation;
		fvec3 m_scale;
		fmat4 m_transform;

		RenderScene* m_owner;

		eRenderDirty m_dirty;
	};

	struct SceneProxyHasher
	{
		size_t operator()(const SceneProxy& proxy) const;
		size_t operator()(const Memory::RefPtr<SceneProxy>& refProxy) const;
	};
};

#endif // __WTR_SCENEPROXY_H__