#ifndef __WTR_ASSETFACTORY_H__
#define __WTR_ASSETFACTORY_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <cstdint>

namespace wtr
{
	class Asset;

	enum class eExtension : uint8_t;
	enum class eAsset : uint8_t;
};

namespace wtr
{
	namespace AssetFactory
	{
		Memory::RefPtr<Asset> Create(const std::string& path);
		Memory::RefPtr<Asset> Create(const std::string& path, const eExtension extension);
		eExtension GetExtension(const std::string& path);
	};
};

#endif // __WTR_ASSETFACTORY_H__