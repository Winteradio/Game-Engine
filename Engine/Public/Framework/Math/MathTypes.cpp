#include <Framework/Math/MathTypes.h>

#include <limits>

namespace wtr
{
	namespace util
	{
		fmat4 GetPerspectiveMatrix(const float fovY, const float width, const float height, const float near, const float far)
		{
			if (std::abs(height - 0.f) <= std::numeric_limits<float>::epsilon())
			{
				return fmat4(1.f);
			}

			const float aspectRatio = width / height;
			
			return GetPerspectiveMatrix(fovY, aspectRatio, near, far);
		}

		fmat4 GetPerspectiveMatrix(const float fovY, const float aspectRatio, const float near, const float far)
		{
			if (std::abs(fovY) <= std::numeric_limits<float>::epsilon() ||
				std::abs(aspectRatio) <= std::numeric_limits<float>::epsilon() ||
				(std::abs(far - near) <= std::numeric_limits<float>::epsilon()))
			{
				return fmat4(1.f);
			}

			const float cotHalfFovY = 1.0f / glm::tan(glm::radians(fovY) * 0.5f);

			fmat4 projMatrix = 1.f;

			projMatrix[0][0] = cotHalfFovY / aspectRatio;
			projMatrix[1][1] = cotHalfFovY;
			projMatrix[2][2] = -(far + near) / (far - near);
			projMatrix[2][3] = -1.0f;
			projMatrix[3][2] = -2.0f * far * near / (far - near);
			projMatrix[3][3] = 0.0f;

			return projMatrix;
		}

		fmat4 GetOrthographicMatrix(const float width, const float height, const float near, const float far)
		{
			if (std::abs(width) <= std::numeric_limits<float>::epsilon() ||
				std::abs(height) <= std::numeric_limits<float>::epsilon() ||
				std::abs(far - near) <= std::numeric_limits<float>::epsilon())
			{
				return fmat4(1.f);
			}

			fmat4 projMatrix = 1.f;

			projMatrix[0][0] = 2.0f / width;
			projMatrix[1][1] = 2.0f / height;
			projMatrix[2][2] = -2.0f / (far - near);
			projMatrix[3][2] = -(far + near) / (far - near);
			projMatrix[3][3] = 1.0f;

			return projMatrix;
		}

		fmat4 GetLookAt(const fvec3 pos, const fquat angle)
		{
			const fmat4 invRotation = glm::transpose(glm::toMat4(angle));
			const fmat4 invTranslation = glm::translate(glm::mat4(1.f), -1.f * pos);

			return invRotation * invTranslation;
		}

		fmat4 GetLookAt(const fvec3 pos, const fvec3 dir)
		{
			const fvec3& forwardDirection = GetForwardDirection();
			const fmat4 invRotation = glm::transpose(GetRotation(forwardDirection, dir));
			const fmat4 invTranslation = glm::translate(glm::mat4(1.f), -1.f * pos);

			return invRotation * invTranslation;
		}

		fmat4 GetLookAt(const fvec3 pos, const fvec3 focus, const fvec3 up)
		{
			const float lengthForward = glm::length(focus - pos);
			const float lengthUp = glm::length(up);
			if (std::abs(lengthForward) <= std::numeric_limits<float>::epsilon() ||
				std::abs(lengthUp) <= std::numeric_limits<float>::epsilon())
			{
				return fmat4(1.f);
			}

			const fvec3 axisForward = glm::normalize(focus - pos);
			
			const fvec3 axisUp = glm::normalize(up);
			const fvec3 axisX = glm::cross(axisForward, axisUp);
			const fvec3 axisY = glm::cross(axisX, axisForward);
			const fvec3 axisZ = -1.f * axisForward;

			fmat4 viewMatrix = 1.f;
			viewMatrix[0][0] = axisX.x;
			viewMatrix[1][0] = axisX.y;
			viewMatrix[2][0] = axisX.z;

			viewMatrix[0][1] = axisY.x;
			viewMatrix[1][1] = axisY.y;
			viewMatrix[2][1] = axisY.z;
			
			viewMatrix[0][2] = axisZ.x;
			viewMatrix[1][2] = axisZ.y;
			viewMatrix[2][2] = axisZ.z;

			viewMatrix[3][0] = -1.f * glm::dot(axisX, pos);
			viewMatrix[3][1] = -1.f * glm::dot(axisY, pos);
			viewMatrix[3][2] = -1.f * glm::dot(axisZ, pos);

			return viewMatrix;
		}

		fmat4 GetRotation(const fvec3 baseDir, const fvec3 nextDir)
		{
			const fvec3 unitBase = glm::normalize(baseDir);
			const fvec3 unitNext = glm::normalize(nextDir);

			const fvec3 axis = glm::cross(unitBase, unitNext);
			const float lengthAxis = glm::length(axis);
			if (std::abs(lengthAxis) <= std::numeric_limits<float>::epsilon())
			{
				return fmat4(1.f);
			}

			const float cosValue = glm::dot(unitBase, unitNext);
			const float sinValue = glm::sqrt(1.f - cosValue * cosValue);

			const fquat angle = fquat(cosValue, sinValue * axis);

			return glm::toMat4(angle);
		}

		fmat4 GetRotation(const fvec3 axis, const float degree)
		{
			const float radian = glm::radians(degree);

			const float cosValue = glm::cos(radian);
			const float sinValue = glm::sqrt(1.f - cosValue * cosValue);

			const fquat angle = fquat(cosValue, sinValue * axis);

			return glm::toMat4(angle);
		}

		fmat4 GetTranslation(const fvec3 offset)
		{
			return glm::translate(glm::mat4(1.f), offset);
		}

		fmat4 GetTranslation(const fvec3 fromPos, const fvec3 toPos)
		{
			return glm::translate(glm::mat4(1.f), toPos - fromPos);
		}

		fmat4 GetScale(const fvec3 scale)
		{
			return glm::scale(glm::mat4(1.f), scale);
		}

		const fvec3& GetForwardDirection()
		{
			static const fvec3 FORWARD_DIRECTION = fvec3(0.f, 0.f, - 1.f);

			return FORWARD_DIRECTION;
		}

		const fquat& GetForwardAngle()
		{
			static const fquat FORWARD_ANGLE = fquat(0.f, -1.f * GetForwardDirection());

			return FORWARD_ANGLE;
		}
	}
}