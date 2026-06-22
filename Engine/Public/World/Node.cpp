#include <World/Node.h>

namespace wtr
{
	fmat4 CameraNode::GetViewMatrix() const
	{
		if (!transform)
		{
			return fmat4(1.f);
		}

		const fmat4 rotation = glm::toMat4(transform->GetRotation());

		const fmat4 inverseRotation = glm::transpose(rotation);
		const fmat4 inverseTranslation = glm::translate(fmat4(1.f), -transform->GetPosition());

		const fmat4 viewMatrix = inverseRotation * inverseTranslation;
		return viewMatrix;
	}

	fmat4 CameraNode::GetProjectionMatrix() const
	{
		if (!camera)
		{
			return fmat4(1.f);
		}

		const float far = std::abs(camera->farPlane) <= std::numeric_limits<float>::epsilon() ? 1000.0f : camera->farPlane;
		const float near = std::abs(camera->nearPlane) <= std::numeric_limits<float>::epsilon() ? 0.1f : camera->nearPlane;
		const float width = std::abs(camera->width) <= std::numeric_limits<float>::epsilon() ? 1.0f : camera->width;
		const float height = std::abs(camera->height) <= std::numeric_limits<float>::epsilon() ? 1.0f : camera->height;
		const float aspectRatio = width / height;
		const float fovY = std::abs(camera->fovY) <= std::numeric_limits<float>::epsilon() ? glm::radians(45.f) : camera->fovY;

		if (camera->perspective)
		{
			return util::GetPerspectiveMatrix(fovY, aspectRatio, near, far);
		}
		else
		{
			const float cotHalfFovY = 1.0f / glm::tan(glm::radians(fovY) * 0.5f);

			const float orthoScale = 100;
			const float orthoHeight = orthoScale * near * cotHalfFovY;
			const float orthoWidth = orthoHeight * aspectRatio;

			return util::GetOrthographicMatrix(orthoWidth, orthoHeight, near, far);
		}
	}
}