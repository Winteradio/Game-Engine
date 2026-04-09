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
	class RHICommandList;
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

		// TODO : 유저단에서 임의의 Asset을 생성하여 데이터를 넣을 수 있도록
		static Memory::RefPtr<Asset> Load(const std::string& path);
		static void Unload(const std::string& path);
		static void Shutdown();

	private :
		static TaskQueue GetTask();
		static void AddTask(Memory::RefPtr<Asset> asset);
		static void Release(Memory::RefPtr<RHICommandList> cmdList);

		static Memory::RefPtr<AssetParser> GetParser(const std::string& path);

		friend class AssetWorker;
	};
};

#endif // __WTR_ASSETSYSTEM_H__