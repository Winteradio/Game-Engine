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
			void onSetup() override;

		private :
			bool InitEntity();
			bool InitSystem();
	};
};

#endif // __WTR_DEMO_APPLICATION_H__