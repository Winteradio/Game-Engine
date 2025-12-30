#include <Framework/Engine.h>
#include <Platform/Win32/Win32Window.h>
#include <Platform/Win32/Win32InputHandler.h>

#include <Log/include/Log.h>

namespace wtr
{
	Engine::Engine()
		: m_Window(nullptr)
		, m_InputHandler(nullptr)
		, m_Application(nullptr)
		, m_RHIDevice(nullptr)
		, m_RHIContext(nullptr)
		, m_InputStorage()
	{}

	Engine::~Engine()
	{
		Shutdown();

		if (nullptr != m_Window)
		{
			delete m_Window;
			m_Window = nullptr;
		}

		if (nullptr != m_InputHandler)
		{
			delete m_InputHandler;
			m_InputHandler = nullptr;
		}

		if (nullptr != m_Application)
		{
			delete m_Application;
			m_Application = nullptr;
		}

		if (nullptr != m_RHIDevice)
		{
			delete m_RHIDevice;
			m_RHIDevice = nullptr;
		}

		if (nullptr != m_RHIContext)
		{
			delete m_RHIContext;
			m_RHIContext = nullptr;
		}
	}

	bool Engine::Init(const WindowDesc& mainWindowDesc)
	{
		LOGINFO() << "[Engine] Initializing Engine";

		if (!InitWindow(mainWindowDesc))
		{
			LOGERROR() << "[Engine] Failed to initialize main window";
			return false;
		}

		m_Window->Show();

		LOGINFO() << "[Engine] Engine initialized successfully";
		return true;
	}

	bool Engine::InitWindow(const WindowDesc& mainWindowDesc)
	{
		if (eWindowType::eWin32 == mainWindowDesc.Type)
		{
			m_Window = new Win32Window();
			m_InputHandler = new Win32InputHandler();
		}
		else
		{
			LOGERROR() << "[Engine] Unsupported window type";
			return false;
		}
		
		if (!m_Window->Init(mainWindowDesc))
		{
			LOGERROR() << "[Engine] Failed to initialize main window";
			return false;
		}

		m_Window->SetInputHandler(m_InputHandler);

		LOGINFO() << "[Engine] Main window initialized successfully";

		return true;
	}

	void Engine::Shutdown()
	{
		LOGINFO() << "[Engine] Shutting down Engine";

		if (m_Window)
		{
			m_Window->Shutdown();
		}

		LOGINFO() << "[Engine] Engine shut down successfully";
	}

	void Engine::Run()
	{
		LOGINFO() << "[Engine] Running Engine";

		while (m_Window->GetStatus() != eWindowStatus::eClosed)
		{
			m_Window->PollEvents();

			if (m_Window->GetStatus() == eWindowStatus::eActive)
			{
				Update();
				Render();
			}

			if (m_InputHandler)
			{
				m_InputHandler->PopInputEvent();
			}
		}

		LOGINFO() << "[Engine] Engine run completed";
	}

	void Engine::Update()
	{
		UpdateInput();
	}

	void Engine::UpdateInput()
	{
		if (m_InputHandler)
		{
			m_InputStorage.Prepare();
			
			while (!m_InputHandler->IsEmpty())
			{
				const InputDesc& inputDesc = m_InputHandler->GetInputEvent();
				if (inputDesc.Type != eInputType::eNone)
				{
					m_InputStorage.Update(inputDesc);
				}

				m_InputHandler->PopInputEvent();
			}
		}
	}

	void Engine::Render()
	{
		// Implementation for rendering
	}
};