#ifndef __WTR_PLAYERCONTROLLER_H__
#define __WTR_PLAYERCONTROLLER_H__

#include <Container/include/DynamicArray.h>
#include <Container/include/HashMap.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Memory/include/Pointer/ObjectPtr.h>
#include <ECS/include/UUID/UUID.h>

namespace wtr
{
	class Entity;
	class Player;
};

namespace wtr
{
	class PlayerController
	{
	public :
		PlayerController() = default;
		~PlayerController() = default;

	public :
		Memory::RefPtr<Player> Create(Memory::ObjectPtr<Entity> entity);
		Memory::RefPtr<Player> Get(const ECS::UUID& id);

		wtr::DynamicArray<Memory::RefPtr<Player>>& GetActives();

	private :
		wtr::HashMap<ECS::UUID, Memory::RefPtr<Player>> m_players;
		wtr::DynamicArray<Memory::RefPtr<Player>> m_actives;
	};
};

#endif // __WTR_PLAYERCONTROLLER_H__