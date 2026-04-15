#include <Framework/Application.h>

#include <Framework/Window.h>
#include <Framework/Player.h>
#include <Framework/PlayerController.h>
#include <Framework/ViewController.h>
#include <Renderer/Renderer.h>

#include <World/Entity.h>
#include <World/Commander.h>
#include <World/Node.h>
#include <World/WorldContext.h>
#include <World/System/CameraSystem.h>

namespace wtr
{
	void Application::onInit()
	{
		const WindowDesc windowDesc = GetWindowDesc();
		const RenderDesc renderDesc = GetRenderDesc();

		if (!InitEngine(windowDesc, renderDesc))
		{
			LOGERROR() << "[Application] Failed to initialize the engine";
			
			onShutdown();
			return;
		}

		if (!InitDefault())
		{
			LOGERROR() << "[Application] Failed to initialize the default settings";
			onShutdown();
			return;
		}

		onSetup();
	}

	void Application::onRun()
	{
		if (m_engine)
		{
			m_engine->Run();
		}
	}

	void Application::onShutdown()
	{
		if (m_engine)
		{
			m_engine->Shutdown();
			m_engine.Reset();
		}
	}

	bool Application::InitEngine(const wtr::WindowDesc& windowDesc, const wtr::RenderDesc& renderDesc)
	{
		if (!m_engine)
		{
			m_engine = Memory::MakeRef<Engine>();
			if (!m_engine)
			{
				LOGERROR() << "[Application] Failed to create the engine";
				return false;
			}
		}

		if (!m_engine->Init(windowDesc, renderDesc))
		{
			LOGERROR() << "[Application] Failed to initialize the engine";
			m_engine->Shutdown();
			return false;
		}

		LOGINFO() << "[Application] Succeed to initialize the engine";
		
		return true;
	}

	bool Application::InitDefault()
	{
		auto world = GetWorld();
		auto views = GetViewController();
		auto players = GetPlayerController();
		if (!world || !views || !players)
		{
			LOGERROR() << "[Game] Failed to get the world, views or players from the world context";
			return false;
		}

		auto cameraSystem = world->CreateSystem<CameraSystem>();
		if (!cameraSystem)
		{
			LOGERROR() << "[Application] Failed to create the camera system";
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
		}

		LOGINFO() << "[Game] Camera Entity ID : " << cameraEntity->GetID().ToString();

		auto cameraPlayer = players->Create(cameraEntity);
		if (!cameraPlayer)
		{
			LOGERROR() << "[Game] Failed to create the camera player";
			return false;
		}

		auto mainView = views->Create("MainView");
		if (!mainView)
		{
			LOGERROR() << "[Game] Failed to create the main view";
			return false;
		}

		cameraPlayer->Activate();
		cameraPlayer->Register(mainView);

		return true;
	}

	const WindowDesc Application::GetWindowDesc() const
	{
		WindowDesc desc;
		desc.Type = eWindowType::eWin32;
		desc.Name = "Winteradio";

		desc.PosX = 0.0f;
		desc.PosY = 0.0f;
		desc.Width = 1280.0f;
		desc.Height = 720.0f;
		return desc;
	}

	const RenderDesc Application::GetRenderDesc() const
	{
		RenderDesc desc;
		desc.Type = eRenderType::eOpenGL;
		return desc;
	}

	Memory::ObjectPtr<World> Application::GetWorld() const
	{
		if (!m_engine)
		{
			return {};
		}

		auto worldContext = m_engine->GetWorldContext();
		if (!worldContext)
		{
			return {};
		}

		return worldContext->world;
	}

	Memory::RefPtr<ViewController> Application::GetViewController() const
	{
		if (!m_engine)
		{
			return {};
		}

		auto worldContext = m_engine->GetWorldContext();
		if (!worldContext)
		{
			return {};
		}

		return worldContext->views;
	}

	Memory::RefPtr<PlayerController> Application::GetPlayerController() const
	{
		if (!m_engine)
		{
			return {};
		}

		auto worldContext = m_engine->GetWorldContext();
		if (!worldContext)
		{
			return {};
		}

		return worldContext->players;
	}

	Memory::RefPtr<Renderer> Application::GetRenderer() const
	{
		if (!m_engine)
		{
			return {};
		}

		return m_engine->GetRenderer();
	}
}