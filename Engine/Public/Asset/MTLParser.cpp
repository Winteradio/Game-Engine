#include <Asset/MTLParser.h>

#include <Log/include/Log.h>
#include <Asset/AssetTypes.h>
#include <Framework/Math/MathTypes.h>
#include <Memory/include/Core.h>

#include <sstream>

namespace wtr
{
	struct OBJMesh
	{
		wtr::DynamicArray<wtr::fvec3> pos;
		wtr::DynamicArray<wtr::fvec2> uv;
		wtr::DynamicArray<wtr::fvec3> nor;
	};

	struct OBJVertex
	{
		wtr::fvec3 pos;
		wtr::fvec2 uv;
		wtr::fvec3 nor;
	};

	void MTLParser::Parse(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			LOGINFO() << "[OBJ] Failed to parse the obj file, the asset is invalid";
			return;
		}

		Memory::RefPtr<ComposeAsset> compose = Memory::Cast<ComposeAsset>(asset);
		if (!compose)
		{
			LOGINFO() << "[OBJ] Failed to parse the obj file, the asset is not the compose asset";
			return;
		}

		std::stringstream fileStream = ReadStream(asset);

		std::string line;
		while (std::getline(fileStream, line))
		{
			std::stringstream lineStream(line);

			std::string tag;
			lineStream >> tag;
		}



		asset->SetState(eAssetState::eLoaded);
	}
}