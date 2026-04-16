#ifndef __WTR_ASSETUTILS_H__
#define __WTR_ASSETUTILS_H__

#include <string>

namespace wtr
{
	namespace AssetUtils
	{
		const std::string GetPath(const std::string& rawPath);
		const std::string GetName(const std::string& rawPath);
		const std::string GetExtension(const std::string& rawPath);
	}
}

#endif // __WTR_ASSETUTILS_H__