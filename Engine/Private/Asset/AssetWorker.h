#ifndef __WTR_ASSETWORKER_H__
#define __WTR_ASSETWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>

#include <Framework/Worker.h>
#include <Framework/TaskWorker.h>

namespace wtr
{
	class AssetLoader;
	class AssetManager;
	class RHIExecutor;
};

namespace wtr
{
	class AssetWorker: public Worker
	{
	public :
		AssetWorker();
		~AssetWorker();

	public :
		void SetManager(const Memory::RefPtr<AssetManager> assetManager);
		void SetLoader(const Memory::RefPtr<AssetLoader> assetLoader);
		void SetExecutor(const Memory::RefPtr<RHIExecutor> executor);

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		Memory::RefPtr<AssetManager> m_refManager;
		Memory::RefPtr<AssetLoader> m_refLoader;
		Memory::RefPtr<RHIExecutor> m_refExecutor;

		wtr::DynamicArray<TaskWorker> m_threads;
	};
};

#endif // __WTR_ASSETWORKER_H__