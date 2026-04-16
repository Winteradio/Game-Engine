#include <Asset/StbImageParser.h>

#include <Log/include/Log.h>
#include <Asset/AssetTypes.h>
#include <Framework/Math/MathTypes.h>
#include <Memory/include/Core.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace wtr
{
	bool StbImageParser::Parse(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			LOGINFO() << "[STDIMAGE] Failed to parse the image, the asset is invalid";
			return false;
		}

		Memory::RefPtr<TextureAsset> texture = Memory::Cast<TextureAsset>(asset);
		if (!texture)
		{
			LOGINFO() << "[STDIMAGE] Failed to parse the image, the asset is not the texture asset";
			return false;
		}

		wtr::DynamicArray<uint8_t> imageBuffer = ReadBuffer(asset);

		int width = 0;
		int height = 0;
		int channels = 0;

		uint8_t* pixels = stbi_load_from_memory(imageBuffer.Data(), 
			static_cast<int>(imageBuffer.Size()),
			&width, &height, &channels, 0);

		if (pixels)
		{
			LOGINFO() << "[STDIMAGE] Succeed to parse the image file : " << asset->path;
		}
		else
		{
			LOGINFO() << "[STDIMAGE] Failed to get the pixels data from the file : " << asset->path;
		}

		stbi_image_free(pixels);

		// TODO

		return true;
	}
}