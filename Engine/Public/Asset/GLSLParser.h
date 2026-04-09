#ifndef __WTR_GLSLPARSER_H__
#define __WTR_GLSLPARSER_H__

#include <Asset/AssetParser.h>

namespace wtr
{
	enum class eShaderType : uint8_t;
};

namespace wtr
{
	class GLSLParser : public AssetParser
	{
	public :
		GLSLParser() = default;
		virtual ~GLSLParser() = default;

	public :
		virtual bool Parse(Memory::RefPtr<Asset> asset) override;

	private :
		eShaderType GetShaderType(const std::string& name) const;
	};
};

#endif // __WTR_GLSLPARSER_H__