#ifndef __WTR_CAMERASYSTEM_H__
#define __WTR_CAMERASYSTEM_H__

#include <World/System/System.h>

namespace wtr
{
	class CameraSystem : public InputSystem<CameraNode>
	{
	public:
		CameraSystem() = default;
		virtual ~CameraSystem() = default;

		void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container) override;
	};
};

#endif // __WTR_CAMERASYSTEM_H__