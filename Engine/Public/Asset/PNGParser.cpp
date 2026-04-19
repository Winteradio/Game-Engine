#include <Asset/PNGParser.h>

#include <Log/include/Log.h>
#include <Memory/include/Core.h>
#include <Asset/AssetTypes.h>
#include <Asset/StbiImage.h>

namespace wtr
{
	bool PNGParser::Parse(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			LOGERROR() << "[PNGParser] Failed to parse the asset, the asset is null";
			return false;
		}

		Memory::RefPtr<TextureAsset> textureAsset = Memory::Cast<TextureAsset>(asset);
		if (!textureAsset)
		{
			LOGERROR() << "[PNGParser] Failed to parse the asset, the asset is not the texture asset";
			return false;
		}

		const wtr::DynamicArray<uint8_t> fileBuffer = ReadBuffer(asset);
		if (fileBuffer.Empty())
		{
			LOGERROR() << "[PNGParser] Failed to read the asset : " << asset->path;
			return false;
		}

		StbiImage image;
		if (!image.LoadFromBuffer(fileBuffer.Data(), fileBuffer.Size()))
		{
			LOGERROR() << "[PNGParser] Failed to load the image from buffer : " << asset->path;
			return false;
		}

		textureAsset->width = image.GetWidth();
		textureAsset->height = image.GetHeight();

		// The PNG image is 2D, so the depth is always 1
		// And the sample count is always 1 since the PNG image is not a multisampled texture
		// The mip levels can be generated later when creating the texture resource, so we can set it to 1 for now
		textureAsset->depth = 1;
		textureAsset->mipLevels = 1;
		textureAsset->sampleCount = 1;

		const bool isSRGB = IsSRGB(fileBuffer.Data(), fileBuffer.Size());
		textureAsset->isSRGB = isSRGB;
		textureAsset->pixelFormat = isSRGB ? ePixelFormat::eR8G8B8A8_sRGB : GetPixelFormat(image.GetChannels(), image.Is16Bit());
		if (textureAsset->pixelFormat == ePixelFormat::eNone)
		{
			LOGERROR() << "[PNGParser] Invalid the pixel format : " << static_cast<uint8_t>(textureAsset->pixelFormat);
			return false;
		}

		// TODO : Determine if the image is a cubemap based on the file name or the image data
		textureAsset->isCubemap = false;

		// TODO : Determine the texture's faces based on the image data, for now we can set it to 1 since the PNG image is not a cubemap
		textureAsset->faces = 1;

		textureAsset->rawBuffer = Memory::MakeRef<TextureBuffer>();
		if (!textureAsset->rawBuffer)
		{
			LOGERROR() << "[PNGParser] Failed to create the texture raw buffer, the path : " << asset->path;
			return false;
		}

		auto& textureBuffer = textureAsset->rawBuffer;
		textureBuffer->data.Resize(image.GetSize());
		memcpy(textureBuffer->data.Data(), image.GetPixels(), image.GetSize());

		size_t dataOffset = 0;
		textureBuffer->desc.componentType = image.Is16Bit() ? eDataType::eFloat : eDataType::eUInt;
		textureBuffer->desc.faces.Resize(textureAsset->faces);
		for (auto& textureFace : textureBuffer->desc.faces)
		{
			textureFace.mipMaps.Resize(textureAsset->mipLevels);

			for (uint32_t mipLevel = 0; mipLevel < textureAsset->mipLevels; mipLevel++)
			{
				TextureMipMapDesc& mipMapDesc = textureFace.mipMaps[mipLevel];

				mipMapDesc.width = textureAsset->width >> mipLevel;
				mipMapDesc.width = mipMapDesc.width > 0 ? mipMapDesc.width : 1;

				mipMapDesc.height = textureAsset->height >> mipLevel;
				mipMapDesc.height = mipMapDesc.height > 0 ? mipMapDesc.height : 1;

				mipMapDesc.depth = textureAsset->depth >> mipLevel;
				mipMapDesc.depth = mipMapDesc.depth > 0 ? mipMapDesc.depth : 1;

				mipMapDesc.channels = image.GetChannels();
				mipMapDesc.size = mipMapDesc.width * mipMapDesc.height * mipMapDesc.depth * mipMapDesc.channels * (image.Is16Bit() ? sizeof(uint16_t) : sizeof(uint8_t));

				mipMapDesc.pointer = static_cast<const uint8_t*>(textureBuffer->data.Data()) + dataOffset;

				dataOffset += mipMapDesc.size;
			}
		}

		LOGINFO() << "[PNGParser] Succeed to parse the asset : " << asset->path;

		return true;
	}

	const ePixelFormat PNGParser::GetPixelFormat(const int32_t channels, const bool is16Bit) const
	{
		if (is16Bit)
		{
			switch (channels)
			{
			case 1:
				return ePixelFormat::eR16_Float;
			case 2:
				return ePixelFormat::eR16G16_Float;
			case 3:
				return ePixelFormat::eR16G16B16_Float;
			case 4:
				return ePixelFormat::eR16G16B16A16_Float;
			default:
				LOGERROR() << "[PNGParser] Invalid number of channels : " << channels;
				return ePixelFormat::eNone;
			}
		}
		else
		{
			switch (channels)
			{
			case 1:
				return ePixelFormat::eR8_UNorm;
			case 2:
				return ePixelFormat::eR8G8_UNorm;
			case 3:
				return ePixelFormat::eR8G8B8_UNorm;
			case 4:
				return ePixelFormat::eR8G8B8A8_UNorm;
			default:
				LOGERROR() << "[PNGParser] Invalid number of channels : " << channels;
				return ePixelFormat::eNone;
			}
		}
	}

	bool PNGParser::IsSRGB(const void* buffer, const uint32_t size) const
	{
		if (!buffer || size == 0)
		{
			LOGERROR() << "[PNGParser] Failed to check if the image is sRGB, the buffer is invalid";
			return false;
		}

		constexpr size_t PNG_SIGNATURE_SIZE = 8;
		constexpr size_t PNG_IHDR_CHUNK_SIZE = 25; // 4 bytes length + 4 bytes type + 13 bytes data + 4 bytes CRC

		size_t offset = PNG_SIGNATURE_SIZE + PNG_IHDR_CHUNK_SIZE;
		if (offset >= size)
		{
			LOGERROR() << "[PNGParser] Failed to check if the image is sRGB, the buffer size is too small";
			return false;
		}

		constexpr uint32_t PNG_SRGB_CHUNK_LENGTH = 4;
		constexpr uint32_t PNG_SRGB_CHUNK_TYPE = 0x73524742; // "sRGB" in ASCII

		offset += PNG_SRGB_CHUNK_LENGTH;
		if (offset >= size)
		{
			LOGERROR() << "[PNGParser] Failed to check if the image is sRGB, the buffer size is too small for sRGB chunk";
			return false;
		}

		const void* sRGBData = static_cast<const uint8_t*>(buffer) + offset;
		std::string chunkType(reinterpret_cast<const char*>(sRGBData), 4);
		if (chunkType != "sRGB")
		{
			return false;
		}
		else
		{
			return true;
		}
	}
};