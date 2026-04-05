#ifndef __WTR_STBIMAGEPARSER_H__
#define __WTR_STBIMAGEPARSER_H__

#include <Asset/AssetParser.h>

namespace wtr
{
	class StbImageParser : public AssetParser
	{
	public :
		StbImageParser() = default;
		virtual ~StbImageParser() = default;

	public :
		virtual bool Parse(Memory::RefPtr<Asset> asset) override;
	};
}

#endif // __WTR_STBIMAGEPARSER_H__