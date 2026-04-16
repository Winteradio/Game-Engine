#include <Renderer/RenderTypes.h>

#include <Container/include/HashMap.h>

namespace wtr
{
	bool IsIntegerDataType(const eDataType dataType)
	{
		switch (dataType)
		{
		case eDataType::eByte:
		case eDataType::eUByte:
		case eDataType::eShort:
		case eDataType::eUShort:
		case eDataType::eInt:
		case eDataType::eUInt:
			return true;
		default:
			return false;
		};
	};

	size_t GetDataTypeSize(const eDataType dataType)
	{
		switch (dataType)
		{
		case eDataType::eByte:
		case eDataType::eUByte:
			return 1;

		case eDataType::eShort:
		case eDataType::eUShort:
			return 2;

		case eDataType::eInt:
		case eDataType::eUInt:
		case eDataType::eFloat:
			return 4;

		case eDataType::eDouble:
			return 8;

		default:
			return 0;
		};
	};

	size_t GetVertexLocation(const VertexKey& vertexKey)
	{
		if (vertexKey.semanticIndex > 2)
		{
			return -1;
		}

		switch (vertexKey.semantic)
		{
		case eVertexSemantic::ePosition:
			return 0;

		case eVertexSemantic::eNormal:
			return 1;

		case eVertexSemantic::eTangent:
			return 2;

		case eVertexSemantic::eColor:
			return 3 + vertexKey.semanticIndex;

		case eVertexSemantic::eTexCoord:
			return 5 + vertexKey.semanticIndex;

		case eVertexSemantic::eJoint:
			return 7 + vertexKey.semanticIndex;

		case eVertexSemantic::eWeight:
			return 9 + vertexKey.semanticIndex;

		case eVertexSemantic::eGeneric:
			return 11 + vertexKey.semanticIndex;
		default:
			return -1;
		};
	};

	const VertexKey GetVertexKey(const std::string& attributeName)
	{
		if (attributeName.empty())
		{
			return { eVertexSemantic::eNone, 0xFF };
		}

		// Change the upper case of the attribute name
		std::string upper = attributeName;
		std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

		// Explicit the last number of the attribute name
		size_t pos = upper.size();
		while (pos > 0 && std::isdigit(upper[pos - 1]))
		{
			--pos;
		}

		const uint8_t semanticIndex = (pos < upper.size()) ? static_cast<uint8_t>(std::stoi(upper.substr(pos))) : 0x00;

		static const wtr::HashMap<std::string, eVertexSemantic> SEMANTIC_MAP = 
		{
			{ "POSITION", eVertexSemantic::ePosition },
			{ "NORMAL", eVertexSemantic::eNormal },
			{ "TANGENT", eVertexSemantic::eTangent },
			{ "COLOR", eVertexSemantic::eColor },
			{ "TEXCOORD", eVertexSemantic::eTexCoord },
			{ "JOINT", eVertexSemantic::eJoint },
			{ "WEIGHT", eVertexSemantic::eWeight },
			{ "GENERIC", eVertexSemantic::eGeneric }
		};

		const auto itr = SEMANTIC_MAP.Find(upper.substr(0, pos));
		if (itr != SEMANTIC_MAP.end())
		{
			return { itr->second, semanticIndex };
		}

		return { eVertexSemantic::eNone, 0xFF };
	}
};