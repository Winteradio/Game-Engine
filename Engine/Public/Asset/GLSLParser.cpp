#include <Asset/GLSLParser.h>

#include <Log/include/Log.h>
#include <Asset/AssetTypes.h>
#include <Asset/AssetStream.h>
#include <Memory/include/Core.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace wtr
{
	namespace
	{
		std::string Normalize(const std::string& path)
		{
			std::error_code error;

			std::filesystem::path normalized = std::filesystem::weakly_canonical(std::filesystem::u8path(path), error);
			if (error)
			{
				normalized = std::filesystem::u8path(path).lexically_normal();
			}

			std::string result = normalized.generic_string();

			// Windows 경로는 대소문자를 구분하지 않으므로 같은 파일 비교용으로 소문자로 맞춤
			std::transform(result.begin(), result.end(), result.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

			return result;
		}

		// 줄 앞 공백을 건너뛴 위치에서 keyword로 시작하는지 확인하고, 그 뒤 위치를 돌려준다
		bool StartsWithDirective(const std::string& line, const char* keyword, size_t& next)
		{
			size_t pos = line.find_first_not_of(" \t");
			if (pos == std::string::npos || line[pos] != '#')
			{
				return false;
			}

			pos = line.find_first_not_of(" \t", pos + 1);
			if (pos == std::string::npos)
			{
				return false;
			}

			const size_t length = std::char_traits<char>::length(keyword);
			if (line.compare(pos, length, keyword) != 0)
			{
				return false;
			}

			next = pos + length;
			return true;
		}
	}

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

		const eShaderType shaderType = GetShaderType(shader->name);
		shader->SetShaderType(shaderType);

		// RHI에는 include가 다 펼쳐진 소스 하나만 넘긴다 (파일 읽기는 에셋 스레드에서 끝냄)
		IncludeContext context;
		std::string source;
		if (!Expand(asset->path, context, source))
		{
			LOGINFO() << "[GLSL] Failed to expand the glsl file : " << asset->path;
			return false;
		}

		Memory::RefPtr<ArrayData<uint8_t>> arrayData = Memory::MakeRef<ArrayData<uint8_t>>();
		if (!arrayData)
		{
			LOGINFO() << "[GLSL] Failed to create the bulk data for the shader asset : " << asset->path;
			return false;
		}

		arrayData->data.Resize(source.size());
		memcpy(arrayData->data.Data(), source.data(), source.size());
		shader->rawBuffer.bulkData = arrayData;

		LOGINFO() << "[GLSL] Succeed to parse the glsl file : " << asset->path << ", included files : " << (context.sources.size() - 1);

		return true;
	}

	eShaderType GLSLParser::GetShaderType(const std::string& name) const
	{
		const size_t dosPos = name.find_last_of('.');
		if (dosPos == std::string::npos || dosPos == name.size() - 1)
		{
			return eShaderType::eNone;
		}

		const std::string extension = name.substr(dosPos + 1);
		if (extension == "vs")
		{
			return eShaderType::eVertex;
		}
		else if (extension == "gs")
		{
			return eShaderType::eGeometry;
		}
		else if (extension == "hs")
		{
			return eShaderType::eHull;
		}
		else if (extension == "ds")
		{
			return eShaderType::eDomain;
		}
		else if (extension == "ps")
		{
			return eShaderType::ePixel;
		}
		else if (extension == "cs")
		{
			return eShaderType::eCompute;
		}
		else
		{
			return eShaderType::eNone;
		}
	}

	bool GLSLParser::Expand(const std::string& path, IncludeContext& context, std::string& source) const
	{
		std::ifstream file(path, std::ios::binary);
		if (!file.is_open())
		{
			LOGINFO() << "[GLSL] Failed to open the glsl file : " << path;
			return false;
		}

		std::stringstream fileStream;
		fileStream << file.rdbuf();

		const uint32_t index = static_cast<uint32_t>(context.sources.size());
		context.sources.push_back(Normalize(path));
		context.stack.push_back(index);

		const std::filesystem::path directory = std::filesystem::u8path(path).parent_path();
		const bool isRoot = (index == 0);

		std::string line;
		uint32_t lineNumber = 0;

		// getline은 마지막 줄의 개행이 없어도 한 줄로 읽고, '\r'은 줄에 그대로 남겨 원본 개행을 보존함
		while (std::getline(fileStream, line))
		{
			++lineNumber;

			size_t next = 0;

			if (StartsWithDirective(line, "version", next))
			{
				// #version은 최상위 파일 첫머리에만 있어야 하고, include한 파일에 있으면 중복 선언이 됨
				if (!isRoot)
				{
					LOGINFO() << "[GLSL] The included file must not have the #version : " << path << "(" << lineNumber << ")";
					return false;
				}
			}
			else if (StartsWithDirective(line, "include", next))
			{
				const size_t begin = line.find('"', next);
				const size_t end = (begin == std::string::npos) ? std::string::npos : line.find('"', begin + 1);
				if (begin == std::string::npos || end == std::string::npos)
				{
					LOGINFO() << "[GLSL] The #include must be written as #include \"file\" : " << path << "(" << lineNumber << ")";
					return false;
				}

				const std::string includePath = (directory / std::filesystem::u8path(line.substr(begin + 1, end - begin - 1))).lexically_normal().generic_string();
				const std::string includeKey = Normalize(includePath);

				// 순환 include : 지금 펼치는 중인 파일을 다시 include하면 끝나지 않음
				const bool isCycle = std::any_of(context.stack.begin(), context.stack.end(), [&](const uint32_t stacked)
				{
					return context.sources[stacked] == includeKey;
				});
				if (isCycle)
				{
					LOGINFO() << "[GLSL] The circular include is detected : " << includePath << ", from : " << path << "(" << lineNumber << ")";
					return false;
				}

				// 이미 펼친 파일은 건너뜀. include 줄을 빈 줄로 남겨 뒤 줄 번호가 밀리지 않게 함
				const bool isIncluded = std::any_of(context.sources.begin(), context.sources.end(), [&](const std::string& included)
				{
					return included == includeKey;
				});
				if (isIncluded)
				{
					source += "\n";
					continue;
				}

				// 포함된 파일의 줄 번호는 그 파일 기준(1부터)으로, 돌아오면 이 파일의 다음 줄 번호로 되돌림
				const uint32_t includeIndex = static_cast<uint32_t>(context.sources.size());
				source += "#line 1 " + std::to_string(includeIndex) + "\n";

				if (!Expand(includePath, context, source))
				{
					return false;
				}

				if (!source.empty() && source.back() != '\n')
				{
					source += "\n";
				}

				source += "#line " + std::to_string(lineNumber + 1) + " " + std::to_string(index) + "\n";
				continue;
			}

			source += line;
			source += "\n";
		}

		context.stack.pop_back();

		return true;
	}
}
