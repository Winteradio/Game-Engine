#ifndef __WTR_DEMO_APPLICATION_H__
#define __WTR_DEMO_APPLICATION_H__

#include <Framework/Application.h>

namespace demo
{
	class Game : public wtr::Application
	{
		public : 
			Game();
			~Game();

		public :
			void onInit() override;
			void onRun() override;
			void onShutdown() override;

		private :
			wtr::Engine* engine;
	};
};

#endif // __WTR_DEMO_APPLICATION_H__