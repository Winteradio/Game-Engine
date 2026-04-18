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
			return {};
		}

		const fvec3 prevRotation = transform->GetRotation();

		const fvec3 forward = glm::normalize(- 1.0f * fvec3(
			std::cos(prevRotation.x) * std::sin(prevRotation.y),
			-1.0f * std::sin(prevRotation.x),
			std::cos(prevRotation.x) * std::cos(prevRotation.y)));

		const fvec3 right = glm::normalize(glm::cross(forward, fvec3(0.f, 1.f, 0.f)));
		const fvec3 up = glm::normalize(glm::cross(right, forward));

		fvec3 diffPosition = inputStorage->GetMouseScroll() * forward;

		if (inputStorage->IsDown(eKeyCode::eKey_W))
		{
			diffPosition += forward;
		}

		if (inputStorage->IsDown(eKeyCode::eKey_S))
		{
			diffPosition += -forward;
		}

		if (inputStorage->IsDown(eKeyCode::eKey_A))
		{
			diffPosition += -right;
		}

		if (inputStorage->IsDown(eKeyCode::eKey_D))
		{
			diffPosition += right;
		}

		if (diffPosition != fvec3(0.f))
		{
			const float seconds = ECS::TimeStep::ToSecond(timeStep.delta);
			const float smoothFactor = 1.f - std::exp(-5.0f * seconds);

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
			return {};
		}

		const fvec2 mouseDelta = inputStorage->GetMouseDelta();

		if (inputStorage->IsDown(eKeyCode::eKey_MouseLeft))
		{
			static constexpr float mouseSensitivity = 0.005f;

			const float seconds = ECS::TimeStep::ToSecond(timeStep.delta);
			const float smoothFactor = 1.f - std::exp(-5.f * seconds);
			
			const fvec3 prevRotation = transform->GetRotation();
			const fvec3 targetRotation = prevRotation + fvec3(-mouseDelta.y, -mouseDelta.x, 0.f) * mouseSensitivity;

			const fquat prevQuat = glm::quat(prevRotation);
			const fquat targetQuat = glm::quat(targetRotation);

			const fquat currQuat = glm::slerp(prevQuat, targetQuat, smoothFactor);
			const fvec3 currRotation = glm::eulerAngles(currQuat);

			return currRotation;
		}

		return transform->GetRotation();
	}
};