#include <Asset/MTLParser.h>

#include <Log/include/Log.h>
#include <Asset/AssetTypes.h>
#include <Asset/AssetStream.h>
#include <Asset/AssetSystem.h>
#include <Asset/AssetUtils.h>
#include <Memory/include/Core.h>

namespace wtr
{
	namespace
	{
		const wtr::HashMap<std::string_view, eResourceSlot> TEXTURE_TAG_MAP =
		{
			{ "map_Ka", eResourceSlot::eAmbient },
			{ "map_Kd", eResourceSlot::eDiffuse },
			{ "map_Ks", eResourceSlot::eSpecular },
			{ "map_Ke", eResourceSlot::eEmissive },
			{ "map_d", eResourceSlot::eOpacity },
			{ "map_bump", eResourceSlot::eBump },
			{ "bump", eResourceSlot::eBump },
			{ "norm", eResourceSlot::eNormal },
			{ "map_Pr", eResourceSlot::eRoughness },
			{ "map_Pm", eResourceSlot::eMetallic },
			{ "map_Ps", eResourceSlot::eSpecular },
			{ "map_ao", eResourceSlot::eAmbientOcclusion }
		};

		const wtr::HashMap<std::string_view, eVectorSlot> VECTOR_TAG_MAP =
		{
			{ "Kd", eVectorSlot::eBaseColor },
			// Not supported the ambient, specular, emissive color
		};

		const wtr::HashMap<std::string_view, eScalarSlot> SCALAR_TAG_MAP =
		{
			{ "Pr", eScalarSlot::eRoughness },
			{ "Pm", eScalarSlot::eMetallic },
			{ "d",  eScalarSlot::eOpacity },
			{ "Tr", eScalarSlot::eOpacity }
			// Not supported the shininess, refraction values
		};
	}

	bool MTLParser::Parse(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			LOGINFO() << "[MTL] Failed to parse the mtl file, the asset is invalid";
			return false;
		}

		Memory::RefPtr<MaterialAsset> material = Memory::Cast<MaterialAsset>(asset);
		if (!material)
		{
			LOGINFO() << "[MTL] Failed to parse the mtl file, the asset is not the material asset";
			return false;
		}

		wtr::DynamicArray<uint8_t> fileBuffer = ReadBuffer(asset);
		if (fileBuffer.Empty())
		{
			LOGINFO() << "[MTL] Failed to read the mtl file : " << asset->path;

			return false;
		}

		const uint8_t* curr = fileBuffer.Data();
		const uint8_t* end = curr + fileBuffer.Size();

		while (curr != end)
		{
			const uint8_t* lineEnd = curr;
			while (lineEnd != end && *lineEnd != '\n' && *lineEnd != '\r')
			{
				lineEnd++;
			}

			std::string_view line(reinterpret_cast<const char*>(curr), static_cast<size_t>(lineEnd - curr));

			if (curr < end && (*curr == '\r' || *curr == '\n'))
			{
				curr++;
				continue;
			}

			AssetStream lineStream(line);

			std::string_view tag;
			lineStream >> tag;

			auto itrTexture = TEXTURE_TAG_MAP.Find(tag);
			if (itrTexture != TEXTURE_TAG_MAP.End())
			{
				std::string texturePath;
				lineStream >> texturePath;

				texturePath = AssetUtils::GetPath(asset->path) + "/" + texturePath;

				Memory::RefPtr<const Asset> asset = AssetSystem::Load(texturePath);
				if (!asset)
				{
					LOGINFO() << "[MTL] Failed to load the texture : " << texturePath;
				}

				Memory::RefPtr<const TextureAsset> texture = Memory::Cast<const TextureAsset>(asset);
				if (texture)
				{
					material->textures[itrTexture->second] = std::move(texture);
				}
				else
				{
					LOGINFO() << "[MTL] Failed to load the texture : " << texturePath;
				}

				curr = lineEnd;
				continue;
			}

			auto itrVector = VECTOR_TAG_MAP.Find(tag);
			if (itrVector != VECTOR_TAG_MAP.End())
			{
				fvec3 color;
				lineStream >> color.x >> color.y >> color.z;
				material->vectorValues[itrVector->second] = color;

				curr = lineEnd;
				continue;
			}

			auto itrScalar = SCALAR_TAG_MAP.Find(tag);
			if (itrScalar != SCALAR_TAG_MAP.End())
			{
				float coefficient;
				lineStream >> coefficient;
				if (tag == "Tr")
				{
					coefficient = 1.0f - coefficient;
				}
				material->scalarValues[itrScalar->second] = coefficient;
				curr = lineEnd;
				continue;
			}

			if (tag == "newmtl")
			{
				std::string materialName;
				lineStream >> materialName;

				material->name = materialName;
			}
			else
			{
				if (tag != "#" && !tag.empty())
				{
					LOGINFO() << "[MTL] Unsupported tag in the mtl file : " << std::string(tag);
				}
			}

			curr = lineEnd;
		}

		LOGINFO() << "[MTL] Succeed to parsed the mtl file : " << material->path;

		return true;
	}
}