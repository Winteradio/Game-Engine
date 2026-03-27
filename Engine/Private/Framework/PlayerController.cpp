#include <Framework/PlayerController.h>

#include <Framework/Player.h>
#include <World/Entity.h>
#include <Memory/include/Core.h>

namespace wtr
{
	Memory::RefPtr<Player> PlayerController::Create(Memory::ObjectPtr<Entity> entity)
	{
		if (!entity)
		{
			return {};
		}

		Memory::RefPtr<Player> player = Memory::MakeRef<Player>();
		if (!player)
		{
			return {};
		}

		player->SetEntity(entity);

		m_players[entity->GetID()] = player;

		return player;
	}

	Memory::RefPtr<Player> PlayerController::Get(const ECS::UUID& id)
	{
		auto itr = m_players.Find(id);
		if (itr == m_players.End())
		{
			return {};
		}

		return itr->second;
	}

	wtr::DynamicArray<Memory::RefPtr<Player>>& PlayerController::GetActives()
	{
		m_actives.Clear();

		for (auto& [id, player] : m_players)
		{
			if (player && player->IsActive())
			{
				m_actives.PushBack(player);
			}
		}

		return m_actives;
	}
}