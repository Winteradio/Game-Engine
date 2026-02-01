#include <Framework/Engine.h>

#include <Framework/Input/InputStorage.h>
#include <Framework/FrameContext.h>

#include <Platform/Win32/Win32Window.h>
#include <Platform/Win32/Win32InputHandler.h>

#include <World/World.h>
#include <World/WorldWorker.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderWorker.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	Engine::Engine()
		: m_window(nullptr)
		, m_inputHandler(nullptr)
		, m_inputStorage(nullptr)
		, m_frameContext(nullptr)
		, m_renderer()
		, m_world()
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
	}

	bool Engine::Init(const WindowDesc& windowDesc, const RenderDesc& renderDesc)
	{
		LOGINFO() << "[Engine] Initializing Engine";

		Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Type);
		Memory::Init(4096, 100);

		m_inputStorage = Memory::MakeRef<InputStorage>();
		m_frameContext = Memory::MakeRef<FrameContext>();

		if (!m_inputStorage || !m_frameContext)
		{
			LOGERROR() << "[Engine] Failed to create the input storage and frame context";
			return false;
		}

		if (!InitWindow(windowDesc))
		{
			LOGERROR() << "[Engine] Failed to initialize the window";
			return false;
		}

		if (!InitRender(renderDesc))
		{
			LOGERROR() << "[Engine] Failed to initialize the renderer";
			return false;
		}

		if (!InitRHI())
		{
			LOGERROR() << "[Engine] Failed to initialize the RHI";
			return false;
		}

		if (!InitAsset())
		{
			LOGERROR() << "[Engine] Failed to initialize the Asset Manager";
			return false;
		}

		if (!InitWorld())
		{
			LOGERROR() << "[Engine] Failed to initialize the world";
			return false;
		}

		m_window->Show();

		LOGINFO() << "[Engine] Engine initialized successfully";
		return true;
	}

	bool Engine::InitWindow(const WindowDesc& windowDesc)
	{
		if (eWindowType::eWin32 == windowDesc.Type)
		{
			m_window = new Win32Window();
			m_inputHandler = new Win32InputHandler();
		}
		else
		{
			LOGERROR() << "[Engine] Unsupported window type";
			return false;
		}
		
		if (!m_window->Init(windowDesc))
		{
			LOGERROR() << "[Engine] Failed to initialize main window";
			return false;
		}

		m_window->SetInputHandler(m_inputHandler);

		LOGINFO() << "[Engine] Main window initialized successfully";

		return true;
	}

	bool Engine::InitRender(const RenderDesc& renderDesc)
	{
		if (!m_window)
		{
			LOGERROR() << "[Engne] The Window's native handle is invalid, failed to initialize the render";
			return false;
		}

		void* nativeHandle = m_window->GetNativeHandle();

		Memory::RefPtr<RenderWorker> worker = Memory::MakeRef<RenderWorker>();
		if (!worker)
		{
			LOGERROR() << "[Engine] Failed to create the render worker";
			return false;
		}

		worker->SetFrameContext(m_frameContext);

		m_renderer = Memory::MakePtr<Renderer>();
		if (!m_renderer)
		{
			LOGERROR() << "[Engine] Failed to create the renderer";
			return false;
		}

		if (!m_renderer->Init(renderDesc, nativeHandle))
		{
			LOGERROR() << "[Engine] Failed to initialize renderer";
			return false;
		}

		m_renderer->SetWorker(worker);

		LOGINFO() << "[Engine] Renderer initialized successfully";

		return true;
	}

	bool Engine::InitWorld()
	{
		Memory::RefPtr<WorldWorker> worker = Memory::MakeRef<WorldWorker>();
		if (!worker)
		{
			LOGERROR() << "[Engine] Failed to create the world worker";
			return false;
		}

		m_world = Memory::MakePtr<World>();
		if (!m_world)
		{
			LOGERROR() << "[Engine] Failed to create the world";
			return false;
		}

		worker->SetInputStorage(m_inputStorage);
		worker->SetFrameContext(m_frameContext);

		m_world->SetWorker(worker);

		LOGINFO() << "[Engine] Succeed to initialize the world";

		return true;
	}

	bool Engine::InitRHI()
	{

	}

	bool Engine::InitAsset()
	{

	}

	void Engine::Shutdown()
	{
		LOGINFO() << "[Engine] Shutting down Engine";

		if (m_window)
		{
			m_window->Shutdown();
		}

		if (m_world)
		{
			m_world->Stop();
			m_world.Reset();
		}

		if (m_renderer)
		{
			m_renderer->Stop();
			m_renderer.Reset();
		}

		Memory::Release();

		LOGINFO() << "[Engine] Engine shut down successfully";
	}

	void Engine::Run()
	{
		LOGINFO() << "[Engine] Running Engine";

		if (m_world)
		{
			m_world->Run();
		}

		if (m_renderer)
		{
			m_renderer->Run();
		}

		while (m_window->GetStatus() != eWindowStatus::eClosed)
		{
			m_window->PollEvents();

			UpdateInput();
		}

		LOGINFO() << "[Engine] Engine run completed";
	}

	Memory::ObjectPtr<World> Engine::GetWorld()
	{
		return m_world;
	}

	Memory::ObjectPtr<Renderer> Engine::GetRenderer()
	{
		return m_renderer;
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