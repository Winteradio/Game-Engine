#include <Framework/Player.h>

#include <Framework/ViewController.h>
#include <Framework/Math/MathTypes.h>
#include <World/Component.h>
#include <World/Entity.h>

#include <limits>

namespace wtr
{
	Player::Player()
		: m_views()
		, m_entity()
		, m_active(true)
	{}

	Player::~Player()
	{}

	void Player::SetEntity(Memory::ObjectPtr<Entity> entity)
	{
		if (entity)
		{
			m_entity = entity;
		}
	}

	Memory::ObjectPtr<const Entity> Player::GetEntity() const
	{
		return m_entity;
	}

	Memory::ObjectPtr<const SceneComponent> Player::GetTransform() const
	{
		if (!m_entity)
		{
			return {};
		}

		return m_entity->GetComponent<SceneComponent>();
	}

	Memory::ObjectPtr<const CameraComponent> Player::GetCamera() const
	{
		if (!m_entity)
		{
			return {};
		}

		return m_entity->GetComponent<CameraComponent>();
	}

	const fmat4 Player::GetViewMatrix() const
	{
		Memory::ObjectPtr<const SceneComponent> transform = GetTransform();
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

	const fmat4 Player::GetProjectionMatrix() const
	{
		Memory::ObjectPtr<const CameraComponent> camera = GetCamera();
		if (!camera)
		{
			return fmat4(1.f);
		}
	
		const float far = std::abs(camera->farPlane) <= std::numeric_limits<float>::epsilon() ? 1000.0f : camera->farPlane;
		const float near = std::abs(camera->nearPlane) <= std::numeric_limits<float>::epsilon() ? 0.1f : camera->nearPlane;
		const float width = std::abs(camera->width) <= std::numeric_limits<float>::epsilon() ? 1.0f : camera->width;
		const float height = std::abs(camera->height) <= std::numeric_limits<float>::epsilon() ? 1.0f : camera->height;

		if (std::abs(far - near) <= std::numeric_limits<float>::epsilon())
		{
			return fmat4(1.0f);
		}

		fmat4 projMatrix = 1.0f;
		if (camera->perspective)
		{
			projMatrix[0][0] = 2.0f * far / width;
			projMatrix[1][1] = 2.0f * far / height;
			projMatrix[2][2] = -(far + near) / (far - near);
			projMatrix[3][2] = -2.0f * far * near / (far - near);
			projMatrix[2][3] = -1.0f;
			projMatrix[3][3] = 0.0f;
		}
		else
		{
			projMatrix[0][0] = 2.0f / width;
			projMatrix[1][1] = 2.0f / height;
			projMatrix[2][2] = -2.0f / (far - near);
			projMatrix[3][2] = -(far + near) / (far - near);
			projMatrix[2][3] = 0.0f;
			projMatrix[3][3] = 1.0f;
		}

		return projMatrix;
	}

	void Player::Register(Memory::RefPtr<ViewInfo> viewInfo)
	{
		if (!viewInfo)
		{
			return;
		}

		m_views.Insert(viewInfo->GetName());
	}

	void Player::Unregister(Memory::RefPtr<ViewInfo> viewInfo)
	{
		if (!viewInfo)
		{
			return;
		}

		Unregister(viewInfo->GetName());
	}

	void Player::Unregister(const std::string& viewName)
	{
		m_views.Erase(viewName);
	}

	void Player::UnregisterAll()
	{
		m_views.Clear();
	}

	const wtr::HashSet<std::string>& Player::GetViews() const
	{
		return m_views;
	}

	void Player::Activate()
	{
		m_active = true;
	}

	void Player::Deactivate()
	{
		m_active = false;
	}

	bool Player::IsActive() const
	{
		return m_active;
	}
}