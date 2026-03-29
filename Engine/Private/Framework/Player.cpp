#include <Framework/Player.h>

#include <Framework/ViewController.h>
#include <World/Component.h>
#include <World/Entity.h>

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

	Memory::ObjectPtr<const TransformComponent> Player::GetTransform() const
	{
		if (!m_entity)
		{
			return {};
		}

		return m_entity->GetComponent<TransformComponent>();
	}

	Memory::ObjectPtr<const CameraComponent> Player::GetCamera() const
	{
		if (!m_entity)
		{
			return {};
		}

		return m_entity->GetComponent<CameraComponent>();
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