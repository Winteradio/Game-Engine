#pragma

#include <World/Entity.h>

namespace wtr
{
	class Player
	{
	public :
		Player();
		virtual ~Player();

	public :
		void SetEntity(Memory::ObjectPtr<Entity> entity);
		Memory::ObjectPtr<Entity> GetEntity() const;

	private :
		Memory::ObjectPtr<Entity> m_entity;
	};
}