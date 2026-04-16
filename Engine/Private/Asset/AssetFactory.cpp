#include <Asset/AssetFactory.h>

#include <Asset/AssetTypes.h>
#include <Asset/AssetUtils.h>
#include <Container/include/StaticArray.h>
#include <Memory/include/Core.h>
#include <Log/include/Log.h>
#include <string>

namespace wtr
{
	using AssetType = std::pair<eExtension, eAsset>;

	const wtr::StaticArray<AssetType, static_cast<size_t>(eExtension::eEnd)> ASSET_TYPE = 
	{
		{ eExtension::eNone, eAsset::eNone },
		{ eExtension::eOBJ, eAsset::eMesh },
		{ eExtension::eFBX, eAsset::eNone },
		{ eExtension::ePNG, eAsset::eTexture },
		{ eExtension::eJPG, eAsset::eTexture },
		{ eExtension::eBMP, eAsset::eTexture },
		{ eExtension::eGLSL, eAsset::eShader },
		{ eExtension::eMTL, eAsset::eMaterial }
	};

	Memory::RefPtr<Asset> AssetFactory::Create(const std::string& path)
	{
		const eExtension extension = GetExtension(path);
		return Create(path, extension);
	}

	Memory::RefPtr<Asset> AssetFactory::Create(const std::string& path, const eExtension extension)
	{
		const eAsset assetType = ASSET_TYPE[static_cast<size_t>(extension)].second;

		Memory::RefPtr<Asset> asset = nullptr;
		switch (assetType)
		{
		case eAsset::eMesh:
			asset = Memory::MakeRef<MeshAsset>(path, extension);
			break;

		case eAsset::eMaterial:
			asset = Memory::MakeRef<MaterialAsset>(path, extension);
			break;

		case eAsset::eTexture:
			asset = Memory::MakeRef<TextureAsset>(path, extension);
			break;

		case eAsset::eShader:
			asset = Memory::MakeRef<ShaderAsset>(path, extension);
			break;

		default :
			break;
		}

		if (asset)
		{
			asset->name = AssetUtils::GetName(path);
		}
		else
		{
			LOGINFO() << "[AssetFactory] Unsupported asset type: " << path;
		}

		return asset;
	}

	eExtension AssetFactory::GetExtension(const std::string& path)
	{
		const size_t dotPos = path.find_last_of('.');
		if (dotPos == std::string::npos)
		{
			return eExtension::eNone;
		}

		const std::string extension = path.substr(dotPos + 1);
		if (extension == "obj")
		{
			return eExtension::eOBJ;
		}
		else if (extension == "fbx")
		{
			return eExtension::eFBX;
		}
		else if (extension == "png")
		{
			return eExtension::ePNG;
		}
		else if (extension == "jpg" || extension == "jpeg")
		{
			return eExtension::eJPG;
		}
		else if (extension == "bmp")
		{
			return eExtension::eBMP;
		}
		else if (extension == "glsl")
		{
			return eExtension::eGLSL;
		}
		else if (extension == "mtl")
		{
			return eExtension::eMTL;
		}
		else
		{
			return eExtension::eNone;
		}
	}
}