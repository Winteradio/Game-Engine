#ifndef __WTR_APPLICATION_H__
#define __WTR_APPLICATION_H__

namespace wtr
{
	class Engine;

	class Application
	{
		public :
			Application() = default;
			virtual ~Application() = default;

		public :
			virtual void onInit(Engine& engine) = 0;
			virtual void onUpdate(const float deltaTime) = 0;
			virtual void onShutdown() = 0;
	};
};

#endif // __WTR_APPLICATION_H__