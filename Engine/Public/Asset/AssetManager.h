#ifndef __WTR_ASSETMANAGER_H__
#define __WTR_ASSETMANAGER_H__

#include <condition_variable>
#include <mutex>
#include <queue>

#include <Container/include/HashMap.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class Asset;
	class MeshAsset;
	class MaterialAsset;
	class ShaderAsset;
};

namespace wtr
{
	class AssetManager
	{
	public:
		AssetManager();
		virtual ~AssetManager();

	public :
		static AssetManager& Get();

	private :
		wtr::HashMap<std::string, Memory::RefPtr<MeshAsset>> m_meshMap;
		wtr::HashMap<std::string, Memory::RefPtr<MaterialAsset>> m_materialMap;
		wtr::HashMap<std::string, Memory::RefPtr<ShaderAsset>> m_shaderMap;
		wtr::HashMap<std::string, Memory::RefPtr<Asset>> m_assetMap;
		std::condition_variable m_cvAsset;
		std::mutex m_mutex;
		std::queue<int> m_taskQueue;
	};
};

#endif // __WTR_ASSETMANAGER_H__