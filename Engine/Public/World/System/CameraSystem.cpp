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
				transform->UpdateRotation(fquat(1.f, 0.f, 0.f, 0.f));
			}
			else
			{
				const fvec3 newPosition = UpdatePosition(timeStep, transform);
				transform->UpdatePosition(newPosition);

				const fquat newRotation = UpdateRotation(timeStep, transform);
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

		const fquat prevRotation = transform->GetRotation();

		const fvec3 forward = glm::normalize(prevRotation * fvec3(0.f, 0.f, -1.f));
		const fvec3 right = glm::normalize(prevRotation * fvec3(1.f, 0.f, 0.f));
		const fvec3 up = glm::normalize(prevRotation * fvec3(0.f, 1.f, 0.f));

		constexpr float wheelSensitivity = 10.0f;

		fvec3 diffPosition = inputStorage->GetMouseScroll() * forward * wheelSensitivity;

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

	const fquat CameraSystem::UpdateRotation(const ECS::TimeStep& timeStep, Memory::ObjectPtr<SceneComponent> transform)
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
			
			const fquat prevRotation = transform->GetRotation();

			const fvec3 worldUp = fvec3(0.f, 1.f, 0.f);
			const fvec3 right = glm::normalize(prevRotation * fvec3(1.f, 0.f, 0.f));

			const fquat yawDelta = glm::angleAxis(-mouseDelta.x * mouseSensitivity, worldUp);
			const fquat pitchDelta = glm::angleAxis(-mouseDelta.y * mouseSensitivity, right);

			const fquat currRotation = yawDelta * pitchDelta * prevRotation;

			return currRotation;
		}

		return transform->GetRotation();
	}
};