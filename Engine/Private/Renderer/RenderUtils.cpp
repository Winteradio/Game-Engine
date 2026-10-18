#include <Renderer/RenderUtils.h>

#include <Renderer/RenderData.h>
#include <Container/include/HashMap.h>
#include <algorithm>
#include <cctype>

namespace wtr
{
	namespace RenderUtils
	{
		uint32_t GetComponentSize(const eDataType dataType)
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

		uint32_t GetPixelSize(const ePixelFormat pixelFormat)
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

		uint32_t GetPixelDimension(const ePixelFormat pixelFormat)
		{
			switch (pixelFormat)
			{
			case ePixelFormat::eR8_UNorm:
			case ePixelFormat::eR8_UInt:
			case ePixelFormat::eR16_UNorm:
			case ePixelFormat::eR16_UInt:
			case ePixelFormat::eR16_Float:
			case ePixelFormat::eR32_UInt:
			case ePixelFormat::eR32_Float:
			case ePixelFormat::eD32:
				return 1;

			case ePixelFormat::eR8G8_UNorm:
			case ePixelFormat::eR8G8_UInt:
			case ePixelFormat::eR16G16_UNorm:
			case ePixelFormat::eR16G16_UInt:
			case ePixelFormat::eR16G16_Float:
			case ePixelFormat::eR32G32_UInt:
			case ePixelFormat::eR32G32_Float:
			case ePixelFormat::eD24_S8:
				return 2;

			case ePixelFormat::eR8G8B8_UNorm:
			case ePixelFormat::eR8G8B8_UInt:
			case ePixelFormat::eR16G16B16_UNorm:
			case ePixelFormat::eR16G16B16_UInt:
			case ePixelFormat::eR16G16B16_Float:
			case ePixelFormat::eR32G32B32_UInt:
			case ePixelFormat::eR32G32B32_Float:
				return 3;

			case ePixelFormat::eR8G8B8A8_UNorm:
			case ePixelFormat::eR8G8B8A8_sRGB:
			case ePixelFormat::eR8G8B8A8_UInt:
			case ePixelFormat::eR16G16B16A16_UNorm:
			case ePixelFormat::eR16G16B16A16_UInt:
			case ePixelFormat::eR16G16B16A16_Float:
			case ePixelFormat::eR32G32B32A32_UInt:
			case ePixelFormat::eR32G32B32A32_Float:
				return 4;

			default:
				return 0;
			}
		}

		int32_t GetVertexLocation(const VertexKey& vertexKey)
		{
			if (vertexKey.semanticIndex >= SEMANTIC_INDEX_COUNT)
			{
				return -1;
			}

			constexpr eVertexSemantic baseSemantic = eVertexSemantic::ePosition;
			constexpr eVertexSemantic dupliSemantic = eVertexSemantic::eColor;
			constexpr eVertexSemantic maxSemantic = eVertexSemantic::eGeneric;

			if (vertexKey.semantic >= baseSemantic 
				&& vertexKey.semantic < dupliSemantic)
			{
				// The base semantic
				// Position : 0
				// Normal : 1
                // Tangent : 2
                const int32_t base = static_cast<int32_t>(vertexKey.semantic) - static_cast<int32_t>(baseSemantic);

                return base;
			}
			else if (vertexKey.semantic >= dupliSemantic && 
				vertexKey.semantic < maxSemantic)
			{
				// The duplicated semantic
				// Color : 3 ~ 5
				// TexCoord : 6 ~ 8
				// Joint : 9 ~ 11
				// Weight : 12 ~ 14
                const int32_t base = static_cast<int32_t>(dupliSemantic) - static_cast<int32_t>(baseSemantic);
                const int32_t offset = static_cast<int32_t>(vertexKey.semanticIndex) + 
                    (static_cast<int32_t>(vertexKey.semantic) - static_cast<int32_t>(dupliSemantic)) * SEMANTIC_INDEX_COUNT;

				return base + offset;
			}
			else if (vertexKey.semantic >= maxSemantic)
			{
                // The generic semantic
                // Generic : 15
				return SEMANTIC_INDEX_MAX - 1;
			}
            else
            {
                return -1;
            }
		};

		VertexKey GetVertexKey(const std::string& attributeName)
		{
			if (attributeName.empty())
			{
				return {};
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
			if (itr == SEMANTIC_MAP.end())
			{
				return {};
			}
			else
			{
				VertexKey resultKey = { eVertexSemantic::eNone, 0 };
				resultKey = { itr->second, semanticIndex };
				return resultKey;
			}
		}

		const std::string& GetSlotName(const eResourceSlot slot)
		{
			// Texture - Material
			if (slot == eResourceSlot::eAmbient) { static const std::string s = "tAmbient"; return s; }
			if (slot == eResourceSlot::eDiffuse) { static const std::string s = "tDiffuse"; return s; }
			if (slot == eResourceSlot::eSpecular) { static const std::string s = "tSpecular"; return s; }
			if (slot == eResourceSlot::eEmissive) { static const std::string s = "tEmissive"; return s; }
			if (slot == eResourceSlot::eOpacity) { static const std::string s = "tOpacity"; return s; }
			if (slot == eResourceSlot::eBump) { static const std::string s = "tBump"; return s; }
			if (slot == eResourceSlot::eNormal) { static const std::string s = "tNormal"; return s; }
			if (slot == eResourceSlot::eRoughness) { static const std::string s = "tRoughness"; return s; }
			if (slot == eResourceSlot::eMetallic) { static const std::string s = "tMetallic"; return s; }
			if (slot == eResourceSlot::eAmbientOcclusion) { static const std::string s = "tAmbientOcclusion"; return s; }
			if (slot == eResourceSlot::eSheen) { static const std::string s = "tSheen"; return s; }

			// Texture - GBuffer
			if (slot == eResourceSlot::eGNormal) { static const std::string s = "tGNormal"; return s; }
			if (slot == eResourceSlot::eGAlbedo) { static const std::string s = "tGAlbedo"; return s; }
			if (slot == eResourceSlot::eGPhong) { static const std::string s = "tGPhong"; return s; }
			if (slot == eResourceSlot::eGPBR) { static const std::string s = "tGPBR"; return s; }
			if (slot == eResourceSlot::eGAlpha) { static const std::string s = "tGAlpha"; return s; }
			if (slot == eResourceSlot::eGDepth) { static const std::string s = "tGDepth"; return s; }
			if (slot == eResourceSlot::eGShadow) { static const std::string s = "tGShadow"; return s; }

			// Uniform - Material
			if (slot == eResourceSlot::eMaterial) { static const std::string s = "uMaterial"; return s; }

			// Uniform - Camera
			if (slot == eResourceSlot::eCamera) { static const std::string s = "uCamera"; return s; }

			// Uniform - Instance
			if (slot == eResourceSlot::eInstanceCount) { static const std::string s = "uInstanceCount"; return s; }
			if (slot == eResourceSlot::eRawTransform) { static const std::string s = "uRawTransform"; return s; }
			if (slot == eResourceSlot::eTransform) { static const std::string s = "uTransform"; return s; }
			if (slot == eResourceSlot::eIndirect) { static const std::string s = "uIndirect"; return s; }
			if (slot == eResourceSlot::eVisible) { static const std::string s = "uVisible"; return s; }
			if (slot == eResourceSlot::eLocalBounding) { static const std::string s = "uLocalBounding"; return s; }

			// Uniform - Light
			if (slot == eResourceSlot::eLight) { static const std::string s = "uLight"; return s; }
			if (slot == eResourceSlot::eLightView) { static const std::string s = "uLightView"; return s; }
			if (slot == eResourceSlot::eLightProj) { static const std::string s = "uLightProj"; return s; }

			static const std::string emptyString = "";
			return emptyString;
		}

		uint32_t GetResourceBinding(const std::string& name)
		{
			// Vk는 한 파이프라인(VS + PS ...) 안에서 binding이 겹치면 안 되므로, 종류와 상관없이 이름마다 번호를 하나씩 고정
			static const wtr::HashMap<std::string, uint32_t> BINDING_MAP =
			{
				// Buffer
				{ "uCamera", 0 },
				{ "uTransform", 1 },
				{ "uMaterial", 2 },
				{ "uLight", 3 },
				{ "uRawTransform", 4 },

				// Texture - Material
				{ "tDiffuse", 5 },

				// Texture - GBuffer
				{ "tGDepth", 6 },
				{ "tGNormal", 7 },
				{ "tGAlbedo", 8 },
				{ "tGPhong", 9 },
				{ "tGShadow", 10 },
				{ "tGParam", 11 },
			};

			// Texture와 Sampler를 나눠 선언(Macro.hlsli의 TEXTURE2D + SAMPLER)하면 SamplerState는 "<텍스처 이름>Sampler"로 옴
			// 표에는 텍스처 이름만 두고, Sampler는 그 텍스처 자리보다 SAMPLER_BINDING_OFFSET만큼 뒤에 고정해 텍스처와 1:1로 묶는다
			constexpr uint32_t SAMPLER_BINDING_OFFSET = 100;
			static const std::string SAMPLER_SUFFIX = "Sampler";

			const auto itr = BINDING_MAP.Find(name);
			if (itr != BINDING_MAP.End())
			{
				return itr->second;
			}

			if (name.size() > SAMPLER_SUFFIX.size() &&
				name.compare(name.size() - SAMPLER_SUFFIX.size(), SAMPLER_SUFFIX.size(), SAMPLER_SUFFIX) == 0)
			{
				const std::string textureName = name.substr(0, name.size() - SAMPLER_SUFFIX.size());
				const auto textureItr = BINDING_MAP.Find(textureName);
				if (textureItr != BINDING_MAP.End())
				{
					return textureItr->second + SAMPLER_BINDING_OFFSET;
				}
			}

			return UINT32_MAX;
		}
	}
};
