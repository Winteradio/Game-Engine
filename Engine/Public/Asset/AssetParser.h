#ifndef __WTR_ASSETPARSER_H__
#define __WTR_ASSETPARSER_H__

#include <Reflection/include/Type/TypeMacro.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class Asset;
};

namespace wtr
{
	class AssetParser
	{
	public :
		AssetParser() = default;
		virtual ~AssetParser() = default;	

	public :
		virtual void Parse(Memory::RefPtr<Asset> asset) = 0;
	};
};

#endif // __WTR_ASSETPARSER_H__