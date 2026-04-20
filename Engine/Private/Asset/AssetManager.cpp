#include <Asset/AssetManager.h>

#include <Asset/AssetTypes.h>
#include <Asset/AssetCommander.h>

namespace wtr
{
	AssetManager::AssetManager()
		: m_assetMap()
	{}

	void AssetManager::Release(Memory::RefPtr<RHICommandList> cmdList)
	{
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
		m_assetMap[path] = asset;
	}

	void AssetManager::RemoveAsset(const std::string& path)
	{
		m_assetMap.Erase(path);
	}

	bool AssetManager::HasAsset(const std::string& path) const
	{
		auto itr = m_assetMap.Find(path);
		return itr != m_assetMap.End();
	}

	Memory::RefPtr<Asset> AssetManager::GetAsset(const std::string& path)
	{
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