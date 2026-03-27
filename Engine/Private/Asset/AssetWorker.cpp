#include <Asset/AssetWorker.h>

#include <Memory/include/Core.h>
#include <Asset/AssetSystem.h>
#include <Asset/AssetTypes.h>
#include <Asset/AssetParser.h>
#include <RHI/RHIResources.h>
#include <RHI/RHITaskExecutor.h>

#include <Log/include/Log.h>

namespace wtr
{
	AssetWorker::AssetWorker()
		: Worker()
		, m_threads()
	{}

	AssetWorker::~AssetWorker()
	{
		for (auto& threadRef : m_threads)
		{
			if (threadRef)
			{
				threadRef->Stop();
			}	
		}
	}

	void AssetWorker::SetTaskThread(const size_t count)
	{
		if (count > 0)
		{
			m_threads.Resize(count);
			for (auto& threadRef : m_threads)
			{
				threadRef = Memory::MakeRef<TaskWorker>();
				if (threadRef)
				{
					threadRef->Start();
					threadRef->Wait();
				}
			}
		}
	}

	void AssetWorker::onStart()
	{}

	void AssetWorker::onUpdate()
	{
		AssetSystem::TaskQueue taskQueue = AssetSystem::GetTask();
		while (!taskQueue.empty())
		{
			Memory::RefPtr<Asset> asset = taskQueue.front();
			taskQueue.pop();

			if (!asset)
			{
				continue;
			}

			for (auto& taskWorker : m_threads)
			{
				if (!taskWorker || !taskWorker->IsWaited())
				{
					continue;
				}

				Memory::RefPtr<AssetParser> parser = AssetSystem::GetParser(asset->path);

				auto taskFunc = [assetRef = asset, parserRef = parser]() -> Memory::RefPtr<Asset>
				{
					if (parserRef && assetRef)
					{
						parserRef->Parse(assetRef);
					}
					return assetRef;
				};

				Memory::RefPtr<Task> task = Memory::MakeRef<Task>(taskFunc);

				taskWorker->Set(task);

				break;
			}
		}
	}

	void AssetWorker::onDestroy()
	{
		for (auto& threadRef : m_threads)
		{
			if (threadRef)
			{
				threadRef->Stop();
			}
		}

		m_threads.Clear();
	}
}