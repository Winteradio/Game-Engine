#include <Asset/AssetManager.h>

#include <Asset/AssetTypes.h>
#include <Asset/AssetCommander.h>

namespace wtr
{
	AssetManager::AssetManager()
		: m_assetMap()
		, m_mutex()
	{}

	void AssetManager::Release(Memory::RefPtr<RHICommandList> cmdList)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (auto& [path, asset] : m_assetMap)
		{
			if (asset)
			{
				asset->SetState(eAssetState::eExpried);
			}

			AssetCommander::Unload(asset, cmdList);
		}
	}

	void AssetManager::AddAsset(const std::string& path, Memory::RefPtr<Asset> asset)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_assetMap[path] = asset;
	}

	void AssetManager::RemoveAsset(const std::string& path)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_assetMap.Erase(path);
	}

	bool AssetManager::HasAsset(const std::string& path) const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		auto itr = m_assetMap.Find(path);

		return itr != m_assetMap.End();
	}

	Memory::RefPtr<Asset> AssetManager::GetAsset(const std::string& path)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		auto itr = m_assetMap.Find(path);
		if (itr != m_assetMap.End())
		{
			return itr->second;
		}
		else
		{
			return nullptr;
		}
	}
};