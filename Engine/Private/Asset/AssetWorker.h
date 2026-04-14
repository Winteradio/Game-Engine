#ifndef __WTR_ASSETWORKER_H__
#define __WTR_ASSETWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>

#include <Framework/Worker.h>
#include <Framework/TaskWorker.h>

namespace wtr
{
	class Asset;
	class RHIExecutor;
	class RHICommandList;
};

namespace wtr
{
	class AssetWorker: public Worker
	{
	public :
		AssetWorker();
		~AssetWorker();

	public :
		void SetTaskThread(const size_t count);
		void SetTaskExecutor(Memory::RefPtr<RHIExecutor> taskExecutor);

	protected :
		void onUpdate() override;
		void onDestroy() override;
		void onNotify() override;

	private :
		void onProcess(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList);
		void onParse(Memory::RefPtr<Asset> asset);
		void onLoad(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList);
		void onUnload(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList);

	private :
		wtr::DynamicArray<Memory::RefPtr<TaskWorker>> m_threads;
		Memory::RefPtr<RHIExecutor> m_refTaskExecutor;
	};
};

#endif // __WTR_ASSETWORKER_H__