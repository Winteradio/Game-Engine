#ifndef __WTR_APPLICATION_H__
#define __WTR_APPLICATION_H__

#include <Framework/Engine.h>

#include <Memory/include/Pointer/RefPtr.h>
#include <Memory/include/Pointer/ObjectPtr.h>

namespace wtr
{
	class World;
	class ViewController;
	class PlayerController;
	class Renderer;
};

namespace wtr
{
	class Application
	{
		public :
			Application() = default;
			virtual ~Application() = default;

		public :
			virtual void onInit();
			virtual void onRun();
			virtual void onShutdown();

			virtual void onSetup() = 0;
			
		protected:
			bool InitEngine(const wtr::WindowDesc& windowDesc, const wtr::RenderDesc& renderDesc);
			bool InitDefault();

			virtual const WindowDesc GetWindowDesc() const;
			virtual const RenderDesc GetRenderDesc() const;

			Memory::ObjectPtr<World> GetWorld() const;
			Memory::RefPtr<ViewController> GetViewController() const;
			Memory::RefPtr<PlayerController> GetPlayerController() const;
			Memory::RefPtr<Renderer> GetRenderer() const;
			Memory::RefPtr<InputStorage> GetInputStorage() const;

		protected :
			Memory::RefPtr<Engine> m_engine;
	};
};

#endif // __WTR_APPLICATION_H__