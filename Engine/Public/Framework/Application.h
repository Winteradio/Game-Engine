#ifndef __WTR_APPLICATION_H__
#define __WTR_APPLICATION_H__

#include <Framework/Engine.h>

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class Engine;

	class Application
	{
		public :
			Application() = default;
			virtual ~Application() = default;

		public :
			virtual void onInit() = 0;
			virtual void onRun() = 0;
			virtual void onShutdown() = 0;

		protected :
			Memory::RefPtr<Engine> m_engine;
	};
};

#endif // __WTR_APPLICATION_H__