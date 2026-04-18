#include <World/Node.h>

namespace wtr
{
	const fmat4 CameraNode::GetViewMatrix() const
	{
		if (!transform)
		{
			return fmat4(1.f);
		}

		const fquat quaternion = glm::quat(transform->GetRotation());
		const fmat4 rotation = glm::toMat4(quaternion);

		const fmat4 inverseRotation = glm::transpose(rotation);
		const fmat4 inverseTranslation = glm::translate(fmat4(1.f), -transform->GetPosition());

		const fmat4 viewMatrix = inverseRotation * inverseTranslation;
		return viewMatrix;
	}

	const fmat4 CameraNode::GetProjectionMatrix() const
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
		if (std::abs(far - near) <= std::numeric_limits<float>::epsilon())
		{
			return fmat4(1.0f);
		}

		static float prevFovY = 0.0f;
		static float cotHalfFovY = 1.0f;
		if (std::abs(prevFovY - camera->fovY) > std::numeric_limits<float>::epsilon())
		{
			cotHalfFovY = 1.0f / glm::tan(glm::radians(camera->fovY) * 0.5f);
			prevFovY = camera->fovY;
		}

		fmat4 projMatrix = 1.0f;
		if (camera->perspective)
		{
			projMatrix[0][0] = cotHalfFovY / aspectRatio;
			projMatrix[1][1] = cotHalfFovY;
			projMatrix[2][2] = -(far + near) / (far - near);
			projMatrix[3][2] = -2.0f * far * near / (far - near);
			projMatrix[2][3] = -1.0f;
			projMatrix[3][3] = 0.0f;
		}
		else
		{
			const float nearHeight = 2.0f * near * cotHalfFovY;
			const float nearWidth = nearHeight * aspectRatio;

			projMatrix[0][0] = 2.0f / nearWidth;
			projMatrix[1][1] = 2.0f / nearHeight;
			projMatrix[2][2] = -2.0f / (far - near);
			projMatrix[3][2] = -(far + near) / (far - near);
			projMatrix[2][3] = 0.0f;
			projMatrix[3][3] = 1.0f;
		}

		return projMatrix;
	}
}