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
			transform->UpdateRotation({0.f, static_cast<float>(seconds * 0.1f), 0.f});
		}
	}
};