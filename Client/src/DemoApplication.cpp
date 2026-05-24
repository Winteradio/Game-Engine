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

		{
			auto dragonEntity = world->CreateEntity();
			if (!dragonEntity)
			{
				LOGERROR() << "[Game] Failed to create the dragon entity";
				return false;
			}

			const std::string dragonPath = "asset/mesh/3d/dragon.obj";
			Memory::RefPtr<wtr::Asset> dragonAsset = wtr::AssetSystem::Load(dragonPath);

			dragonEntity->AddComponent<wtr::InstancedTransformComponent>();
			dragonEntity->AddComponent<wtr::StaticMeshComponent>(dragonAsset);
			dragonEntity->AddNode<wtr::InstancedStaticMeshNode>();

			auto transformComponent = dragonEntity->GetComponent<wtr::InstancedTransformComponent>();
			if (transformComponent)
			{
				for (size_t index = 0; index < 25; index++)
				{
					const wtr::fvec3 position = { (index % 2 == 0 ? -1.f : 1.f) * static_cast<float>(index % 5), 0.f, ((index / 5) % 2 == 0 ? -1.f : 1.f) * static_cast<float>(index / 5) };
					transformComponent->AddInstance(position);
				}
			}

			world->scene.Attach(dragonEntity->GetNode<wtr::InstancedStaticMeshNode>());
		}

		{
			auto cubeEntity = world->CreateEntity();
			if (!cubeEntity)
			{
				LOGERROR() << "[Game] Failed to create the cube entity";
				return false;
			}

			const std::string cubePath = "asset/mesh/3d/cube.obj";
			Memory::RefPtr<wtr::Asset> cubeAsset = wtr::AssetSystem::Load(cubePath);

			cubeEntity->AddComponent<wtr::TransformComponent>();
			cubeEntity->AddComponent<wtr::StaticMeshComponent>(cubeAsset);
			cubeEntity->AddNode<wtr::StaticMeshNode>();

			auto transformComponent = cubeEntity->GetComponent<wtr::TransformComponent>();
			if (transformComponent)
			{
				transformComponent->UpdateScale(wtr::fvec3{ 20.f, 0.5f, 20.0f });
				transformComponent->UpdatePosition(wtr::fvec3{ 0.f, -1.0f, 0.f });
			}

			world->scene.Attach(cubeEntity->GetNode<wtr::StaticMeshNode>());
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

		//auto moveSystem = world->CreateSystem<wtr::MoveSystem>();
		//if (!moveSystem)
		//{
		//	LOGERROR() << "[Game] Failed to create the render system";
		//	return false;
		//}

		auto instancedMoveSystem = world->CreateSystem<wtr::InstancedMoveSystem>();
		if (!instancedMoveSystem)
		{
			LOGERROR() << "[Game] Failed to create the render system";
			return false;
		}

		return true;
	}
}