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

		texture->rawBuffer = Memory::MakeRef<FormattedBuffer>();
		if (!texture->rawBuffer)
		{
			LOGINFO() << "[STDIMAGE] Failed to create the raw buffer for the texture asset : " << asset->path;
			return false;
		}

		texture->width = width;
		texture->height = height;
		texture->depth = 1;
		texture->mipLevels = 1;
		texture->sampleCount = 1;
		texture->pixelFormat = (channels == 4) ? ePixelFormat::eR8G8B8A8_UNorm : ePixelFormat::eR8G8B8_UNorm;

		texture->rawBuffer->data.Resize(width * height * channels);
		texture->rawBuffer->desc.pointer = texture->rawBuffer->data.Data();
		texture->rawBuffer->desc.componentType = eDataType::eUByte;
		texture->rawBuffer->desc.numComponents = static_cast<uint32_t>(channels);
		texture->rawBuffer->desc.count = static_cast<uint32_t>(width * height);

		memcpy(texture->rawBuffer->data.Data(), pixels, width * height * channels);

		// TODO : Get the pixel format from the file extension or the file header, 
		// currently we just assume the pixel format based on the number of channels, 
		// which is not always correct

		stbi_image_free(pixels);
		return true;
	}
}