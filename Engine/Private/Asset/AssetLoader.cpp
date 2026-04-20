#include <Asset/AssetLoader.h>

#include <Asset/AssetTypes.h>
#include <Asset/AssetFactory.h>
#include <Asset/OBJParser.h>
#include <Asset/MTLParser.h>
#include <Asset/GLSLParser.h>
#include <Asset/PNGParser.h>

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

		m_parserMap[eExtension::ePNG] = Memory::MakeRef<PNGParser>();
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