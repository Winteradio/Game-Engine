#include <DemoApplication.h>

#include <Asset/AssetSystem.h>
#include <Asset/AssetTypes.h>

#include <World/WorldContext.h>
#include <World/Component.h>
#include <World/Node.h>
#include <World/Entity.h>
#include <World/System/MoveSystem.h>

#include <Log/include/Log.h>

namespace demo
{
	Game::Game()
	{}

	Game::~Game()
	{}

	void Game::onSetup()
	{
		if (!InitEntity())
		{
			LOGERROR() << "[Game] Failed to initialize the entity";

			onShutdown();
			return;
		}

		if (!InitSystem())
		{
			LOGERROR() << "[Game] Failed to initialize the system";
			onShutdown();
			return;
		}
	}

	bool Game::InitEntity()
	{
		auto world = GetWorld();
		if (!world)
		{
			LOGERROR() << "[Game] Failed to get the world from the engine";
			return false;
		}

		for (size_t index = 0; index < 25; index++)
		{
			auto dragonEntity = world->CreateEntity();
			if (!dragonEntity)
			{
				LOGERROR() << "[Game] Failed to create the dragon entity";
				continue;
			}

			const std::string dragonPath = "asset/mesh/3d/dragon.obj";
			Memory::RefPtr<wtr::Asset> dragonAsset = wtr::AssetSystem::Load(dragonPath);

			dragonEntity->AddComponent<wtr::SceneComponent>();
			dragonEntity->AddComponent<wtr::MeshComponent>(dragonAsset);
			dragonEntity->AddNode<wtr::MeshNode>();

			auto sceneComponent = dragonEntity->GetComponent<wtr::SceneComponent>();
			if (sceneComponent)
			{
				sceneComponent->UpdatePosition({ static_cast<float>(index % 5) * 0.4f, static_cast<float>(index / 5) * 0.4f, 0.0f });
				sceneComponent->UpdateScale({ 0.5f, 0.5f, 0.5f });
			}

			world->scene.Attach(dragonEntity->GetNode<wtr::MeshNode>());

			LOGINFO() << "[Game] Dragon Entity ID : " << dragonEntity->GetID().ToString();
		}

		return true;
	}

	bool Game::InitSystem()
	{
		auto world = GetWorld();
		if (!world)
		{
			LOGERROR() << "[Game] Failed to get the world from the engine";
			return false;
		}

		auto moveSystem = world->CreateSystem<wtr::MoveSystem>();
		if (!moveSystem)
		{
			LOGERROR() << "[Game] Failed to create the render system";
			return false;
		}
		return true;
	}
}