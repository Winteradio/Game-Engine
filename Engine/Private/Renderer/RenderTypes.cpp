#include <Renderer/RenderTypes.h>

#include <Container/include/HashMap.h>
#include <algorithm>

namespace wtr
{
	RawData::RawData()
	{
	}

	bool RawData::IsEmpty() const
	{
		return (GetPointer() == nullptr) || (GetSize() == 0);
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
		case ePixelFormat::eR8_UInt:
			return 1;

		case ePixelFormat::eR8G8_UNorm:
		case ePixelFormat::eR8G8_UInt:
		case ePixelFormat::eR16_UNorm:
		case ePixelFormat::eR16_UInt:
		case ePixelFormat::eR16_Float:
			return 2;

		case ePixelFormat::eR8G8B8_UNorm:
		case ePixelFormat::eR8G8B8_UInt:
			return 3;

		case ePixelFormat::eR8G8B8A8_UNorm:
		case ePixelFormat::eR8G8B8A8_sRGB:
		case ePixelFormat::eR8G8B8A8_UInt:
		case ePixelFormat::eR16G16_UNorm:
		case ePixelFormat::eR16G16_UInt:
		case ePixelFormat::eR16G16_Float:
		case ePixelFormat::eR32_UInt:
		case ePixelFormat::eR32_Float:
		case ePixelFormat::eD24_S8:
		case ePixelFormat::eD32:
			return 4;

		case ePixelFormat::eR16G16B16_UNorm:
		case ePixelFormat::eR16G16B16_UInt:
		case ePixelFormat::eR16G16B16_Float:
			return 6;

		case ePixelFormat::eR16G16B16A16_UNorm:
		case ePixelFormat::eR16G16B16A16_UInt:
		case ePixelFormat::eR16G16B16A16_Float:
		case ePixelFormat::eR32G32_UInt:
		case ePixelFormat::eR32G32_Float:
			return 8;

		case ePixelFormat::eR32G32B32_UInt:
		case ePixelFormat::eR32G32B32_Float:
			return 12;

		case ePixelFormat::eR32G32B32A32_UInt:
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

	const VertexKey& GetVertexKey(const std::string& attributeName)
	{
		static const VertexKey emptyKey = { eVertexSemantic::eNone, 0xFF };

		if (attributeName.empty())
		{
			return emptyKey;
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
			static VertexKey resultKey = { eVertexSemantic::eNone, 0 };
			resultKey = { itr->second, semanticIndex };
			return resultKey;
		}

		return emptyKey;
	}

	const std::string& GetSlotName(const eResourceSlot slot)
	{
		// Texture - Material
		if (slot == eResourceSlot::eAmbient)			{ static const std::string s = "tAmbient"; return s; }
		if (slot == eResourceSlot::eDiffuse)			{ static const std::string s = "tDiffuse"; return s; }
		if (slot == eResourceSlot::eSpecular)			{ static const std::string s = "tSpecular"; return s; }
		if (slot == eResourceSlot::eEmissive)			{ static const std::string s = "tEmissive"; return s; }
		if (slot == eResourceSlot::eOpacity)			{ static const std::string s = "tOpacity"; return s; }
		if (slot == eResourceSlot::eBump)				{ static const std::string s = "tBump"; return s; }
		if (slot == eResourceSlot::eNormal)				{ static const std::string s = "tNormal"; return s; }
		if (slot == eResourceSlot::eRoughness)			{ static const std::string s = "tRoughness"; return s; }
		if (slot == eResourceSlot::eMetallic)			{ static const std::string s = "tMetallic"; return s; }
		if (slot == eResourceSlot::eAmbientOcclusion)	{ static const std::string s = "tAmbientOcclusion"; return s; }
		if (slot == eResourceSlot::eSheen)				{ static const std::string s = "tSheen"; return s; }

		// Texture - GBuffer
		if (slot == eResourceSlot::eGNormal)			{ static const std::string s = "tGNormal"; return s; }
		if (slot == eResourceSlot::eGAlbedo)			{ static const std::string s = "tGAlbedo"; return s; }
		if (slot == eResourceSlot::eGPhong)				{ static const std::string s = "tGPhong"; return s; }
		if (slot == eResourceSlot::eGPBR)				{ static const std::string s = "tGPBR"; return s; }
		if (slot == eResourceSlot::eGAlpha)				{ static const std::string s = "tGAlpha"; return s; }
		if (slot == eResourceSlot::eGDepth)				{ static const std::string s = "tGDepth"; return s; }
		if (slot == eResourceSlot::eGShadow)			{ static const std::string s = "tGShadow"; return s; }

		// Uniform - Material
		if (slot == eResourceSlot::eMaterial)				{ static const std::string s = "uMaterial"; return s; }

		// Uniform - Camera
		if (slot == eResourceSlot::eCamera)				{ static const std::string s = "uCamera"; return s; }

		// Uniform - Instance
		if (slot == eResourceSlot::eInstanceCount)		{ static const std::string s = "uInstanceCount"; return s; }
		if (slot == eResourceSlot::eRawTransform)		{ static const std::string s = "uRawTransform"; return s; }
		if (slot == eResourceSlot::eTransform)			{ static const std::string s = "uTransform"; return s; }
		if (slot == eResourceSlot::eIndirect)			{ static const std::string s = "uIndirect"; return s; }
		if (slot == eResourceSlot::eVisible)			{ static const std::string s = "uVisible"; return s; }
		if (slot == eResourceSlot::eLocalBounding)		{ static const std::string s = "uLocalBounding"; return s; }

		// Uniform - Light
		if (slot == eResourceSlot::eLight)				{ static const std::string s = "uLight"; return s; }
		if (slot == eResourceSlot::eLightView)			{ static const std::string s = "uLightView"; return s; }
		if (slot == eResourceSlot::eLightProj)			{ static const std::string s = "uLightProj"; return s; }

		static const std::string emptyString = "";
		return emptyString;
	}

	eResourceSlot GetResourceSlot(const std::string& name)
	{
		// Texture - Material
		if (name == "tAmbient")				return eResourceSlot::eAmbient;
		if (name == "tDiffuse")				return eResourceSlot::eDiffuse;
		if (name == "tSpecular")			return eResourceSlot::eSpecular;
		if (name == "tEmissive")			return eResourceSlot::eEmissive;
		if (name == "tOpacity")				return eResourceSlot::eOpacity;
		if (name == "tBump")				return eResourceSlot::eBump;
		if (name == "tNormal")				return eResourceSlot::eNormal;
		if (name == "tRoughness")			return eResourceSlot::eRoughness;
		if (name == "tMetallic")			return eResourceSlot::eMetallic;
		if (name == "tAmbientOcclusion")	return eResourceSlot::eAmbientOcclusion;
		if (name == "tSheen")				return eResourceSlot::eSheen;

		// Texture - GBuffer
		if (name == "tGNormal")				return eResourceSlot::eGNormal;
		if (name == "tGAlbedo")				return eResourceSlot::eGAlbedo;
		if (name == "tGPhong")				return eResourceSlot::eGPhong;
		if (name == "tGPBR")				return eResourceSlot::eGPBR;
		if (name == "tGAlpha")				return eResourceSlot::eGAlpha;
		if (name == "tGDepth")				return eResourceSlot::eGDepth;
		if (name == "tGShadow")				return eResourceSlot::eGShadow;

		// Uniform - Material
		if (name == "uMaterial")			return eResourceSlot::eMaterial;

		// Uniform - Camera
		if (name == "uCamera")				return eResourceSlot::eCamera;

		// Uniform - Instance
		if (name == "uInstanceCount")		return eResourceSlot::eInstanceCount;
		if (name == "uRawTransform")		return eResourceSlot::eRawTransform;
		if (name == "uTransform")			return eResourceSlot::eTransform;
		if (name == "uIndirect")			return eResourceSlot::eIndirect;
		if (name == "uVisible")				return eResourceSlot::eVisible;
		if (name == "uLocalBounding")		return eResourceSlot::eLocalBounding;

		// Uniform - Light
		if (name == "uLight")				return eResourceSlot::eLight;
		if (name == "uLightView")			return eResourceSlot::eLightView;
		if (name == "uLightProj")			return eResourceSlot::eLightProj;

		return eResourceSlot::eNone;
	}
};