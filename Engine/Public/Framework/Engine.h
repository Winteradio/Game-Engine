#ifndef __WTR_ENGINE_H__
#define __WTR_ENGINE_H__

#include <Memory/include/Pointer/RootPtr.h>
#include <Memory/include/Pointer/RefPtr.h>

#include <World/World.h>
#include <Renderer/Renderer.h>

namespace wtr
{
	class Window;
	class InputHandler;
	class InputStorage;
	class FrameContext;

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
			Memory::ObjectPtr<Renderer> GetRenderer();

		private :
			bool InitWindow(const WindowDesc& windowDesc);
			bool InitRender(const RenderDesc& renderDesc);
			bool InitWorld();

			void UpdateInput();

		private :
			Window*			m_window;
			InputHandler*	m_inputHandler;

			Memory::RefPtr<InputStorage>	m_inputStorage;
			Memory::RefPtr<FrameContext>	m_frameContext;
			
			Memory::RootPtr<World>	m_world;
			Memory::RootPtr<Renderer> m_renderer;
	};
};

#endif // __WTR_ENGINE_H__