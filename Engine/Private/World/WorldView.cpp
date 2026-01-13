#include <World/WorldView.h>

namespace wtr
{
	View::View()
		: name("main")
		, cameraID()
		, sceneID()
		, posX(0)
		, posY(0)
		, width(1080)
		, height(800)
		, active(true)
	{}

	View::View(const std::string& name, const ECS::UUID& cameraID, const ECS::UUID& sceneID)
		: name(name)
		, cameraID(cameraID)
		, sceneID(sceneID)
		, posX(0)
		, posY(0)
		, width(1080)
		, height(800)
		, active(true)
	{}

	ViewContainer::ViewContainer()
		: m_storage()
	{}

	ViewContainer::~ViewContainer()
	{}

	View& ViewContainer::Create(const std::string& name, const ECS::UUID& cameraID, const ECS::UUID& sceneID)
	{
		auto [itr, inserted] = m_storage.TryEmplace(name);
		if (inserted)
		{
			itr->second = View(name, cameraID, sceneID);
		}

		return itr->second;
	}

	void ViewContainer::Remove(const std::string& name)
	{
		m_storage.Erase(name);
	}

	View& ViewContainer::GetView(const std::string& name)
	{
		auto itr = m_storage.Find(name);
		if (itr != m_storage.End())
		{
			return itr->second;
		}
		else
		{
			static View null;
			return null;
		}
	}

	const View& ViewContainer::GetView(const std::string& name) const
	{
		auto itr = m_storage.Find(name);
		if (itr != m_storage.End())
		{
			return itr->second;
		}
		else
		{
			static View null;
			return null;
		}
	}

	ViewContainer::Storage& ViewContainer::GetView()
	{
		return m_storage;
	}

	const ViewContainer::Storage& ViewContainer::GetView() const
	{
		return m_storage;
	}

	bool ViewContainer::HasView(const std::string& name) const
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