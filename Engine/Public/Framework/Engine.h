#ifndef __WTR_ENGINE_H__
#define __WTR_ENGINE_H__

#include <Framework/Input/InputStorage.h>

namespace wtr
{
	enum class eWindowType : uint8_t;

	class Window;
	class Application;
	class InputHandler;
	class RHIDevice;
	class RHIContext;

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

			void Update();
			void UpdateInput();
			void Render();

		private :
			Window*			m_Window;
			InputHandler*	m_InputHandler;
			Application*	m_Application;
			RHIDevice*		m_RHIDevice;
			RHIContext*		m_RHIContext;

			InputStorage	m_InputStorage;
	};
};

#endif // __WTR_ENGINE_H__