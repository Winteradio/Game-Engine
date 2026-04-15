#include <World/System/CameraSystem.h>

namespace wtr
{
	void CameraSystem::UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container)
	{
		auto& storage = container->GetStorage();
		for (auto& cameraNode : storage)
		{
			// TODO
		}
	}
};