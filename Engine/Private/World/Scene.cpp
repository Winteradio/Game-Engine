#include <World/Scene.h>

namespace wtr
{
	Scene::Scene()
		: ECS::Scene()
	{}

	Scene::Scene(const std::string& name)
		: ECS::Scene(name)
	{}

	Scene::~Scene()
	{}

	void Scene::RegisterView(const ViewInfo& view)
	{
		m_viewMap.Emplace(view.name, view);
	}

	void Scene::RemoveView(const std::string name)
	{
		m_viewMap.Erase(name);
	}

	const ViewInfo& Scene::GetView(const std::string& name) const
	{
		auto itr = m_viewMap.Find(name);
		if (itr != m_viewMap.End())
		{
			return itr->second;
		}
		else
		{
			static ViewInfo null;
			return null;
		}
	}

	const wtr::HashMap<std::string, ViewInfo>& Scene::GetView() const
	{
		return m_viewMap;
	}

	SceneContainer::SceneContainer()
		: m_storage()
	{}

	SceneContainer::~SceneContainer()
	{}

	Scene& SceneContainer::Create(const std::string& name)
	{
		auto [itr, inserted] = m_storage.TryEmplace(name);
		if (inserted)
		{
			itr->second = Scene(name);
		}

		return itr->second;
	}

	void SceneContainer::Remove(const std::string& name)
	{
		m_storage.Erase(name);
	}

	Scene& SceneContainer::GetScene(const std::string& name)
	{
		auto itr = m_storage.Find(name);
		if (itr != m_storage.End())
		{
			return itr->second;
		}
		else
		{
			static Scene null;
			return null;
		}
	}

	const Scene& SceneContainer::GetScene(const std::string& name) const
	{
		auto itr = m_storage.Find(name);
		if (itr != m_storage.End())
		{
			return itr->second;
		}
		else
		{
			static Scene null;
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