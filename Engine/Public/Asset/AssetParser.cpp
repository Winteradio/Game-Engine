#include <Asset/AssetParser.h>

#include <Asset/AssetTypes.h>
#include <Log/include/Log.h>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace wtr
{
	std::stringstream AssetParser::ReadStream(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			LOGINFO() << "[PARSER] Failed to read the file stream data";
			return {};
		}

		const std::string& path = asset->path;
		std::ifstream file(path);
		if (!file.is_open())
		{
			LOGINFO() << "[PARSER] Failed to read the file stream data : " << path;
			return {};
		}

		std::stringstream fileStream;
		fileStream << file.rdbuf();

		LOGINFO() << "[PARSER] Succeed to read the file stream data : " << path;

		return fileStream;
	}

	wtr::DynamicArray<uint8_t> AssetParser::ReadBuffer(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			LOGINFO() << "[PARSER] Failed to read the file raw buffer";
			return {};
		}

		const std::string& path = asset->path;
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			LOGINFO() << "[PARSE] Failed to read the file raw buffer : " << path;
			return {};
		}

		const std::streamsize bufferSize = file.tellg();
		file.seekg(0, std::ios::beg);

		wtr::DynamicArray<uint8_t> rawBuffer(bufferSize);
		if (!file.read(reinterpret_cast<char*>(rawBuffer.Data()), bufferSize))
		{
			LOGINFO() << "[PARSE] Failed to copy the file raw buffer : " << path;
			return {};
		}

		return rawBuffer;
	}

	std::string AssetParser::GetPath(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			return {};
		}

		return std::filesystem::path(asset->path).parent_path().string();
	}

	std::string AssetParser::GetName(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			return {};
		}

		return std::filesystem::path(asset->path).filename().string();
	}

	std::string AssetParser::GetExtension(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			return {};
		}

		return std::filesystem::path(asset->path).extension().string();
	}

	void AssetParser::Done(Memory::RefPtr<Asset> asset)
	{
		if (asset)
		{
			asset->SetState(eAssetState::eLoaded);
		}
	}
}