#include <Asset/AssetWorker.h>

#include <Memory/include/Core.h>
#include <Asset/AssetCommander.h>
#include <Asset/AssetSystem.h>
#include <Asset/AssetTypes.h>
#include <Asset/AssetParser.h>
#include <RHI/RHIExecutor.h>
#include <RHI/RHICommandList.h>

#include <Log/include/Log.h>

namespace wtr
{
	AssetWorker::AssetWorker()
		: Worker()
		, m_threads()
	{}

	AssetWorker::~AssetWorker()
	{
		Stop();

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

	void AssetWorker::SetTaskExecutor(Memory::RefPtr<RHIExecutor> taskExecutor)
	{
		m_refTaskExecutor = taskExecutor;
	}

	void AssetWorker::onUpdate()
	{
		if (!m_refTaskExecutor)
		{
			return;
		}

		Memory::RefPtr<RHICommandList> cmdList = m_refTaskExecutor->Acquire();

		AssetSystem::TaskQueue taskQueue = AssetSystem::GetTask();
		while (!taskQueue.empty())
		{
			Memory::RefPtr<Asset> asset = taskQueue.front();
			taskQueue.pop();

			if (!asset)
			{
				continue;
			}

			onProcess(asset, cmdList);
		}

		m_refTaskExecutor->Submit(cmdList);
	}

	void AssetWorker::onDestroy()
	{
		if (!m_refTaskExecutor)
		{
			return;
		}

		bool processing = true;
		while (processing)
		{
			processing = false;

			for (auto& taskWorker : m_threads)
			{
				if (taskWorker && taskWorker->IsTasking())
				{
					processing = true;
					break;
				}
			}

			if (processing)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

		Memory::RefPtr<RHICommandList> cmdList = m_refTaskExecutor->Acquire();
		if (!cmdList)
		{
			return;
		}

		AssetSystem::Release(cmdList);

		m_refTaskExecutor->Submit(cmdList);
	}

	void AssetWorker::onNotify()
	{
		for (auto& threadRef : m_threads)
		{
			if (threadRef)
			{
				threadRef->Stop();
			}
		}
	}

	void AssetWorker::onProcess(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!asset || !cmdList)
		{
			return;
		}

		const eAssetState assetState = asset->GetState();
		if (assetState == eAssetState::eNone)
		{
			onParse(asset);
		}
		else if (assetState == eAssetState::eLoaded)
		{
			onLoad(asset, cmdList);
		}
		else if (assetState == eAssetState::eExpried)
		{
			onUnload(asset, cmdList);
		}
		else
		{
			LOGERROR() << "[AssetWorker] The invalid asset state : " << static_cast<uint32_t>(assetState);
			return;
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

			auto taskFunc = [assetRef = asset, parserRef = parser]() -> void
				{
					if (!parserRef || !assetRef)
					{
						return;
					}

					if (!parserRef->Parse(assetRef))
					{
						LOGERROR() << "[AssetWorker] Failed to parse the asset : " << assetRef->path;
						return;
					}
					assetRef->SetState(eAssetState::eLoaded);

					AssetSystem::AddTask(assetRef);
				};

			Memory::RefPtr<DefaultTask> task = Memory::MakeRef<DefaultTask>(taskFunc);

			taskWorker->Set(task);

			break;
		}
	}

	void AssetWorker::onLoad(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!asset || !cmdList)
		{
			return;
		}

		AssetCommander::Load(asset, cmdList);
	}

	void AssetWorker::onUnload(Memory::RefPtr<Asset> asset, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!asset || !cmdList)
		{
			return;
		}

		AssetCommander::Unload(asset, cmdList);
	}
}