#ifndef __WTR_ASSETWORKER_H__
#define __WTR_ASSETWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>

#include <Framework/Worker.h>
#include <Framework/TaskWorker.h>

namespace wtr
{
	class AssetWorker: public Worker
	{
	public :
		AssetWorker();
		~AssetWorker();

	public :
		void SetTaskThread(const size_t count);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		wtr::DynamicArray<Memory::RefPtr<TaskWorker>> m_threads;
	};
};

#endif // __WTR_ASSETWORKER_H__