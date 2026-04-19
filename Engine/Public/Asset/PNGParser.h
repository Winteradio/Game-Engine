#ifndef __WTR_PNGPARSER_H__
#define __WTR_PNGPARSER_H__

#include <Asset/AssetParser.h>

namespace wtr
{
	enum class ePixelFormat : uint8_t;
}

namespace wtr
{
	class PNGParser : public AssetParser
	{
	public :
		PNGParser() = default;
		virtual ~PNGParser() = default;
	public :
		virtual bool Parse(Memory::RefPtr<Asset> asset) override;
	private :
		const ePixelFormat GetPixelFormat(const int32_t channels, const bool is16Bit) const;
		bool IsSRGB(const void* buffer, const uint32_t size) const;
	};
};

#endif // __WTR_PNGPARSER_H__