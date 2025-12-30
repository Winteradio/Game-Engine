#ifndef __WTR_WORLDWORKER_H__
#define __WTR_WORLDWORKER_H__

#include <Framework/Worker.h>

namespace wtr
{
	class WorldWorker : public Worker
	{
	public :
		WorldWorker();
		~WorldWorker();

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;
	};
};

#endif // __WTR_WORLDWORKER_H__