#ifndef __WTR_ASSETWORKER_H__
#define __WTR_ASSETWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>

#include <Framework/Worker.h>
#include <Framework/TaskWorker.h>

namespace wtr
{
	class Asset;
	class AssetLoader;
	class AssetManager;
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
		void SetExecutor(const Memory::RefPtr<RHIExecutor> executor);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		void onParse(Memory::RefPtr<Asset> asset);
		void onCreate(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList);
		void onBind(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList);

	private :
		Memory::RefPtr<RHIExecutor> m_refExecutor;

		wtr::DynamicArray<Memory::RefPtr<TaskWorker>> m_threads;
	};
};

#endif // __WTR_ASSETWORKER_H__