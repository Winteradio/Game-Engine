#ifndef __WTR_MTLPARSER_H__
#define __WTR_MTLPARSER_H__

#include <Asset/AssetParser.h>

namespace wtr
{
	class MTLParser : public AssetParser
	{
	public :
		MTLParser() = default;
		virtual ~MTLParser() = default;

	public :
		virtual void Parse(Memory::RefPtr<Asset> asset) override;
	};
};

#endif // __WTR_MTLPARSER_H__