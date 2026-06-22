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

	bool Application::InitView(const ViewInfo viewInfo, const bool fixedCamera)
	{
		auto world = GetWorld();
		auto views = GetViewController();
		auto players = GetPlayerController();
		if (!world || !views || !players)
		{
			LOGERROR() << "[Application] Failed to get the world, views or players from the world context";
			return false;
		}

		auto cameraEntity = world->CreateEntity();
		if (!cameraEntity)
		{
			LOGERROR() << "[Application] Failed to create the camera entity";
			return false;
		}

		cameraEntity->AddComponent<wtr::TransformComponent>();
		cameraEntity->AddComponent<wtr::CameraComponent>();

		if (fixedCamera)
		{
			cameraEntity->AddNode<wtr::FixedCameraNode>();
		}
		else
		{
			cameraEntity->AddNode<wtr::CameraNode>();
		}

		auto transformComponent = cameraEntity->GetComponent<wtr::TransformComponent>();
		if (transformComponent)
		{
			transformComponent->UpdatePosition({ 0.0f, 2.0f, -5.0f });
			transformComponent->UpdateRotation(glm::angleAxis(glm::radians(-180.f), fvec3(0.f, 1.f, 0.f)));
		}

		auto cameraComponent = cameraEntity->GetComponent<wtr::CameraComponent>();
		if (cameraComponent)
		{
			cameraComponent->nearPlane = 0.1f;
			cameraComponent->farPlane = 100.0f;
			cameraComponent->width = static_cast<float>(viewInfo.GetWidth());
			cameraComponent->height = static_cast<float>(viewInfo.GetHeight());
			cameraComponent->perspective = true;
		}

		auto cameraPlayer = players->Create(cameraEntity);
		if (!cameraPlayer)
		{
			LOGERROR() << "[Application] Failed to create the camera player";
			return false;
		}

		auto view = views->Create(viewInfo.GetName());
		if (!view)
		{
			LOGERROR() << "[Application] Failed to create the main view";
			return false;
		}

		view->SetPosition(viewInfo.GetPosX(), viewInfo.GetPosY());
		view->SetSize(viewInfo.GetWidth(), viewInfo.GetHeight());

		cameraPlayer->Activate();
		cameraPlayer->Register(view);

		return true;
	}

	WindowDesc Application::GetWindowDesc() const
	{
		WindowDesc desc;
		desc.Type = eWindowType::eWin32;
		desc.Name = "Winteradio";

		desc.PosX = 0.0f;
		desc.PosY = 0.0f;
		desc.Width = 1080.0f;
		desc.Height = 800.0f;
		return desc;
	}

	RenderDesc Application::GetRenderDesc() const
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

	Memory::RefPtr<InputStorage> Application::GetInputStorage() const
	{
		if (!m_engine)
		{
			return {};
		}

		return m_engine->GetInputStorage();
	}
}