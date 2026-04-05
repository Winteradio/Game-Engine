#ifndef __WTR_GLSLPARSER_H__
#define __WTR_GLSLPARSER_H__

#include <Asset/AssetParser.h>

namespace wtr
{
	class GLSLParser : public AssetParser
	{
	public :
		GLSLParser() = default;
		virtual ~GLSLParser() = default;

	public :
		virtual bool Parse(Memory::RefPtr<Asset> asset) override;
	};
};

#endif // __WTR_GLSLPARSER_H__