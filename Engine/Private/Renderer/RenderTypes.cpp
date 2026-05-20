#include <Renderer/RenderTypes.h>

#include <Container/include/HashMap.h>

namespace wtr
{
	RawData::RawData()
		: m_isUsed(false)
	{
	}

	bool RawData::IsEmpty() const
	{
		return (GetPointer() == nullptr) || (GetSize() == 0);
	}

	bool RawData::IsUsed() const
	{
		return m_isUsed.load(std::memory_order_acquire);
	}

	void RawData::SetUsed(const bool isUsed)
	{
		m_isUsed.store(isUsed, std::memory_order_release);
	}

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

	size_t GetPixelSize(const ePixelFormat pixelFormat)
	{
		switch (pixelFormat)
		{
		case ePixelFormat::eR8_UNorm:
			return 1;

		case ePixelFormat::eR8G8_UNorm:
		case ePixelFormat::eR16_Float:
			return 2;

		case ePixelFormat::eR8G8B8_UNorm:
			return 3;

		case ePixelFormat::eR8G8B8A8_UNorm:
		case ePixelFormat::eR8G8B8A8_sRGB:
		case ePixelFormat::eR16G16_Float:
		case ePixelFormat::eR32_Float:
		case ePixelFormat::eD24_S8:
		case ePixelFormat::eD32:
			return 4;

		case ePixelFormat::eR16G16B16_Float:
			return 6;

		case ePixelFormat::eR16G16B16A16_Float:
		case ePixelFormat::eR32G32_Float:
			return 8;

		case ePixelFormat::eR32G32B32_Float:
			return 12;

		case ePixelFormat::eR32G32B32A32_Float:
			return 16;

		default:
			return 0;
		}
	}

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

	int32_t GetVertexLocation(const VertexKey& vertexKey)
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

	const std::string GetSlotName(const eResourceSlot slot)
	{
		// Texture - Material
		if (slot == eResourceSlot::eAmbient)          return "tAmbient";
		if (slot == eResourceSlot::eDiffuse)          return "tDiffuse";
		if (slot == eResourceSlot::eSpecular)         return "tSpecular";
		if (slot == eResourceSlot::eEmissive)         return "tEmissive";
		if (slot == eResourceSlot::eOpacity)          return "tOpacity";
		if (slot == eResourceSlot::eBump)             return "tBump";
		if (slot == eResourceSlot::eNormal)           return "tNormal";
		if (slot == eResourceSlot::eRoughness)        return "tRoughness";
		if (slot == eResourceSlot::eMetallic)         return "tMetallic";
		if (slot == eResourceSlot::eAmbientOcclusion) return "tAmbientOcclusion";
		if (slot == eResourceSlot::eSheen)            return "tSheen";

		// Texture - GBuffer
		if (slot == eResourceSlot::eGPosition)        return "tGPosition";
		if (slot == eResourceSlot::eGNormal)          return "tGNormal";
		if (slot == eResourceSlot::eGAlbedo)          return "tGAlbedo";
		if (slot == eResourceSlot::eGDepth)           return "tGDepth";

		// Uniform - Material
		if (slot == eResourceSlot::eVector)           return "uVector";
		if (slot == eResourceSlot::eScalar)           return "uScalar";

		// Uniform - Camera
		if (slot == eResourceSlot::eCamera)           return "uCamera";

		// Uniform - Instance
		if (slot == eResourceSlot::eTransform)        return "uTransform";
		if (slot == eResourceSlot::eIndirect)         return "uIndirect";
		if (slot == eResourceSlot::eVisible)          return "uVisible";
		if (slot == eResourceSlot::eLocalBounding)    return "uLocalBounding";

		// Uniform - Light
		if (slot == eResourceSlot::eLight)           return "uLight";

		return "";
	}

	const eResourceSlot GetResourceSlot(const std::string& name)
	{
		// Texture - Material
		if (name == "tAmbient")          return eResourceSlot::eAmbient;
		if (name == "tDiffuse")          return eResourceSlot::eDiffuse;
		if (name == "tSpecular")         return eResourceSlot::eSpecular;
		if (name == "tEmissive")         return eResourceSlot::eEmissive;
		if (name == "tOpacity")          return eResourceSlot::eOpacity;
		if (name == "tBump")             return eResourceSlot::eBump;
		if (name == "tNormal")           return eResourceSlot::eNormal;
		if (name == "tRoughness")        return eResourceSlot::eRoughness;
		if (name == "tMetallic")         return eResourceSlot::eMetallic;
		if (name == "tAmbientOcclusion") return eResourceSlot::eAmbientOcclusion;
		if (name == "tSheen")            return eResourceSlot::eSheen;

		// Texture - GBuffer
		if (name == "tGPosition")        return eResourceSlot::eGPosition;
		if (name == "tGNormal")          return eResourceSlot::eGNormal;
		if (name == "tGAlbedo")          return eResourceSlot::eGAlbedo;
		if (name == "tGDepth")           return eResourceSlot::eGDepth;

		// Uniform - Material
		if (name == "uVector")           return eResourceSlot::eVector;
		if (name == "uScalar")           return eResourceSlot::eScalar;

		// Uniform - Camera
		if (name == "uCamera")           return eResourceSlot::eCamera;

		// Uniform - Instance
		if (name == "uTransform")        return eResourceSlot::eTransform;
		if (name == "uIndirect")         return eResourceSlot::eIndirect;
		if (name == "uVisible")          return eResourceSlot::eVisible;
		if (name == "uLocalBounding")    return eResourceSlot::eLocalBounding;

		// Uniform - Light
		if (name == "uLight")			 return eResourceSlot::eLight;

		return eResourceSlot::eNone;
	}
};