#ifndef __WTR_ASSETMANAGER_H__
#define __WTR_ASSETMANAGER_H__

#include <mutex>

#include <Container/include/HashMap.h>
#include <Memory/include/Pointer/RefPtr.h>

#include <string>

namespace wtr
{
	class Asset;
};

namespace wtr
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager() = default;

	public :
		void Release();
		void AddAsset(const std::string& path, Memory::RefPtr<Asset> asset);
		void RemoveAsset(const std::string& path);
		bool HasAsset(const std::string& path) const;

		Memory::RefPtr<Asset> GetAsset(const std::string& path);

	public :
		wtr::HashMap<std::string, Memory::RefPtr<Asset>> m_assetMap;
		mutable std::mutex m_mutex;
	};
};

#endif // __WTR_ASSETMANAGER_H__