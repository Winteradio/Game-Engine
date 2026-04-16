#ifndef __WTR_SCENEPROXY_H__
#define __WTR_SCENEPROXY_H__

#include <ECS/include/Object/Object.h>
#include <Framework/Math/MathTypes.h>
#include <Reflection/include/Type/TypeMacro.h>

#include <cstdint>
namespace wtr
{
	class SceneProxy : public ECS::Object
	{
		GENERATE(SceneProxy);
	public :
		SceneProxy();
		virtual ~SceneProxy();

	public :
		void UpdatePosition(const fvec3 position);
		void UpdateRotation(const fvec3 rotation);
		void UpdateScale(const fvec3 scale);

		const fvec3 GetPosition() const;
		const fvec3 GetRotation() const;
		const fvec3 GetScale() const;
		const fmat4 GetTransform() const;

	private :
		fvec3 m_position;
		fvec3 m_rotation;
		fvec3 m_scale;
	};

	struct UpdateProxyInfo
	{
		const ECS::UUID id;
		const fvec3 position;
		const fvec3 rotation;
		const fvec3 scale;
	};
};

#endif // __WTR_SCENEPROXY_H__