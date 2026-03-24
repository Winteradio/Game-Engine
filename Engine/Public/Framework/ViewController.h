#ifndef __WTR_VIEWCONTROLLER_H__
#define __WTR_VIEWCONTROLLER_H__

#include <Memory/include/Pointer/ObjectPtr.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <ECS/include/UUID/UUID.h>
#include <Container/include/HashMap.h>
#include <Container/include/DynamicArray.h>

namespace wtr
{
	class Player;
	class RenderView;
};

namespace wtr
{
	class ViewInfo
	{
	public :
		ViewInfo();
		virtual ~ViewInfo() = default;

	public :
		void SetName(const std::string& name);
		void SetPosition(const uint16_t posX, const uint16_t posY);
		void SetSize(const uint16_t width, const uint16_t height);

		void Register(Memory::RefPtr<Player> player);
		void Unregister(Memory::RefPtr<Player> player);
		void Unregister(const ECS::UUID& uuid);
		void UnregisterAll();

	private :
		std::string m_name;
		uint16_t m_posX;
		uint16_t m_posY;
		uint16_t m_width;
		uint16_t m_height;
		
		wtr::HashMap<ECS::UUID, Memory::RefPtr<Player>> m_players;
	};

	class ViewController
	{
	public :
		ViewController() = default;
		~ViewController() = default;

	public :
		Memory::RefPtr<ViewInfo> Create();
		Memory::RefPtr<ViewInfo> Get(const std::string& name);

		wtr::DynamicArray<const RenderView> GetRenderView() const;

	private :
		wtr::HashMap<std::string, Memory::RefPtr<ViewInfo>> m_views;
	};
};

#endif // __WTR_VIEWCONTROLLER_H__