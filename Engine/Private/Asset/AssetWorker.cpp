#include <Asset/AssetWorker.h>

namespace wtr
{
	AssetWorker::AssetWorker()
		: Worker()
		, m_refManager()
		, m_refLoader()
		, m_threads()
	{
		m_threads.Resize(4);
		for (auto& thread : m_threads)
		{
			thread.Start();
			thread.Wait();
		}
	}

	AssetWorker::~AssetWorker()
	{
		for (auto& thread : m_threads)
		{
			thread.Stop();
		}
	}

	void AssetWorker::SetManager(const Memory::RefPtr<AssetManager> assetManager)
	{
		if (assetManager)
		{
			m_refManager = assetManager;
		}
	}

	void AssetWorker::SetLoader(const Memory::RefPtr<AssetLoader> assetLoader)
	{
		if (assetLoader)
		{
			m_refLoader = assetLoader;
		}
	}

	void AssetWorker::SetExecutor(const Memory::RefPtr<RHIExecutor> executor)
	{
		if (executor)
		{
			m_refExecutor = executor;
		}
	}

	void AssetWorker::onStart()
	{}

	void AssetWorker::onUpdate()
	{}

	void AssetWorker::onDestroy()
	{}
}