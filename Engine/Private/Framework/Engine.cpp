#include <Framework/Engine.h>

#include <Framework/Input/InputStorage.h>
#include <Framework/FrameContext.h>

#include <Platform/Win32/Win32Window.h>
#include <Platform/Win32/Win32InputHandler.h>

#include <Asset/AssetManager.h>
#include <Asset/AssetWorker.h>
#include <World/World.h>
#include <World/WorldWorker.h>
#include <Renderer/RenderTypes.h>
#include <Renderer/RenderGraph.h>
#include <Renderer/RenderWorker.h>
#include <RHI/RHISystem.h>
#include <RHI/RHIFrameExecutor.h>
#include <RHI/RHITaskExecutor.h>
#include <RHI/OpenGL/GLSystem.h>
#include <RHI/RHIWorker.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	Engine::Engine()
		: m_window(nullptr)
		, m_inputHandler(nullptr)
		, m_inputStorage(nullptr)
		, m_frameContext(nullptr)
		, m_world()
		, m_worldWorker(nullptr)
		, m_renderGraph(nullptr)
		, m_renderWorker(nullptr)
		, m_rhiSystem(nullptr)
		, m_rhiFrameExecutor(nullptr)
		, m_rhiTaskExecutor(nullptr)
		, m_rhiWorker(nullptr)
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
		if (!InitRHI(renderDesc))
		{
			LOGERROR() << "[Engine] Failed to initialize the rhi";
			return false;
		}

		// Initialize the render graph
		{
			m_renderGraph = Memory::MakeRef<RenderGraph>();
			if (!m_renderGraph)
			{
				LOGERROR() << "[Engine] Failed to create the render graph";
				return false;
			}			
		}

		// Initialize the render worker
		{
			m_renderWorker = Memory::MakeRef<RenderWorker>();
			if (!m_renderWorker)
			{
				LOGERROR() << "[Engine] Failed to create the render worker";
				return false;
			}

			m_renderWorker->SetGraph(m_renderGraph);
			m_renderWorker->SetFrameContext(m_frameContext);
			m_renderWorker->SetExecutor(m_rhiFrameExecutor);
		}

		LOGINFO() << "[Engine] Renderer initialized successfully";

		return true;
	}

	bool Engine::InitRHI(const RenderDesc& renderDesc)
	{
		// Initialize the RHI System
		{
			if (!m_window)
			{
				LOGERROR() << "[Engne] The Window is invalid, failed to initialize the render";
				return false;
			}

			void* nativeHandle = m_window->GetNativeHandle();
			if (nullptr == nativeHandle)
			{
				LOGERROR() << "[Engine] The Window's native handle is invalide, failed to initialize the rhi system";
				return false;
			}

			if (eRenderType::eOpenGL == renderDesc.Type)
			{
				m_rhiSystem = Memory::MakeRef<GLSystem>();
			}
			else
			{
				LOGERROR() << "[Engine] Not implemented Graphics API, failed to initialize the rhi system";
				return false;
			}

			if (!m_rhiSystem->Init(nativeHandle))
			{
				LOGERROR() << "[Engine] Failed to initialize the rhi system";
				return false;
			}
		}

		// Initialize the RHI Command Executor
		{
			Memory::RefPtr<RHIFrameExecutor> frameExecutor = Memory::MakeRef<RHIFrameExecutor>();
			if (!frameExecutor)
			{
				LOGERROR() << "[Engine] Failed to create the rhi frame executor";
				return false;
			}

			if (!frameExecutor->Init(renderDesc.FrameCount))
			{
				LOGERROR() << "[Engine] Failed to initialize the rhi frame executor";
				return false;
			}

			m_rhiFrameExecutor = frameExecutor;

			m_rhiTaskExecutor = Memory::MakeRef<RHITaskExecutor>();
			if (!m_rhiTaskExecutor)
			{
				LOGERROR() << "[Engine] Failed to create the rhi task executor";
				return false;
			}
		}

		// Initialize the RHI Worker
		{
			m_rhiWorker = Memory::MakeRef<RHIWorker>();
			if (!m_rhiWorker)
			{
				LOGERROR() << "[Engine] Failed to create the rhi worker";
				return false;
			}

			m_rhiWorker->SetFrameExecutor(m_rhiFrameExecutor);
			m_rhiWorker->SetTaskExecutor(m_rhiTaskExecutor);
			m_rhiWorker->SetSystem(m_rhiSystem);
		}

		LOGINFO() << "[Engine] Succeed to initialize the rhi";

		return true;
	}

	bool Engine::InitWorld()
	{
		// Initialize the main world
		{
			m_world = Memory::MakePtr<World>();
			if (!m_world)
			{
				LOGERROR() << "[Engine] Failed to create the world";
				return false;
			}
		}

		// Initialize the world worker
		{
			m_worldWorker = Memory::MakeRef<WorldWorker>();
			if (!m_worldWorker)
			{
				LOGERROR() << "[Engine] Failed to create the world worker";
				return false;
			}

			m_worldWorker->SetInputStorage(m_inputStorage);
			m_worldWorker->SetFrameContext(m_frameContext);
			m_worldWorker->SetWorld(m_world);
		}

		LOGINFO() << "[Engine] Succeed to initialize the world";

		return true;
	}

	bool Engine::InitAsset()
	{
		// TODO
		return true;
	}

	void Engine::Shutdown()
	{
		LOGINFO() << "[Engine] Shutting down Engine";

		if (m_worldWorker)
		{
			m_worldWorker->Stop();
		}

		if (m_assetWorker)
		{
			m_assetWorker->Stop();
		}

		if (m_renderWorker)
		{
			m_renderWorker->Stop();
		}

		if (m_rhiWorker)
		{
			m_rhiWorker->Stop();
		}

		if (m_window)
		{
			m_window->Shutdown();
		}

		Memory::Release();

		LOGINFO() << "[Engine] Engine shut down successfully";
	}

	void Engine::Run()
	{
		LOGINFO() << "[Engine] Running Engine";

		if (m_rhiWorker)
		{
			m_rhiWorker->Start();
		}

		if (m_assetWorker)
		{
			m_assetWorker->Start();
		}

		if (m_renderWorker)
		{
			m_renderWorker->Start();
		}

		if (m_worldWorker)
		{
			m_worldWorker->Start();
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

	Memory::RefPtr<RenderGraph> Engine::GetGraph()
	{
		return m_renderGraph;
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