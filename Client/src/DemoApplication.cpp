#include <DemoApplication.h>

#include <Asset/AssetSystem.h>
#include <Asset/AssetTypes.h>

#include <World/WorldContext.h>
#include <World/Component.h>
#include <World/RenderNode.h>
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

			dragonEntity->AddComponent<wtr::TransformComponent>();
			dragonEntity->AddComponent<wtr::StaticMeshComponent>(dragonAsset);
			dragonEntity->AddNode<wtr::StaticMeshNode>();

			auto transformComponent = dragonEntity->GetComponent<wtr::TransformComponent>();
			if (transformComponent)
			{
				transformComponent->UpdatePosition({ (index % 2 == 0 ? -1.f : 1.f) * static_cast<float>(index % 5), static_cast<float>(index / 5), 0.0f });
				transformComponent->UpdateScale({ 0.5f, 0.5f, 0.5f });
			}

			world->scene.Attach(dragonEntity->GetNode<wtr::StaticMeshNode>());

			//LOGINFO() << "[Game] Dragon Entity ID : " << dragonEntity->GetID().ToString();
		}

		auto cubeEntity = world->CreateEntity();
		if (!cubeEntity)
		{
			LOGERROR() << "[Game] Failed to create the cube entity";
			return false;
		}

		const std::string cubePath = "asset/mesh/3d/cube.obj";
		Memory::RefPtr<wtr::Asset> cubeAsset = wtr::AssetSystem::Load(cubePath);

		cubeEntity->AddComponent<wtr::InstancedTransformComponent>();
		cubeEntity->AddComponent<wtr::StaticMeshComponent>(cubeAsset);
		cubeEntity->AddNode<wtr::InstancedStaticMeshNode>();

		auto transformComponent = cubeEntity->GetComponent<wtr::InstancedTransformComponent>();
		if (transformComponent)
		{
			constexpr size_t cubeCount = 1000000;
			constexpr size_t cubePerRow = 100;
			constexpr size_t cubePerLow = 100;

			for (size_t index = 0; index < cubeCount; index++)
			{
				const wtr::fvec3 position = { (index % 2 == 0 ? -1.f : 1.f) * static_cast<float>(index % cubePerRow), static_cast<float>(index / (cubePerRow * cubePerLow)), static_cast<float>((index / cubePerLow)% cubePerRow)};
				const wtr::fquat rotation = { 1.f, 0.f, 0.f, 0.f };
				const wtr::fvec3 scale = { 0.5f, 0.5f, 0.5f };
				transformComponent->AddInstance(position, rotation, scale);
			}
		}

		world->scene.Attach(cubeEntity->GetNode<wtr::InstancedStaticMeshNode>());

		//LOGINFO() << "[Game] Cube Entity ID : " << cubeEntity->GetID().ToString();

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

		auto instancedMoveSystem = world->CreateSystem<wtr::InstancedMoveSystem>();
		if (!instancedMoveSystem)
		{
			LOGERROR() << "[Game] Failed to create the render system";
			return false;
		}

		return true;
	}
}