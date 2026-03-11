#include <DemoApplication.h>

#include <Framework/Engine.h>
#include <Framework/Window.h>
#include <Renderer/RenderTypes.h>
#include <Renderer/RenderGraph.h>

#include <Log/include/Log.h>

namespace demo
{
	Game::Game()
		: engine(nullptr)
	{}

	Game::~Game()
	{
		if (nullptr != engine)
		{
			delete engine;
		}
	}

	void Game::onInit()
	{
		if (nullptr != engine)
		{
			LOGERROR() << "[GAME] The engine was already initialized";
			return;
		}

		engine = new wtr::Engine();

		wtr::WindowDesc windowDesc;
		windowDesc.Type = wtr::eWindowType::eWin32;

		wtr::RenderDesc renderDesc;
		renderDesc.Type = wtr::eRenderType::eOpenGL;
		
		if (!engine->Init(windowDesc, renderDesc))
		{
			LOGERROR() << "[GAME] Failed to initialize the engine";
			engine->Shutdown();
			return;
		}

		auto& world = engine->GetWorld();
		if (!world)
		{
			LOGERROR() << "[GAME] Failed to get the world from the engine";
			engine->Shutdown();
			return;
		}

		auto& mainScene = world->scene.Create("MainScene");
		auto mainView = wtr::ViewInfo();
		mainView.name = "MainView";
		mainView.pipeline = "BasePipeline";
		mainView.cameraID = ECS::UUID();
		mainView.posX = 0;
		mainView.posY = 0;
		mainView.width = 1280;
		mainView.height = 720;
		mainView.active = true;

		mainScene.RegisterView(mainView);
	}

	void Game::onRun()
	{
		if (nullptr != engine)
		{
			engine->Run();
		}
	}

	void Game::onShutdown()
	{
		if (nullptr != engine)
		{
			engine->Shutdown();
		}
	}
}