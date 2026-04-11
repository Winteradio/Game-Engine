#include <Asset/AssetUtils.h>

#include <filesystem>

namespace wtr
{
	namespace AssetUtils
	{
		const std::string GetPath(const std::string& rawPath)
		{
			return std::filesystem::path(rawPath).parent_path().string();
		}

		const std::string GetName(const std::string& rawPath)
		{
			return std::filesystem::path(rawPath).stem().string();
		}

		const std::string GetExtension(const std::string& rawPath)
		{
			return std::filesystem::path(rawPath).extension().string();
		}
	}
}