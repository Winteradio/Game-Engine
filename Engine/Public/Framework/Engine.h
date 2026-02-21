#ifndef __WTR_ENGINE_H__
#define __WTR_ENGINE_H__

#include <Memory/include/Pointer/RootPtr.h>
#include <Memory/include/Pointer/RefPtr.h>

#include <World/World.h>

namespace wtr
{
	class Window;
	class InputHandler;
	class InputStorage;
	class FrameContext;
	class RenderGraph;
	class RHISystem;
	class RHICommandExecutor;
	class AssetManager;
	
	class WorldWorker;
	class RenderWorker;
	class RHIWorker;
	class AssetWorker;

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

			Memory::ObjectPtr<World> GetWorld();
			Memory::RefPtr<RenderGraph> GetGraph();

		private :
			bool InitWindow(const WindowDesc& windowDesc);
			bool InitRender(const RenderDesc& renderDesc);
			bool InitRHI(const RenderDesc& renderDesc);
			bool InitWorld();
			bool InitRHI();
			bool InitAsset();

			void UpdateInput();

		private :
			Window*			m_window;
			InputHandler*	m_inputHandler;

			Memory::RefPtr<InputStorage>	m_inputStorage;
			Memory::RefPtr<FrameContext>	m_frameContext;
			
			Memory::RootPtr<World>	m_world;
			Memory::RefPtr<RenderGraph> m_renderGraph;
			Memory::RefPtr<RHISystem> m_rhiSystem;
			Memory::RefPtr<RHICommandExecutor> m_rhiExecutor;
			Memory::RefPtr<AssetManager> m_assetManager;

			Memory::RefPtr<WorldWorker> m_worldWorker;
			Memory::RefPtr<RenderWorker> m_renderWorker;
			Memory::RefPtr<RHIWorker> m_rhiWorker;
			Memory::RefPtr<AssetWorker> m_assetWorker;
	};
};

#endif // __WTR_ENGINE_H__