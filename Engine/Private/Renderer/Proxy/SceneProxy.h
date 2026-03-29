#ifndef __WTR_SCENEPROXY_H__
#define __WTR_SCENEPROXY_H__

#include <ECS/include/Object/Object.h>
#include <Framework/Math/MathTypes.h>
#include <Reflection/include/Type/TypeMacro.h>

#include <cstdint>
namespace wtr
{
	enum eProxyState : uint8_t
	{
		eNone		= 0x00,
		eAdded		= 0x01,
		eReady		= 0x02,
		eRemoved	= 0x03,
		eError		= 0xFF,
	};

	class SceneProxy : public ECS::Object
	{
		GENERATE(SceneProxy);
	public :
		SceneProxy();
		virtual ~SceneProxy();

	public :
		void SetState(const eProxyState state);
		void UpdatePosition(const fvec3 position);
		void UpdateRotation(const fvec3 rotation);
		void UpdateScale(const fvec3 scale);

		eProxyState GetState() const;
		const fvec3 GetPosition() const;
		const fvec3 GetRotation() const;
		const fvec3 GetScale() const;

	private :
		fvec3 m_position;
		fvec3 m_rotation;
		fvec3 m_scale;

		eProxyState m_state;
	};
};

#endif // __WTR_SCENEPROXY_H__