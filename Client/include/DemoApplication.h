#ifndef __WTR_DEMO_APPLICATION_H__
#define __WTR_DEMO_APPLICATION_H__

#include <Framework/Application.h>

namespace demo
{
	class Game : public wtr::Application
	{
		public : 
			Game();
			virtual ~Game();

		public :
			void onInit() override;
			void onRun() override;
			void onShutdown() override;

		private :
			bool InitEngine();
			bool InitWorld();
			bool InitScene();
	};
};

#endif // __WTR_DEMO_APPLICATION_H__