#include <World/Node.h>
#include <World/Scene.h>

namespace wtr
{
	void SceneNode::SetScene(Scene* owner)
	{
		m_owner = owner;
	}

	void SceneNode::Clear()
	{
		transform = nullptr;
		m_owner = nullptr;
	}

	void SceneNode::UpdatePosition(const fvec3& position)
	{
		if (!transform)
		{
			return;
		}

		if (transform->position == position)
		{
			return;
		}

		transform->position = position;

		if (m_owner)
		{
			m_owner->Update(GetID());
		}
	}

	void SceneNode::UpdateRotation(const fvec3& rotation)
	{
		if (!transform)
		{
			return;
		}

		if (transform->rotation == rotation)
		{
			return;
		}

		transform->rotation = rotation;

		if (m_owner)
		{
			m_owner->Update(GetID());
		}
	}

	void SceneNode::UpdateScale(const fvec3& scale)
	{
		if (!transform)
		{
			return;
		}

		if (transform->scale == scale)
		{
			return;
		}

		transform->scale = scale;

		if (m_owner)
		{
			m_owner->Update(GetID());
		}
	}

	const fvec3 SceneNode::GetPosition() const
	{
		if (!transform)
		{
			return fvec3(0.0f);
		}

		return transform->position;
	}

	const fvec3 SceneNode::GetRotation() const
	{
		if (!transform)
		{
			return fvec3(0.0f);
		}

		return transform->rotation;
	}

	const fvec3 SceneNode::GetScale() const
	{
		if (!transform)
		{
			return fvec3(1.0f);
		}

		return transform->scale;
	}
}