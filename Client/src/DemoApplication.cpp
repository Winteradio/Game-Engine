#include <DemoApplication.h>

#include <Framework/Engine.h>
#include <Framework/Window.h>
#include <Renderer/RenderTypes.h>
#include <Renderer/Renderer.h>

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