#include <World/System/MoveSystem.h>

#include <World/RenderNode.h>
#include <Framework/Input/InputTypes.h>
#include <Framework/Input/InputStorage.h>

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

	void InstancedMoveSystem::UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container)
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

			const size_t instanceCount = transform->GetInstanceCount();
			for (size_t index = 0; index < instanceCount; index++)
			{
				auto rotation = transform->GetRotation(index);
				rotation = deltaRotation * rotation;

				transform->UpdateRotation(index, rotation);
			}
		}
	}

	//void LightMoveSystem::UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container)
	//{
	//	auto& storage = container->GetStorage();

	//	const double seconds = ECS::TimeStep::ToSecond(timeStep.delta);

	//	constexpr float MAX_HEIGHT = 3.5f;
	//	for (auto& [id, lightNode] : storage)
	//	{
	//		if (!lightNode || !lightNode->transform)
	//		{
	//			continue;
	//		}

	//		auto& transform = lightNode->transform;
	//		const auto position = transform->GetPosition();
	//		if (position.y >= 0
	//	}
	//}
};