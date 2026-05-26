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
			auto sphereEntity = world->CreateEntity();
			if (!sphereEntity)
			{
				LOGERROR() << "[Game] Failed to create the sphere entity";
				continue;
			}

			const size_t localIndex = index % 25;
			const size_t row = localIndex / 5;
			const size_t col = localIndex % 5;

			const float shiniess = static_cast<float>(col) / 4.0f;

			const std::string matName = "red_" + std::to_string(localIndex);

			Memory::RefPtr<wtr::MaterialAsset> mat = Memory::Cast<wtr::MaterialAsset>(
				wtr::AssetSystem::Create(matName, wtr::eAsset::eMaterial));
			if (!mat) continue;

			mat->vectorValues[wtr::eVectorSlot::eBaseColor] = wtr::fvec3(1.f, 0.1f, 0.1f);
			mat->scalarValues[wtr::eScalarSlot::eShininess] = shiniess;

			const std::string dragonPath = "asset/mesh/3d/sphere.obj";
			Memory::RefPtr<wtr::Asset> dragonAsset = wtr::AssetSystem::Load(dragonPath);
			Memory::RefPtr<wtr::Asset> materialAsset = wtr::AssetSystem::Load(matName);

			sphereEntity->AddComponent<wtr::TransformComponent>();
			sphereEntity->AddComponent<wtr::StaticMeshComponent>(dragonAsset);
			sphereEntity->AddComponent<wtr::MaterialComponent>(materialAsset);
			sphereEntity->AddNode<wtr::StaticMeshNode>();

			auto transformComponent = sphereEntity->GetComponent<wtr::TransformComponent>();
			if (transformComponent)
			{
				const wtr::fvec3 position = {static_cast<float>(col), 0.f, static_cast<float>(row) };
				const wtr::fvec3 scale = wtr::fvec3(0.3f);
				transformComponent->UpdatePosition(position);
				transformComponent->UpdateScale(scale);
			}

			if (index % 6 == 0)
			{
				sphereEntity->AddComponent<wtr::PointLightComponent>();
				sphereEntity->AddNode<wtr::PointLightNode>();
				world->scene.Attach(sphereEntity->GetNode<wtr::PointLightNode>());
			}

			world->scene.Attach(sphereEntity->GetNode<wtr::StaticMeshNode>());
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