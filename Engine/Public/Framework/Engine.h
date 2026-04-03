#ifndef __WTR_ENGINE_H__
#define __WTR_ENGINE_H__

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class Window;
	class InputHandler;
	class InputStorage;
	
	class WorldContext;
	
	class Renderer;

	class RHISystem;
	class RHIExecutor;

	class Worker;
	class FrameGate;

	struct WindowDesc;
	struct RenderDesc;
};

namespace wtr
{
	class Engine
	{
		public :
			Engine();
			virtual ~Engine();

		public :
			bool Init(const WindowDesc& windowDesc, const RenderDesc& renderDesc);
			void Shutdown();
			void Run();

			Memory::RefPtr<WorldContext> GetWorldContext();
			Memory::RefPtr<Renderer> GetRenderer();

		private :
			bool InitWindow(const WindowDesc& windowDesc);
			bool InitRender(const RenderDesc& renderDesc);
			bool InitRHI(const RenderDesc& renderDesc);
			bool InitWorld();
			bool InitAsset();
			bool InitWorker();

			void UpdateInput();

		private :
			Memory::RefPtr<Window>			m_window;

			Memory::RefPtr<InputHandler>	m_inputHandler;
			Memory::RefPtr<InputStorage>	m_inputStorage;
			Memory::RefPtr<FrameGate>		m_updateGate;
			Memory::RefPtr<FrameGate>		m_renderGate;
			
			Memory::RefPtr<WorldContext>	m_worldContext;

			Memory::RefPtr<Renderer>		m_renderer;

			Memory::RefPtr<RHISystem>		m_rhiSystem;
			Memory::RefPtr<RHIExecutor>		m_rhiFrameExecutor;
			Memory::RefPtr<RHIExecutor>		m_rhiTaskExecutor;

			Memory::RefPtr<Worker>			m_worldWorker;
			Memory::RefPtr<Worker>			m_renderWorker;
			Memory::RefPtr<Worker>			m_rhiWorker;
			Memory::RefPtr<Worker>			m_assetWorker;
	};
};

#endif // __WTR_ENGINE_H__