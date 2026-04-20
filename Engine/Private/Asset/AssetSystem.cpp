#include <Asset/AssetSystem.h>

#include <Asset/AssetFactory.h>
#include <Asset/AssetManager.h>
#include <Asset/AssetLoader.h>
#include <Asset/AssetTypes.h>

#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <filesystem>

#include <Log/include/Log.h>

namespace wtr
{
	struct AssetCore
	{
		AssetManager manager;
		AssetLoader loader;

		AssetSystem::TaskQueue taskQueue;
		std::condition_variable cvTask;
		std::mutex mutexTask;

		std::string assetPath = std::filesystem::current_path().string() + "/../";

		std::atomic<bool> running = true;
	};

	AssetCore& GetCore()
	{
		static AssetCore core;
		return core;
	}

	Memory::RefPtr<Asset> AssetSystem::Load(const std::string& path)
	{
		AssetCore& core = GetCore();

		const std::filesystem::path inputPath(path);
		const std::string assetPath = inputPath.is_absolute() ? path : core.assetPath + path;

		std::lock_guard<std::mutex> lock(core.mutexTask);
		Memory::RefPtr<Asset> asset = core.manager.GetAsset(assetPath);
		if (asset)
		{
			return asset;
		}
		else
		{
			asset = AssetFactory::Create(assetPath);
			if (asset)
			{
				core.manager.AddAsset(assetPath, asset);
				core.taskQueue.push(asset);
			}
		}

		return asset;
	}

	void AssetSystem::Unload(const std::string& path)
	{
		AssetCore& core = GetCore();

		const std::filesystem::path inputPath(path);
		const std::string assetPath = inputPath.is_absolute() ? path : core.assetPath + path;

		std::lock_guard<std::mutex> lock(core.mutexTask);
		Memory::RefPtr<Asset> asset = core.manager.GetAsset(assetPath);
		if (asset)
		{
			asset->SetState(eAssetState::eExpried);
			core.taskQueue.push(asset);
		}
		
		core.manager.RemoveAsset(assetPath);
	}

	void AssetSystem::Shutdown()
	{
		AssetCore& core = GetCore();
		std::lock_guard<std::mutex> lock(core.mutexTask);

		core.running = false;
		core.cvTask.notify_all();
	}

	void AssetSystem::Release(Memory::RefPtr<RHICommandList> cmdList)
	{
		AssetCore& core = GetCore();
		std::lock_guard<std::mutex> lock(core.mutexTask);

		core.manager.Release(cmdList);
	}

	AssetSystem::TaskQueue AssetSystem::GetTask()
	{
		AssetCore& core = GetCore();

		{
			std::unique_lock<std::mutex> lock(core.mutexTask);
			core.cvTask.wait(lock, [&core] { return !core.taskQueue.empty() || !core.running; });
		}

		TaskQueue tasks;
		{
			std::lock_guard<std::mutex> lock(core.mutexTask);
			std::swap(tasks, core.taskQueue);
		}

		return tasks;
	}

	void AssetSystem::AddTask(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			return;
		}

		AssetCore& core = GetCore();
		{
			std::lock_guard<std::mutex> lock(core.mutexTask);
			core.taskQueue.push(asset);
		}

		core.cvTask.notify_one();
	}

	Memory::RefPtr<AssetParser> AssetSystem::GetParser(const std::string& path)
	{
		AssetCore& core = GetCore();

		const std::filesystem::path inputPath(path);
		const std::string assetPath = inputPath.is_absolute() ? path : core.assetPath + path;

		return core.loader.GetParser(assetPath);
	}
}