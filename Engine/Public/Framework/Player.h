#ifndef __WTR_PLAYER_H__
#define __WTR_PLAYER_H__

#include <Memory/include/Pointer/ObjectPtr.h>

namespace wtr
{
	class Entity;
	class TransformComponent;
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
		Memory::ObjectPtr<const TransformComponent> GetTransform() const;
		Memory::ObjectPtr<const CameraComponent> GetCamera() const;

		void Activate();
		void Deactivate();

		bool IsActive() const;

	private :
		Memory::ObjectPtr<const Entity> m_entity;
		bool m_active;
	};
}

#endif // __WTR_PLAYER_H__