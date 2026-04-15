#include <World/System/CameraSystem.h>

#include <Framework/Input/InputStorage.h>
#include <cmath>

namespace wtr
{
	void CameraSystem::UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container)
	{
		auto& storage = container->GetStorage();
		for (auto& [id, cameraNode] : storage)
		{
			if (!cameraNode || !cameraNode->transform)
			{
				continue;
			}

			auto& transform = cameraNode->transform;

			auto inputStorage = GetInputStorage();
			if (!inputStorage)
			{
				continue;
			}

			if (inputStorage->IsPressed(eKeyCode::eKey_Space))
			{
				transform->UpdatePosition(fvec3(0.f, 0.f, 5.f));
				transform->UpdateRotation(fvec3(0.f, 0.f, 0.f));
			}
			else
			{
				const fvec3 newPosition = UpdatePosition(timeStep, transform);
				transform->UpdatePosition(newPosition);

				const fvec3 newRotation = UpdateRotation(timeStep, transform);
				transform->UpdateRotation(newRotation);
			}
		}
	}

	const fvec3 CameraSystem::UpdatePosition(const ECS::TimeStep& timeStep, Memory::ObjectPtr<SceneComponent> transform)
	{
		auto inputStorage = GetInputStorage();
		if (!inputStorage || !transform)
		{
			return transform->GetPosition();
		}

		const fvec3 prevRotation = transform->GetRotation();

		const fvec3 up = fvec3(0.0f, 1.0f, 0.0f);

		const fvec3 forward = -1.0f * fvec3(
			std::cos(prevRotation.x) * std::sin(prevRotation.y),
			std::sin(prevRotation.x),
			std::cos(prevRotation.x) * std::cos(prevRotation.y));

		const fvec3 right = glm::cross(forward, up);

		fvec3 diffPosition = inputStorage->GetMouseScroll() * forward;

		if (inputStorage->IsDown(eKeyCode::eKey_W))
		{
			diffPosition = forward;
		}

		if (inputStorage->IsDown(eKeyCode::eKey_S))
		{
			diffPosition = -forward;
		}

		if (inputStorage->IsDown(eKeyCode::eKey_A))
		{
			diffPosition = -right;
		}

		if (inputStorage->IsDown(eKeyCode::eKey_D))
		{
			diffPosition = right;
		}

		if (diffPosition != fvec3(0.f))
		{
			const float seconds = ECS::TimeStep::ToSecond(timeStep.delta);
			const float smoothFactor = 1.f - std::exp(-2.5f * seconds);

			const fvec3 currPosition = transform->GetPosition() + diffPosition * smoothFactor;

			return currPosition;
		}
		else
		{
			return transform->GetPosition();
		}
	}

	const fvec3 CameraSystem::UpdateRotation(const ECS::TimeStep& timeStep, Memory::ObjectPtr<SceneComponent> transform)
	{
		auto inputStorage = GetInputStorage();
		if (!inputStorage || !transform)
		{
			return transform->GetRotation();
		}

		const fvec2 mouseDelta = inputStorage->GetMouseDelta();

		if (inputStorage->IsPressed(eKeyCode::eKey_MouseLeft))
		{
			const float seconds = ECS::TimeStep::ToSecond(timeStep.delta);
			const float smoothFactor = 1.f - std::exp(-5.f * seconds);
			
			const fvec3 prevRotation = transform->GetRotation();
			const fvec3 targetRotation = prevRotation + fvec3(-mouseDelta.y, -mouseDelta.x, 0.f) * 0.01f;

			const fquat prevQuat = glm::quat(prevRotation);
			const fquat targetQuat = glm::quat(targetRotation);

			const fquat currQuat = glm::slerp(prevQuat, targetQuat, smoothFactor);
			const fvec3 currRotation = glm::eulerAngles(currQuat);

			return currRotation;
		}

		return transform->GetRotation();
	}
};