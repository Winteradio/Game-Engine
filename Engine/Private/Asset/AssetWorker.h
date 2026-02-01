#ifndef __WTR_ASSETWORKER_H__
#define __WTR_ASSETWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Worker.h>

namespace wtr
{
	class AssetWorker: public Worker
	{
	public :
		using AssetFunc =  ̦std::function<void()>;

		AssetWorker();
		~AssetWorker();
	};
};

#endif // __WTR_ASSETWORKER_H__