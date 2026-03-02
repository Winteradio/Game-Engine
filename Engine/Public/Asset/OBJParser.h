#ifndef __WTR_OBJPARSER_H__
#define __WTR_OBJPARSER_H__

#include <Asset/AssetParser.h>

namespace wtr
{
	class OBJParser : public AssetParser
	{
	public :
		OBJParser() = default;
		virtual ~OBJParser() = default;

	public :
		virtual void Parse(Memory::RefPtr<Asset> asset) override;
	};
};

#endif // __WTR_OBJPARSER_H__