#include <Renderer/RenderTypes.h>

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
};