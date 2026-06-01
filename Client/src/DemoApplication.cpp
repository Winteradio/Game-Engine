#include <DemoApplication.h>

#include <Asset/AssetSystem.h>
#include <Asset/AssetTypes.h>

#include <World/WorldContext.h>
#include <World/Component.h>
#include <World/RenderNode.h>
#include <World/Entity.h>
#include <World/System/MoveSystem.h>
#include <Log/include/Log.h>

#include <random>

namespace demo
{
	static std::random_device RANDOM_DEVICE;

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
		if (!InitBase())
		{
			LOGERROR() << "[Game] Failed to initialize the base entity";
			return false;
		}

		if (!InitCube())
		{
			LOGERROR() << "[Game] Failed to initialize the cube entities";
			return false;
		}

		if (!InitPointLight())
		{
			LOGERROR() << "[Game] Failed to initialize the point light entities";
			return false;
		}

		if (!InitSpotLight())
		{
			LOGERROR() << "[Game] Failed to initialize the spot light entities";
			return false;
		}

		return true;
	}

	bool Game::InitBase()
	{
		auto world = GetWorld();
		if (!world)
		{
			LOGERROR() << "[Game] Failed to get the world from the engine";
			return false;
		}

		auto baseEntity = world->CreateEntity();
		if (!baseEntity)
		{
			LOGERROR() << "[Game] Failed to create the cube entity";
			return false;
		}

		const std::string cubePath = "asset/mesh/3d/cube.obj";
		Memory::RefPtr<wtr::Asset> cubeAsset = wtr::AssetSystem::Load(cubePath);

		baseEntity->AddComponent<wtr::TransformComponent>();
		baseEntity->AddComponent<wtr::StaticMeshComponent>(cubeAsset);
		baseEntity->AddNode<wtr::StaticMeshNode>();

		auto transformComponent = baseEntity->GetComponent<wtr::TransformComponent>();
		if (transformComponent)
		{
			transformComponent->UpdateScale(wtr::fvec3{ 20.f, 0.5f, 20.0f });
			transformComponent->UpdatePosition(wtr::fvec3{ 0.f, -1.0f, 0.f });
		}

		world->scene.Attach(baseEntity->GetNode<wtr::StaticMeshNode>());

		return true;
	}

	bool Game::InitCube()
	{
		auto world = GetWorld();
		if (!world)
		{
			LOGERROR() << "[Game] Failed to get the world from the engine";
			return false;
		}

		std::mt19937 generator(RANDOM_DEVICE());
		std::uniform_real_distribution<double> distribution(0.0, 1.0);

		for (size_t index = 0; index < 25; index++)
		{
			auto cubeEntity = world->CreateEntity();
			if (!cubeEntity)
			{
				LOGERROR() << "[Game] Failed to create the sphere entity";
				continue;
			}

			const size_t localIndex = index % 25;
			const size_t row = localIndex / 5;
			const size_t col = localIndex % 5;

			const float shiniess = static_cast<float>(col + 2L * row) + 2.0f;

			const std::string matName = "cube_" + std::to_string(localIndex);

			Memory::RefPtr<wtr::MaterialAsset> mat = Memory::Cast<wtr::MaterialAsset>(wtr::AssetSystem::Create(matName, wtr::eAsset::eMaterial));
			if (!mat) continue;

			const wtr::fvec3 color = wtr::fvec3(static_cast<float>(distribution(generator)), static_cast<float>(distribution(generator)), static_cast<float>(distribution(generator)));
			mat->vectorValues[wtr::eVectorSlot::eBaseColor] = color;
			mat->scalarValues[wtr::eScalarSlot::eShininess] = shiniess;

			const std::string dragonPath = "asset/mesh/3d/cube.obj";
			Memory::RefPtr<wtr::Asset> dragonAsset = wtr::AssetSystem::Load(dragonPath);
			Memory::RefPtr<wtr::Asset> materialAsset = wtr::AssetSystem::Load(matName);

			cubeEntity->AddComponent<wtr::TransformComponent>();
			cubeEntity->AddComponent<wtr::StaticMeshComponent>(dragonAsset);
			cubeEntity->AddComponent<wtr::MaterialComponent>(materialAsset);
			cubeEntity->AddNode<wtr::StaticMeshNode>();

			auto transformComponent = cubeEntity->GetComponent<wtr::TransformComponent>();
			if (transformComponent)
			{
				const wtr::fvec3 position = { static_cast<float>(col), 0.f, static_cast<float>(row) };
				const wtr::fvec3 scale = wtr::fvec3(0.3f);
				transformComponent->UpdatePosition(position);
				transformComponent->UpdateScale(scale);
			}

			world->scene.Attach(cubeEntity->GetNode<wtr::StaticMeshNode>());
		}

		return true;
	}

	bool Game::InitPointLight()
	{
		auto world = GetWorld();
		if (!world)
		{
			LOGERROR() << "[Game] Failed to get the world from the engine";
			return false;
		}

		for (size_t index = 0; index < 5; index++)
		{
			auto lightEntity = world->CreateEntity();
			if (!lightEntity)
			{
				LOGERROR() << "[Game] Failed to create the sphere entity";
				continue;
			}

			const float shiniess = static_cast<float>(index) * 4.f + 2.0f;

			const std::string matName = "red_" + std::to_string(index);

			Memory::RefPtr<wtr::MaterialAsset> mat = Memory::Cast<wtr::MaterialAsset>(
				wtr::AssetSystem::Create(matName, wtr::eAsset::eMaterial));
			if (!mat) continue;

			mat->vectorValues[wtr::eVectorSlot::eBaseColor] = wtr::fvec3(1.f, 0.1f, 0.1f);
			mat->scalarValues[wtr::eScalarSlot::eShininess] = shiniess;

			const std::string cubePath = "asset/mesh/3d/cube.obj";
			Memory::RefPtr<wtr::Asset> cubeAsset = wtr::AssetSystem::Load(cubePath);
			Memory::RefPtr<wtr::Asset> materialAsset = wtr::AssetSystem::Load(matName);

			lightEntity->AddComponent<wtr::TransformComponent>();
			lightEntity->AddComponent<wtr::StaticMeshComponent>(cubeAsset);
			lightEntity->AddComponent<wtr::MaterialComponent>(materialAsset);
			lightEntity->AddNode<wtr::StaticMeshNode>();

			auto transformComponent = lightEntity->GetComponent<wtr::TransformComponent>();
			if (transformComponent)
			{
				const wtr::fvec3 position = { static_cast<float>(index), 0.f, static_cast<float>(index) };
				const wtr::fvec3 scale = wtr::fvec3(0.3f);
				transformComponent->UpdatePosition(position);
				transformComponent->UpdateScale(scale);
			}

			lightEntity->AddComponent<wtr::PointLightComponent>();
			lightEntity->AddNode<wtr::PointLightNode>();

			auto lightComponent = lightEntity->GetComponent<wtr::PointLightComponent>();
			if (lightComponent)
			{
				lightComponent->UpdateRange(index * 2.f);
			}

			world->scene.Attach(lightEntity->GetNode<wtr::StaticMeshNode>());
			world->scene.Attach(lightEntity->GetNode<wtr::PointLightNode>());
		}

		return true;
	}

	bool Game::InitSpotLight()
	{
		auto world = GetWorld();
		if (!world)
		{
			LOGERROR() << "[Game] Failed to get the world from the engine";
			return false;
		}

		for (size_t index = 0; index < 2; index++)
		{
			auto lightEntity = world->CreateEntity();
			if (!lightEntity)
			{
				LOGERROR() << "[Game] Failed to create the sphere entity";
				continue;
			}

			const std::string cubePath = "asset/mesh/3d/cube.obj";
			const std::string matName = "default_spot";

			Memory::RefPtr<wtr::MaterialAsset> mat = Memory::Cast<wtr::MaterialAsset>(wtr::AssetSystem::Create(matName, wtr::eAsset::eMaterial));
			if (!mat) continue;

			Memory::RefPtr<wtr::Asset> cubeAsset = wtr::AssetSystem::Load(cubePath);
			Memory::RefPtr<wtr::Asset> materialAsset = wtr::AssetSystem::Load(matName);

			lightEntity->AddComponent<wtr::TransformComponent>();
			lightEntity->AddComponent<wtr::StaticMeshComponent>(cubeAsset);
			lightEntity->AddComponent<wtr::MaterialComponent>(materialAsset);
			lightEntity->AddNode<wtr::StaticMeshNode>();

			auto transformComponent = lightEntity->GetComponent<wtr::TransformComponent>();
			if (transformComponent)
			{
				const wtr::fvec3 position = { index % 2 == 0 ? 1.0f + static_cast<float>(index) : 5.f - 1.f * static_cast<float>(index), 1.f, 0.f};
				const wtr::fvec3 scale = wtr::fvec3(0.3f);
				transformComponent->UpdatePosition(position);
				transformComponent->UpdateScale(scale);
			}

			lightEntity->AddComponent<wtr::SpotLightComponent>();
			lightEntity->AddNode<wtr::SpotLightNode>();
			auto lightComponent = lightEntity->GetComponent<wtr::SpotLightComponent>();
			if (lightComponent)
			{
				lightComponent->UpdateRange(20.f);
				lightComponent->UpdateInnerAngle(15.f);
				lightComponent->UpdateOuterAngle(30.f);
			}

			world->scene.Attach(lightEntity->GetNode<wtr::StaticMeshNode>());
			world->scene.Attach(lightEntity->GetNode<wtr::SpotLightNode>());
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

		{
			auto moveSystem = world->CreateSystem<wtr::LightMoveSystem>();
			if (!moveSystem)
			{
				LOGERROR() << "[Game] Failed to create the render system";
				return false;
			}
		}

		//auto instancedMoveSystem = world->CreateSystem<wtr::InstancedMoveSystem>();
		//if (!instancedMoveSystem)
		//{
		//	LOGERROR() << "[Game] Failed to create the render system";
		//	return false;
		//}

		return true;
	}
}