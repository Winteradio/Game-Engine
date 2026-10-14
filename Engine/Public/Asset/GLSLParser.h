#ifndef __WTR_GLSLPARSER_H__
#define __WTR_GLSLPARSER_H__

#include <Asset/AssetParser.h>

#include <string>
#include <vector>

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
		// include를 펼치는 동안 공유하는 상태
		struct IncludeContext
		{
			// 펼친 파일 목록 : 인덱스가 #line의 source string number가 됨 (0 = 최상위 파일)
			std::vector<std::string> sources;

			// 지금 펼치는 중인 파일 인덱스 (순환 include 검사용)
			std::vector<uint32_t> stack;
		};

	private :
		eShaderType GetShaderType(const std::string& name) const;

		// #include "file"을 재귀로 펼쳐 한 덩어리 소스로 만든다 (파일 기준 상대 경로, 같은 파일은 한 번만)
		// GL은 #include를 지원하지 않고 glslang도 확장이 필요해서 RHI에는 다 펼친 소스만 넘김
		bool Expand(const std::string& path, IncludeContext& context, std::string& source) const;
	};
};

#endif // __WTR_GLSLPARSER_H__
