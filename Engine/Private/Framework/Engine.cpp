#include <Framework/Engine.h>

#include <Framework/Input/InputStorage.h>
#include <Framework/FrameGate.h>

#include <Platform/Win32/Win32Window.h>
#include <Platform/Win32/Win32InputHandler.h>

#include <World/Commander.h>
#include <World/WorldContext.h>
#include <World/WorldWorker.h>

#include <Renderer/RenderTypes.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderWorker.h>

#include <RHI/RHISystem.h>
#include <RHI/RHIFrameExecutor.h>
#include <RHI/RHITaskExecutor.h>
#include <RHI/OpenGL/GLSystem.h>
#include <RHI/RHIWorker.h>

#include <Asset/AssetWorker.h>
#include <Asset/AssetSystem.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	static constexpr size_t UPDATE_FRAME_GAP = 3;
	static constexpr size_t RENDER_FRAME_GAP = 3;

	Engine::Engine()
		: m_window(nullptr)
		, m_inputHandler(nullptr)
		, m_inputStorage(nullptr)
		, m_updateGate(nullptr)
		, m_renderGate(nullptr)
		, m_worldContext(nullptr)
		, m_worldWorker(nullptr)
		, m_renderer(nullptr)
		, m_renderWorker(nullptr)
		, m_rhiSystem(nullptr)
		, m_rhiFrameExecutor(nullptr)
		, m_rhiTaskExecutor(nullptr)
		, m_rhiWorker(nullptr)
		, m_assetWorker(nullptr)
	{}

	Engine::~Engine()
	{
		Shutdown();
	}

	bool Engine::Init(const WindowDesc& windowDesc, const RenderDesc& renderDesc)
	{
		LOGINFO() << "[Engine] Initializing Engine";

		Log::Init(1024, Log::Enum::eMode_Print, Log::Enum::eLevel_Type);
		Memory::Init(4096, 100);

		m_inputStorage = Memory::MakeRef<InputStorage>();

		m_updateGate = Memory::MakeRef<FrameGate>(UPDATE_FRAME_GAP);
		m_renderGate = Memory::MakeRef<FrameGate>(RENDER_FRAME_GAP);

		if (!m_inputStorage)
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

		if (!InitWorker())
		{
			LOGERROR() << "[Engine] Failed to initialize the worker";
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
			m_window = Memory::MakeRef<Win32Window>();
			m_inputHandler = Memory::MakeRef<Win32InputHandler>();
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
		m_window->SetCloseCallback([this](){this->Shutdown();});

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

		m_renderer = Memory::MakeRef<Renderer>();
		if (!m_renderer)
		{
			LOGERROR() << "[Engine] Failed to create the renderer";
			return false;
		}

		if (!m_renderer->Init())
		{
			LOGERROR() << "[Engine] Failed to initialize the renderer";
			return false;
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
			Memory::RefPtr<RHIFrameExecutor> frameExecutor = Memory::MakeRef<RHIFrameExecutor>(m_rhiSystem);
			if (!frameExecutor)
			{
				LOGERROR() << "[Engine] Failed to create the rhi frame executor";
				return false;
			}

			if (!frameExecutor->Init(RENDER_FRAME_GAP))
			{
				LOGERROR() << "[Engine] Failed to initialize the rhi frame executor";
				return false;
			}

			m_rhiFrameExecutor = frameExecutor;

			Memory::RefPtr<RHITaskExecutor> taskExecutor = Memory::MakeRef<RHITaskExecutor>(m_rhiSystem);
			if (!taskExecutor)
			{
				LOGERROR() << "[Engine] Failed to create the rhi task executor";
				return false;
			}

			m_rhiTaskExecutor = taskExecutor;
		}

		LOGINFO() << "[Engine] Succeed to initialize the rhi";

		return true;
	}

	bool Engine::InitWorld()
	{
		m_worldContext = Memory::MakeRef<WorldContext>();
		if (!m_worldContext)
		{
			LOGERROR() << "[Engine] Failed to create the world";
			return false;
		}

		if (!m_worldContext->Init())
		{
			LOGERROR() << "[Engine] Failed to initialize the world context";
			return false;
		}

		if (!m_renderer || !m_renderer->GetCommandList())
		{
			LOGERROR() << "[Engine] Failed to initiailze the commander, the renderer or render command list is invalid";
			return false;
		}
		else
		{
			m_worldContext->commander->SetCommand(m_renderer->GetCommandList());
		}

		LOGINFO() << "[Engine] Succeed to initialize the world";

		return true;
	}

	bool Engine::InitAsset()
	{
		m_assetWorker = Memory::MakeRef<AssetWorker>();
		if (!m_assetWorker)
		{
			LOGERROR() << "[Engine] Failed to create the asset worker";
			return false;
		}

		LOGINFO() << "[Engine] Succeed to initialize the asset worker";

		return true;
	}

	bool Engine::InitWorker()
	{
		// Initialize the worlrd worker
		{
			Memory::RefPtr<WorldWorker> worldWorker = Memory::MakeRef<WorldWorker>();
			if (!worldWorker)
			{
				LOGERROR() << "[Engine] Failed to create the world worker";
				return false;
			}

			worldWorker->SetInputStorage(m_inputStorage);
			worldWorker->SetWorldContext(m_worldContext);
			worldWorker->SetProducer(m_updateGate);

			m_worldWorker = worldWorker;
		}

		// Initialize the render worker
		{
			Memory::RefPtr<RenderWorker> renderWorker = Memory::MakeRef<RenderWorker>();
			if (!renderWorker)
			{
				LOGERROR() << "[Engine] Failed to create the render worker";
				return false;
			}

			renderWorker->SetExecutor(m_rhiFrameExecutor);
			renderWorker->SetRenderer(m_renderer);
			renderWorker->SetConsumer(m_updateGate);
			renderWorker->SetProducer(m_renderGate);

			m_renderWorker = renderWorker;
		}

		// Initialize the rhi worker
		{
			Memory::RefPtr<RHIWorker> rhiWorker = Memory::MakeRef<RHIWorker>();
			if (!rhiWorker)
			{
				LOGERROR() << "[Engine] Failed to create the rhi worker";
				return false;
			}

			rhiWorker->SetFrameExecutor(m_rhiFrameExecutor);
			rhiWorker->SetTaskExecutor(m_rhiTaskExecutor);
			rhiWorker->SetSystem(m_rhiSystem);
			rhiWorker->SetConsumer(m_renderGate);
			
			m_rhiWorker = rhiWorker;
		}

		// Initialize the asset worker
		{
			Memory::RefPtr<AssetWorker> assetWorker = Memory::MakeRef<AssetWorker>();
			if (!assetWorker)
			{
				LOGERROR() << "[Engine] Failed to create the asset worker";
				return false;
			}

			assetWorker->SetTaskThread(4);
			assetWorker->SetTaskExecutor(m_rhiTaskExecutor);

			m_assetWorker = assetWorker;
		}
		
		LOGINFO() << "[Engine] Succeed to initialize the worker";

		return true;
	}

	void Engine::Shutdown()
	{
		LOGINFO() << "[Engine] Shutting down Engine";

		AssetSystem::Shutdown();

		if (m_worldContext)
		{
			m_worldContext->Clear();
			m_worldContext.Reset();
		}

		if (m_updateGate)
		{
			m_updateGate->NotifyAll();
		}

		if (m_renderGate)
		{
			m_renderGate->NotifyAll();
		}

		if (m_worldWorker)
		{
			m_worldWorker->Stop();
			m_worldWorker.Reset();
		}

		if (m_assetWorker)
		{
			m_assetWorker->Stop();
			m_assetWorker.Reset();
		}

		if (m_renderWorker)
		{
			m_renderWorker->Stop();
			m_renderWorker.Reset();
		}

		if (m_rhiWorker)
		{
			m_rhiWorker->Stop();
			m_rhiWorker.Reset();
		}

		if (m_window)
		{
			m_window->Shutdown();
			m_window.Reset();
		}

		if (m_inputHandler)
		{
			m_inputHandler.Reset();
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

		while (m_window && m_window->GetStatus() != eWindowStatus::eClosed)
		{
			m_window->PollEvents();

			UpdateInput();
		}

		LOGINFO() << "[Engine] Engine run completed";
	}

	Memory::RefPtr<InputStorage> Engine::GetInputStorage()
	{
		return m_inputStorage;
	}

	Memory::RefPtr<WorldContext> Engine::GetWorldContext()
	{
		return m_worldContext;
	}

	Memory::RefPtr<Renderer> Engine::GetRenderer()
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