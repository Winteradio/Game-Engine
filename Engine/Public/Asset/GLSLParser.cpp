#include <Asset/GLSLParser.h>

#include <Log/include/Log.h>
#include <Asset/AssetTypes.h>
#include <Asset/AssetStream.h>
#include <Memory/include/Core.h>

namespace wtr
{
	bool GLSLParser::Parse(Memory::RefPtr<Asset> asset)
	{
		if (!asset)
		{
			LOGINFO() << "[GLSL] Failed to parse the glsl file, the asset is invalid";
			return false;
		}

		Memory::RefPtr<ShaderAsset> shader = Memory::Cast<ShaderAsset>(asset);
		if (!shader)
		{
			LOGINFO() << "[GLSL] Failed to parse the glsl file, the asset is not the shader asset";
			return false;
		}

		wtr::DynamicArray<uint8_t> fileBuffer = ReadBuffer(asset);
		if (fileBuffer.Empty())
		{
			LOGINFO() << "[GLSL] Failed to read the glsl file : " << asset->path;
			return false;
		}

		shader->rawBuffer = Memory::MakeRef<RawBuffer>();

		auto& rawBuffer = shader->rawBuffer;
		if (!rawBuffer)
		{
			LOGINFO() << "[GLSL] Failed to create the raw buffer for the shader asset : " << asset->path;
			return false;
		}

		rawBuffer->data = std::move(fileBuffer);

		LOGINFO() << "[GLSL] Succeed to parse the glsl file : " << asset->path;

		return true;
	}
}