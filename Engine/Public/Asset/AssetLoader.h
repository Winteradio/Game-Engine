#ifndef __WTR_ASSETLOADER_H__
#define __WTR_ASSETLOADER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/HashMap.h>

#include <cstdint>

namespace wtr
{
	enum class eExtension : uint8_t;
};

namespace wtr
{
	class AssetParser;

	class AssetLoader
	{
	public :
		AssetLoader();
		virtual ~AssetLoader() = default;

	public :
		Memory::RefPtr<AssetParser> GetParser(const std::string& path);

	private :
		void Init();

	private :
		wtr::HashMap<eExtension, Memory::RefPtr<AssetParser>> m_parserMap;
	};
};

#endif // __WTR_ASSETLOADER_H__