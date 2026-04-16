#ifndef __WTR_CAMERASYSTEM_H__
#define __WTR_CAMERASYSTEM_H__

#include <World/System/System.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
	class CameraSystem : public InputSystem<CameraNode>
	{
	public:
		CameraSystem() = default;
		virtual ~CameraSystem() = default;

		void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container) override;

	protected:
		virtual const fvec3 UpdatePosition(const ECS::TimeStep& timeStep, Memory::ObjectPtr<SceneComponent> transform);
		virtual const fvec3 UpdateRotation(const ECS::TimeStep& timeStep, Memory::ObjectPtr<SceneComponent> transform);
	};
};

#endif // __WTR_CAMERASYSTEM_H__