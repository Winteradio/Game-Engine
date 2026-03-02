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
	{
		if (!m_refExecutor)
		{
			return;
		}

		auto cmdList = m_refExecutor->Acquire();
		if (!cmdList)
		{
			return;
		}

		AssetSystem::TaskQueue taskQueue = AssetSystem::GetTask();
		while (!taskQueue.empty())
		{
			Memory::RefPtr<Asset> asset = taskQueue.front();
			taskQueue.pop();

			if (!asset)
			{
				continue;
			}

			if (eAssetState::eNone == asset->GetState())
			{
				onParse(asset);
			}
			else if (eAssetState::eLoaded == asset->GetState())
			{
				onCreate(asset, cmdList);
			}
			else if (eAssetState::eCreated == asset->GetState())
			{
				onBind(asset, cmdList);
			}
			else
			{
				// nothing
			}
		}
	}

	void AssetWorker::onParse(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			return;
		}

		for (auto& taskWorker : m_threads)
		{
			if (!taskWorker || !taskWorker->IsWaited())
			{
				continue;
			}

			Memory::RefPtr<AssetParser> parser = AssetSystem::GetParser(asset->path);

			Memory::RefPtr<Task> task = Task::Make(
				[assetRef = asset, parserRef = parser]() -> Memory::RefPtr<Asset>
				{
					if (parserRef && assetRef)
					{
						parserRef->Parse(assetRef);
					}

					return assetRef;
				},
				[](Memory::RefPtr<Asset> asset)
				{
					if (asset)
					{
						AssetSystem::AddTask(asset);
					}
				}
			);

			taskWorker->Set(task);

			break;
		}
	}

	void AssetWorker::onCreate(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!asset || !cmdList)
		{
			return;
		}
	}

	void AssetWorker::onBind(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!asset || !cmdList)
		{
			return;
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