#include <World/WorldScene.h>

namespace wtr
{
	SceneContainer::SceneContainer()
		: m_storage()
	{}

	SceneContainer::~SceneContainer()
	{}

	ECS::Scene& SceneContainer::Create(const std::string& name)
	{
		auto [itr, inserted] = m_storage.TryEmplace(name);
		if (inserted)
		{
			itr->second = ECS::Scene(name);
		}

		return itr->second;
	}

	void SceneContainer::Remove(const std::string& name)
	{
		m_storage.Erase(name);
	}

	ECS::Scene& SceneContainer::GetScene(const std::string& name)
	{
		auto itr = m_storage.Find(name);
		if (itr != m_storage.End())
		{
			return itr->second;
		}
		else
		{
			static ECS::Scene null;
			return null;
		}
	}

	const ECS::Scene& SceneContainer::GetScene(const std::string& name) const
	{
		auto itr = m_storage.Find(name);
		if (itr != m_storage.End())
		{
			return itr->second;
		}
		else
		{
			static ECS::Scene null;
			return null;
		}
	}

	SceneContainer::Storage& SceneContainer::GetScene()
	{
		return m_storage;
	}

	const SceneContainer::Storage& SceneContainer::GetScene() const
	{
		return m_storage;
	}

	bool SceneContainer::HasScene(const std::string& name) const
	{
		auto itr = m_storage.Find(name);
		if (itr != m_storage.End())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}