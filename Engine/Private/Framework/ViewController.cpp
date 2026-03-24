#include <World/ViewController.h>

#include <>

namespace wtr
{
	ViewInfo::ViewInfo()
		: m_name("")
		, m_posX(0)
		, m_posY(0)
		, m_width(1080)
		, m_height(800)
		, m_players()
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

	void ViewInfo::Register(Memory::RefPtr<Player> player)
	{
		auto entity = player->GetEntity();
		m_players[player->GetUUID()] = player;
	}
}