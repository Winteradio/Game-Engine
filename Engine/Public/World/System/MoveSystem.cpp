#include <World/System/MoveSystem.h>

namespace wtr
{
	void MoveSystem::UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container)
	{
		auto& storage = container->GetStorage();

		const double seconds = ECS::TimeStep::ToSecond(timeStep.delta);

		for (auto& [id, meshNode] : storage)
		{
			if (!meshNode || !meshNode->transform)
			{
				continue;
			}

			auto& transform = meshNode->transform;
			auto deltaRotation = glm::angleAxis(static_cast<float>(seconds), fvec3(0.f, 1.f, 0.f));
			auto rotation = transform->GetRotation();
			rotation = deltaRotation * rotation;
			transform->UpdateRotation(rotation);
		}
	}
};