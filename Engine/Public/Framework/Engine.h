#ifndef __WTR_ENGINE_H__
#define __WTR_ENGINE_H__

#include <Framework/Input/InputStorage.h>
#include <Renderer/RenderWorker.h>
#include <Renderer/RenderContext.h>
#include <World/WorldWorker.h>

namespace wtr
{
	enum class eWindowType : uint8_t;

	class Window;
	class Application;
	class InputHandler;

	struct WindowDesc;
};

namespace wtr
{
	class Engine
	{
		public :
			Engine();
			virtual ~Engine();

		public :
			bool Init(const WindowDesc& mainWindowDesc);
			void Shutdown();
			void Run();

		private :
			bool InitWindow(const WindowDesc& mainWindowDesc);

			void UpdateInput();

		private :
			Window*			m_window;
			InputHandler*	m_inputHandler;
			Application*	m_application;

			Memory::RefPtr<InputStorage> m_inputStorage;
			Memory::RefPtr<RenderContext> m_renderContext;
			
			WorldWorker		m_worldWorker;
			RenderWorker	m_renderWorker;
	};
};

#endif // __WTR_ENGINE_H__