#include <DemoApplication.h>

#include <Framework/Engine.h>
#include <Framework/Window.h>
#include <Framework/Player.h>
#include <Framework/PlayerController.h>
#include <Framework/ViewController.h>

#include <Renderer/RenderTypes.h>
#include <Renderer/Renderer.h>

#include <Asset/AssetSystem.h>
#include <Asset/AssetTypes.h>

#include <World/WorldContext.h>
#include <World/Component.h>
#include <World/Node.h>
#include <World/Entity.h>
#include <World/Scene.h>

#include <Log/include/Log.h>

namespace demo
{
	Game::Game()
	{}

	Game::~Game()
	{}

	void Game::onInit()
	{
		if (!InitEngine())
		{
			LOGERROR() << "[Game] Failed to initialize the engine";
			
			onShutdown();
			return;
		}

		if (!InitWorld())
		{
			LOGERROR() << "[Game] Failed to initialize the world";
			onShutdown();
			return;
		}

		if (!InitScene())
		{
			LOGERROR() << "[Game] Failed to initialize the scene";
			onShutdown();
			return;
		}
	}

	void Game::onRun()
	{
		if (m_engine)
		{
			m_engine->Run();
		}
	}

	void Game::onShutdown()
	{
		if (m_engine)
		{
			m_engine->Shutdown();
			m_engine.Reset();
		}
	}

	bool Game::InitEngine()
	{
		if (!m_engine)
		{
			m_engine = Memory::MakeRef<wtr::Engine>();

			if (!m_engine)
			{
				LOGERROR() << "[Game] Failed to create the engine";
				return false;
			}
		}

		wtr::WindowDesc windowDesc;
		windowDesc.Type = wtr::eWindowType::eWin32;

		wtr::RenderDesc renderDesc;
		renderDesc.Type = wtr::eRenderType::eOpenGL;

		if (!m_engine->Init(windowDesc, renderDesc))
		{
			LOGERROR() << "[Game] Failed to initialize the engine";
			m_engine->Shutdown();
			return false;
		}

		LOGINFO() << "[Game] Succeed to initialize the engine";

		return true;
	}

	bool Game::InitWorld()
	{
		if (!m_engine)
		{
			LOGERROR() << "[Game] Invalid engine instance, failed to set the base options for the world";
			return false;
		}

		auto worldContext = m_engine->GetWorldContext();
		if (!worldContext)
		{
			LOGERROR() << "[Game] Failed to get the world context";
			return false;
		}

		auto world = worldContext->world;
		auto views = worldContext->views;
		auto players = worldContext->players;
		if (!world || !views || !players)
		{
			LOGERROR() << "[Game] Failed to get the world, views or players from the world context";
			return false;
		}

		auto cameraEntity = world->CreateEntity();
		if (!cameraEntity)
		{
			LOGERROR() << "[Game] Failed to create the camera entity";
			return false;
		}

		cameraEntity->AddComponent<wtr::SceneComponent>();
		cameraEntity->AddComponent<wtr::CameraComponent>();
		cameraEntity->AddNode<wtr::CameraNode>();

		auto sceneComponent = cameraEntity->GetComponent<wtr::SceneComponent>();
		if (sceneComponent)
		{
			sceneComponent->UpdatePosition({ 0.0f, 0.0f, 5.0f });
			sceneComponent->UpdateRotation({ 0.0f, 0.0f, 0.0f });
		}

		LOGINFO() << "[Game] Camera Entity ID : " << cameraEntity->GetID().ToString();

		auto cameraPlayer = players->Create(cameraEntity);
		if (!cameraPlayer)
		{
			LOGERROR() << "[Game] Failed to create the camera player";
			return false;
		}

		cameraPlayer->Activate();

		auto mainView = views->Create("MainView");
		if (!mainView)
		{
			LOGERROR() << "[Game] Failed to create the main view";
			return false;
		}

		cameraPlayer->Register(mainView);

		return true;
	}

	bool Game::InitScene()
	{
		if (!m_engine)
		{
			LOGERROR() << "[Game] Invalid engine instance, failed to initialize the scene";
			return false;
		}

		auto worldContext = m_engine->GetWorldContext();
		if (!worldContext)
		{
			LOGERROR() << "[Game] Failed to get the world context from the engine";
			return false;
		}

		auto world = worldContext->world;
		if (!world)
		{
			LOGERROR() << "[Game] Failed to get the world from the engine";
			return false;
		}

		for (size_t index = 0; index < 3; index++)
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
				sceneComponent->UpdatePosition({ static_cast<float>(index) * 0.2f, 0.0f, 0.0f });
				sceneComponent->UpdateScale({ 0.5f, 0.5f, 0.5f });
			}

			world->scene.Attach(dragonEntity->GetNode<wtr::MeshNode>());

			LOGINFO() << "[Game] Dragon Entity ID : " << dragonEntity->GetID().ToString();
		}

		return true;
	}
}