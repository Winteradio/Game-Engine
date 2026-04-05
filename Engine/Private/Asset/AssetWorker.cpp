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

					DispatchTask(assetRef);
				};

				Memory::RefPtr<DefaultTask> task = Memory::MakeRef<DefaultTask>(taskFunc);

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

	void AssetWorker::DispatchTask(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			return;
		}

		if (asset->type == eAsset::eMesh)
		{
			Memory::RefPtr<MeshAsset> mesh = Memory::Cast<MeshAsset>(asset);
			if (!mesh)
			{
				return;
			}

			for (auto& material : mesh->materials)
			{
				if (!material)
				{
					continue;
				}

				AssetSystem::AddTask(material);
			}
		}
		else if (asset->type == eAsset::eMaterial)
		{
			Memory::RefPtr<MaterialAsset> material = Memory::Cast<MaterialAsset>(asset);
			if (!material)
			{
				return;
			}

			for (auto& [slot, texture] : material->textures)
			{
				if (!texture)
				{
					continue;
				}

				AssetSystem::AddTask(texture);
			}
		}
		else
		{
			// TODO
		}
	}
}