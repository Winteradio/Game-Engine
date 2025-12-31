#include <Framework/Engine.h>

#include <Framework/Input/InputStorage.h>
#include <Platform/Win32/Win32Window.h>
#include <Platform/Win32/Win32InputHandler.h>
#include <Renderer/RenderContext.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	Engine::Engine()
		: m_window(nullptr)
		, m_inputHandler(nullptr)
		, m_application(nullptr)
		, m_inputStorage(nullptr)
		, m_renderContext(nullptr)
		, m_worldWorker()
		, m_renderWorker()
	{}

	Engine::~Engine()
	{
		Shutdown();

		if (nullptr != m_window)
		{
			delete m_window;
			m_window = nullptr;
		}

		if (nullptr != m_inputHandler)
		{
			delete m_inputHandler;
			m_inputHandler = nullptr;
		}

		if (nullptr != m_application)
		{
			delete m_application;
			m_application = nullptr;
		}
	}

	bool Engine::Init(const WindowDesc& mainWindowDesc)
	{
		LOGINFO() << "[Engine] Initializing Engine";

		m_inputStorage = Memory::MakeRef<InputStorage>();
		m_renderContext = Memory::MakeRef<RenderContext>();
		if (!m_inputStorage || !m_renderContext)
		{
			LOGERROR() << "[Engine] Failed to create the input storage and render context";
			return false;
		}

		if (!InitWindow(mainWindowDesc))
		{
			LOGERROR() << "[Engine] Failed to initialize main window";
			return false;
		}

		if (!m_worldWorker.Init(m_inputStorage, m_renderContext))
		{
			LOGERROR() << "[Engine] Failed to initialize the world worker";
			return false;
		}

		if (!m_renderWorker.Init(m_renderContext))
		{
			LOGERROR() << "[Engine] Failed to initialize the render worker";
			return false;
		}

		m_window->Show();

		LOGINFO() << "[Engine] Engine initialized successfully";
		return true;
	}

	bool Engine::InitWindow(const WindowDesc& mainWindowDesc)
	{
		if (eWindowType::eWin32 == mainWindowDesc.Type)
		{
			m_window = new Win32Window();
			m_inputHandler = new Win32InputHandler();
		}
		else
		{
			LOGERROR() << "[Engine] Unsupported window type";
			return false;
		}
		
		if (!m_window->Init(mainWindowDesc))
		{
			LOGERROR() << "[Engine] Failed to initialize main window";
			return false;
		}

		m_window->SetInputHandler(m_inputHandler);

		LOGINFO() << "[Engine] Main window initialized successfully";

		return true;
	}

	void Engine::Shutdown()
	{
		LOGINFO() << "[Engine] Shutting down Engine";

		if (m_window)
		{
			m_window->Shutdown();
		}

		m_worldWorker.Stop();
		m_renderWorker.Stop();

		LOGINFO() << "[Engine] Engine shut down successfully";
	}

	void Engine::Run()
	{
		LOGINFO() << "[Engine] Running Engine";

		m_worldWorker.Start();
		m_renderWorker.Start();

		while (m_window->GetStatus() != eWindowStatus::eClosed)
		{
			m_window->PollEvents();

			UpdateInput();
		}

		LOGINFO() << "[Engine] Engine run completed";
	}

	void Engine::UpdateInput()
	{
		if (!m_inputHandler || !m_inputStorage)
		{
			return;
		}

		m_inputStorage->Prepare();

		while (!m_inputHandler->IsEmpty())
		{
			const InputDesc& inputDesc = m_inputHandler->GetInputEvent();
			if (inputDesc.Type != eInputType::eNone)
			{
				m_inputStorage->Update(inputDesc);
			}

			m_inputHandler->PopInputEvent();
		}
	}
};