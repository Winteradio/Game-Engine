#include <Framework/ViewController.h>

#include <Framework/Player.h>
#include <Renderer/RenderView.h>
#include <World/Entity.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>
namespace wtr
{
	ViewInfo::ViewInfo()
		: m_name("")
		, m_posX(0)
		, m_posY(0)
		, m_width(1080)
		, m_height(800)
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

	Memory::RefPtr<ViewInfo> ViewController::Create(const std::string& name)
	{
		Memory::RefPtr<ViewInfo> viewInfo = Memory::MakeRef<ViewInfo>();
		if (!viewInfo)
		{
			LOGERROR() << "[ViewController] Failed to create the view info(" << name << ")";
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