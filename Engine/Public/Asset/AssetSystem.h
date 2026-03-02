#ifndef __WTR_ASSETSYSTEM_H__
#define __WTR_ASSETSYSTEM_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <queue>
#include <string>

namespace wtr
{
	enum class eExtensionType : uint8_t;
	class Asset;
	class AssetParser;
	class AssetWorker;
};

namespace wtr
{
	class AssetSystem
	{
	private :
		AssetSystem() = default;
		~AssetSystem() = default;

	public :
		using TaskQueue = std::queue<Memory::RefPtr<Asset>>;

		static Memory::RefPtr<Asset> Load(const std::string& path);
		static void Unload(const std::string& path);
		static void Release();

	private :
		static TaskQueue GetTask();
		static void AddTask(Memory::RefPtr<Asset> asset);

		static Memory::RefPtr<AssetParser> GetParser(const std::string& path);

		friend class AssetWorker;
	};
};

#endif // __WTR_ASSETSYSTEM_H__