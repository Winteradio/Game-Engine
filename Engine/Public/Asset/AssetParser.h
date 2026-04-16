#ifndef __WTR_ASSETPARSER_H__
#define __WTR_ASSETPARSER_H__

#include <Reflection/include/Type/TypeMacro.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>

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
		virtual bool Parse(Memory::RefPtr<Asset> asset) = 0;

	protected : 
		std::stringstream ReadStream(Memory::RefPtr<Asset> asset);
		wtr::DynamicArray<uint8_t> ReadBuffer(Memory::RefPtr<Asset> asset);
	};
};

#endif // __WTR_ASSETPARSER_H__