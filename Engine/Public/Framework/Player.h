#ifndef __WTR_PLAYER_H__
#define __WTR_PLAYER_H__

#include <Container/include/HashSet.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Memory/include/Pointer/ObjectPtr.h>

namespace wtr
{
	class ViewInfo;
	class Entity;
	class SceneComponent;
	class CameraComponent;
};

namespace wtr
{
	class Player
	{
	public :
		Player();
		virtual ~Player();

	public :
		void SetEntity(Memory::ObjectPtr<Entity> entity);
		Memory::ObjectPtr<const Entity> GetEntity() const;
		Memory::ObjectPtr<const SceneComponent> GetTransform() const;
		Memory::ObjectPtr<const CameraComponent> GetCamera() const;

		void Register(Memory::RefPtr<ViewInfo> viewInfo);
		void Unregister(Memory::RefPtr<ViewInfo> viewInfo);
		void Unregister(const std::string& viewName);
		void UnregisterAll();

		const wtr::HashSet<std::string>& GetViews() const;

		void Activate();
		void Deactivate();

		bool IsActive() const;

	private :
		wtr::HashSet<std::string> m_views;
		Memory::ObjectPtr<const Entity> m_entity;
		bool m_active;
	};
}

#endif // __WTR_PLAYER_H__