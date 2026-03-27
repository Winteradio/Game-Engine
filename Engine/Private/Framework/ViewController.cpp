#include <Framework/ViewController.h>

#include <Framework/Player.h>
#include <Renderer/RenderView.h>
#include <World/Entity.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>
namespace wtr
{
	ViewInfo::ViewInfo()
		: m_players()
		, m_name("")
		, m_posX(0)
		, m_posY(0)
		, m_width(1080)
		, m_height(800)
		, m_active(true)
	{}

	void ViewInfo::SetName(const std::string& name)
	{
		m_name = name;
	}

	void ViewInfo::SetPosition(const uint16_t posX, const uint16_t posY)
	{
		m_posX = posX;
		m_posY = posY;
	}

	void ViewInfo::SetSize(const uint16_t width, const uint16_t height)
	{
		m_width = width;
		m_height = height;
	}

	void ViewInfo::Activate()
	{
		m_active = true;
	}

	void ViewInfo::Deactivate()
	{
		m_active = false;
	}

	void ViewInfo::Register(Memory::RefPtr<Player> player)
	{
		if (!player)
		{
			return;
		}
		
		auto entity = player->GetEntity();
		if (!entity)
		{
			return;
		}

		m_players[entity->GetID()] = player;
	}

	void ViewInfo::Unregister(Memory::RefPtr<Player> player)
	{
		if (!player)
		{
			return;
		}

		auto entity = player->GetEntity();
		if (!entity)
		{
			return;
		}

		Unregister(entity->GetID());
	}

	void ViewInfo::Unregister(const ECS::UUID& id)
	{
		m_players.Erase(id);
	}

	void ViewInfo::UnregisterAll()
	{
		m_players.Clear();
	}

	const std::string& ViewInfo::GetName() const
	{
		return m_name;
	}

	uint16_t ViewInfo::GetPosX() const
	{
		return m_posX;
	}

	uint16_t ViewInfo::GetPosY() const
	{
		return m_posY;
	}

	uint16_t ViewInfo::GetWidth() const
	{
		return m_width;
	}

	uint16_t ViewInfo::GetHeight() const
	{
		return m_height;
	}

	bool ViewInfo::IsActive() const
	{
		return m_active;
	}

	Memory::RefPtr<ViewInfo> ViewController::Create(const std::string& name)
	{
		Memory::RefPtr<ViewInfo> viewInfo = Memory::MakeRef<ViewInfo>();
		if (!viewInfo)
		{
			LOGERROR() << "[VIEWCONTROLLER] Failed to create the view info(" << name << ")";
			return {};
		}

		viewInfo->SetName(name);
		m_views[name] = viewInfo;

		return viewInfo;
	}

	Memory::RefPtr<ViewInfo> ViewController::Get(const std::string& name)
	{
		auto itr = m_views.Find(name);
		if (itr == m_views.End())
		{
			return {};
		}

		return itr->second;
	}
}