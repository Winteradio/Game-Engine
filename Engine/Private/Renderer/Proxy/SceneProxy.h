#ifndef __WTR_SCENEPROXY_H__
#define __WTR_SCENEPROXY_H__

#include <Renderer/Proxy/RenderProxy.h>

#include <Framework/Math/MathTypes.h>

namespace wtr
{
	class SceneProxy : public RenderProxy
	{
		GENERATE(SceneProxy);
	public :
		SceneProxy(const ECS::UUID& id);
		virtual ~SceneProxy();

	public :
		void UpdatePosition(const fvec3 position);
		void UpdateRotation(const fquat rotation);
		void UpdateRotation(const fvec3 rotation);
		void UpdateScale(const fvec3 scale);

		const fvec3 GetPosition() const;
		const fquat GetRotation() const;
		const fvec3 GetScale() const;
		const fmat4 GetTransform() const;

	protected :
		void UpdateTransform();

	private :
		fvec3 m_position;
		fquat m_rotation;
		fvec3 m_scale;
		fmat4 m_transform;
	};
};

#endif // __WTR_SCENEPROXY_H__