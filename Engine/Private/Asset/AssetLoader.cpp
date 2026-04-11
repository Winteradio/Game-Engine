#include <Asset/AssetLoader.h>

#include <Asset/AssetTypes.h>
#include <Asset/AssetFactory.h>
#include <Asset/OBJParser.h>
#include <Asset/MTLParser.h>
#include <Asset/StbImageParser.h>
#include <Asset/GLSLParser.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	AssetLoader::AssetLoader()
	{
		Init();
	}

	void AssetLoader::Init()
	{
		m_parserMap[eExtension::eOBJ] = Memory::MakeRef<OBJParser>();
		m_parserMap[eExtension::eMTL] = Memory::MakeRef<MTLParser>();
		m_parserMap[eExtension::eGLSL] = Memory::MakeRef<GLSLParser>();

		auto imageParser = Memory::MakeRef<StbImageParser>();
		m_parserMap[eExtension::eJPG] = imageParser;
		m_parserMap[eExtension::ePNG] = imageParser;
		m_parserMap[eExtension::eBMP] = imageParser;
	}

	Memory::RefPtr<AssetParser> AssetLoader::GetParser(const std::string& path)
	{
		eExtension extension = AssetFactory::GetExtension(path);
		auto it = m_parserMap.Find(extension);
		if (it != m_parserMap.End())
		{
			return it->second;
		}
		else
		{
			LOGERROR() << "[Asset] Failed to get parser for path : " << path;
			return nullptr;
		}
	}
};