#ifndef __WTR_RENDERTYPES_H__
#define __WTR_RENDERTYPES_H__

#include <Container/include/StaticArray.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	enum class eRenderType : uint8_t
	{
		eNone = 0x00,
		eOpenGL = 0x01,
		eVulkan = 0x02,

		// Not Implemented
		eDirectX11 = 0x03,
		eDirectX12 = 0x04
	};

	enum class eResourceType : uint8_t
	{
		eNone		= 0x00,
		eConst		= 0x01,
		eBuffer		= 0x02,
		eTexture	= 0x03,
		eSampler	= 0x04,
		eShader		= 0x05,
		ePipeLine	= 0x06,
		eLayout		= 0x07,
		ePass		= 0x08,
		eBufferView = 0x09,
		eTextureView = 0x0A
	};

	enum class eBindingType : uint8_t
	{
		eNone		= 0x00,
		eConst		= 0x01,
		eUniform	= 0x02,
		eStorage	= 0x03,
		eSampler	= 0x04,
		eTexture	= 0x05
	};

	enum class eDataUsage : uint8_t
	{
		eNone		= 0x00,
		eStatic		= 0x01,
		eDynamic	= 0x02,
		eStream		= 0x03
	};

	enum class eDataState : uint8_t
	{
		eNone	= 0x00,
		eRead	= 0x01 << 0,
		eWrite	= 0x01 << 1,
	};

	enum class eDataStage : uint8_t
	{
		eNone			= 0x00,
		eTransfer		= 0x01 << 0,
		eVertexInput	= 0x01 << 1,
		eVertex			= 0x01 << 2,
		eDomain			= 0x01 << 3,
		eHull			= 0x01 << 4,
		eGeometry		= 0x01 << 5,
		ePixel			= 0x01 << 6,
		eCompute		= 0x01 << 7
	};

	enum class eDataType : uint8_t
	{
		eNone	= 0x00,
		eByte	= 0x01,
		eUByte	= 0x02,
		eShort	= 0x03,
		eUShort	= 0x04,
		eInt	= 0x05,
		eUInt	= 0x06,
		eFloat	= 0x07,
		eDouble	= 0x08
	};

	enum class eDataDivisor : uint8_t
	{
		eNone			= 0x00,
		ePerVertex		= 0x01,
		ePerInstance	= 0x02
	};

	enum class eVertexSemantic : uint8_t
	{
		eNone		= 0x00,
		ePosition	= 0x01,
		eNormal		= 0x02,
		eTangent	= 0x03,
		eColor		= 0x04,
		eTexCoord	= 0x05,
		eJoint		= 0x06,
		eWeight		= 0x07,
		eGeneric	= 0x08
	};

	enum class eBufferUsage : uint8_t
	{
		eNone			= 0x00,
		eTransfer		= 0x01 << 0,
		eVertex			= 0x01 << 1,
		eIndex			= 0x01 << 2,
		eUniform		= 0x01 << 3,
		eStorage		= 0x01 << 4,
		eIndirect		= 0x01 << 5
	};

	enum class eTextureUsage : uint8_t
	{
		eNone				= 0x00,
		eTransfer			= 0x01 << 0,
		eSampler			= 0x01 << 1,
		eColorAttach		= 0x01 << 2,
		eDepthStencilAttach	= 0x01 << 3,
		ePresent			= 0x01 << 4
	};

	enum class eTextureType : uint8_t
	{
		eNone						= 0x00,
		eTexture1D					= 0x01,
		eTexture2D					= 0x02,
		eTexture3D					= 0x03,
		eTextureCube				= 0x04,
		eTexture1DArray				= 0x05,
		eTexture2DArray				= 0x06,
		eTextureCubeArray			= 0x07,
		eTextureMultisample			= 0x08,
		eTextureMultisampleArray	= 0x09
	};

	enum class eTextureFormat : uint8_t
	{
		eNone				= 0x00,

		eR8_UNorm			= 0x01,
		eR8G8_UNorm			= 0x02,
		eR8G8B8_UNorm		= 0x03,
		eR8G8B8A8_UNorm		= 0x04,

		eR16_UNorm			= 0x05,
		eR16G16_UNorm		= 0x06,
		eR16G16B16_UNorm	= 0x07,
		eR16G16B16A16_UNorm	= 0x08,

		eR8G8B8A8_sRGB		= 0x09,

		eR8_UInt			= 0x0A,
		eR8G8_UInt			= 0x0B,
		eR8G8B8_UInt		= 0x0C,
		eR8G8B8A8_UInt		= 0x0D,
		
		eR16_UInt			= 0x0E,
		eR16G16_UInt		= 0x0F,
		eR16G16B16_UInt		= 0x10,
		eR16G16B16A16_UInt	= 0x11,

		eR32_UInt			= 0x12,
		eR32G32_UInt		= 0x13,
		eR32G32B32_UInt		= 0x14,
		eR32G32B32A32_UInt	= 0x15,

		eR16_Float			= 0x16,
		eR16G16_Float		= 0x17,
		eR16G16B16_Float	= 0x18,
		eR16G16B16A16_Float	= 0x19,

		eR32_Float			= 0x1A,
		eR32G32_Float		= 0x1B,
		eR32G32B32_Float	= 0x1C,
		eR32G32B32A32_Float	= 0x1D,

		eD24_S8				= 0x1E,
		eD32				= 0x1F,

		// TODO : Not yet the compressed pixel format
	};

	enum class eTextureSwizzle : uint8_t
	{
		eNone	= 0x00,
		eRed	= 0x01,
		eGreen	= 0x02,
		eBlue	= 0x03,
		eAlpha	= 0x04,
		eZero	= 0x05,
		eOne	= 0x06
	};

	enum class eSamplerFilter : uint8_t
	{
		eNone		= 0x00,
		eNearest	= 0x01,
		eLinear		= 0x02,
	};

	enum class eSamplerWrap : uint8_t
	{
		eNone			= 0x00,
		eRepeat			= 0x01,
		eMirroredRepeat	= 0x02,
		eClampToEdge	= 0x03,
		eClampToBorder	= 0x04,
	};

	enum class eSamplerBorderColor : uint8_t
	{
		eNone				= 0x00,
		eTransparentBlack	= 0x01,
		eOpaqueBlack		= 0x02,
		eOpaqueWhite		= 0x03
	};

	enum class eShaderType : uint8_t
	{
		eNone		= 0x00,
		eVertex		= 0x01,
		eGeometry	= 0x02,
		eHull		= 0x03,
		eDomain		= 0x04,
		ePixel		= 0x05,
		eCompute	= 0x06,
	};

	enum class eCullFace : uint8_t
	{
		eNone		= 0x00,
		eFront		= 0x01,
		eBack		= 0x02,
		eFront_Back	= 0x03,
	};

	enum class eFrontFace : uint8_t
	{
		eNone	= 0x00,
		eCCW	= 0x01,
		eCW		= 0x02,
	};

	enum class eDrawMode : uint8_t
	{
		eNone			= 0x00,
		ePointList		= 0x01,
		eLineList		= 0x02,
		eLineStrip		= 0x03,
		eTriangleList	= 0x04,
		eTriangleStrip	= 0x05,
		eTriangleFan	= 0x06,
	};

	enum class ePrimitiveMode : uint8_t
	{
		eNone	= 0x00,
		ePoint	= 0x01,
		eLine	= 0x02,
		eFill	= 0x03
	};

	enum class eClearBuffer : uint8_t
	{
		eNone		= 0x00,
		eColor		= 0x01 << 0,
		eDepth		= 0x01 << 1,
		eStencil	= 0x01 << 2,
	};

	enum class eCompareFunc : uint8_t
	{
		eNone			= 0x00,
		eNever			= 0x01,
		eEqual			= 0x02,
		eNotEqual		= 0x03,
		eLess			= 0x04,
		eLessEqual		= 0x05,
		eGreater		= 0x06,
		eGreaterEqual	= 0x07,
		eAlways			= 0x08,
	};

	enum class eStencilOp : uint8_t
	{
		eNone		= 0x00,
		eZero		= 0x01,
		eKeep		= 0x02,
		eReplace	= 0x03,
		eInc_Clamp	= 0x04,
		eInc_Wrap	= 0x05,
		eDec_Clamp	= 0x06,
		eDec_Wrap	= 0x07,
		eInvert		= 0x08,
	};

	enum class eBlendFunc : uint8_t
	{
		eNone					= 0x00,
		eZero					= 0x01,
		eOne					= 0x02,
		eSrc_Color				= 0x03,
		eOne_Minus_Src_Color	= 0x04,
		eDst_Color				= 0x05,
		eOne_Minus_Dst_Color	= 0x06,
		eSrc_Alpha				= 0x07,
		eOne_Minus_Src_Alpha	= 0x08,
		eDst_Alpha				= 0x09,
		eOne_Minus_Dst_Alpha	= 0x0A,
		eConst_Color			= 0x0B,
		eOne_Minus_Const_Color	= 0x0C,
		eConst_Alpha			= 0x0D,
		eOne_Minus_Const_Alpha	= 0x0E,
		eSrc_Alpha_Saturate		= 0x0F,
	};

	enum class eBlendOp : uint8_t
	{
		eNone			= 0x00,
		eAdd			= 0x01,
		eSub			= 0x02,
		eReverse_Sub	= 0x03,
		eMin			= 0x04,
		eMax			= 0x05,
	};

	enum class eAttachmentType : uint8_t
	{
		eNone = 0x00,
		eColor = 0x01 << 0,
		eDepth = 0x01 << 1,
		eStencil = 0x01 << 2
	};

	enum class eAttachmentLoadOp : uint8_t
	{
		eNone		= 0x00,
		eLoad		= 0x01,
		eClear		= 0x02,
		eDontCare	= 0x03
	};

	enum class eAttachmentStoreOp : uint8_t
	{
		eNone		= 0x00,
		eStore		= 0x01,
		eDontCare	= 0x02
	};

	enum class eResourceSlot : uint8_t
	{
		eNone			= 0,

		eBegin_Texture,

		// Texture - Material
		eAmbient		= eBegin_Texture,
		eDiffuse,
		eSpecular,
		eEmissive,
		eOpacity,
		eBump,
		eNormal,
		eRoughness,
		eMetallic,
		eAmbientOcclusion,
		eSheen,

		// Texture - GBuffer
		eGNormal,
		eGAlbedo,
		eGPhong,
		eGPBR,
		eGAlpha,
		eGDepth,
		eGShadow,
		eEnd_Texture	= eGShadow,

		eBegin_Uniform	= eEnd_Texture,
		// Uniform - Material
		eMaterial,

		// Uniform - Camera
		eCamera,

		// Uniform - Instance
		eInstanceCount,
		eRawTransform,
		eTransform,
		eIndirect,
		eVisible,
		eLocalBounding,

		// Uniform - Light
		eLight,
		eLightView,
		eLightProj,

		eEnd_Uniform
	};

	enum class eBlendMode : uint8_t
	{
		eNone			= 0x00,
		eOpaque			= 0x01,
		eTransparent	= 0x02,
		eAdditive		= 0x03,
		eMasked			= 0x04,
	};

	enum class eLightType : uint8_t
	{
		eNone			= 0x00,
		eDirectional	= 0x01,
		ePoint			= 0x02,
		eSpot			= 0x03,
	};

	enum class eRenderDirty : uint8_t
	{
		eNone		= 0x00,
		eTransform	= 0x01 << 0,
		eMesh		= 0x01 << 1,
		eMaterial	= 0x01 << 2,
		eLight		= 0x01 << 3,
		eAll		= eTransform | eMesh | eMaterial | eLight
	};

	enum class eGBufferSlot : uint8_t
	{
		eNone	= 0x00,
		eNormal,
		eAlbedo,
		ePhong,
		ePBR,
		eAlpha,
		eDepth,
	};

	enum class eRenderPass : uint8_t
	{
		eNone			= 0x00,
		eTransform		= 0x01 << 0,
		eCulling		= 0x01 << 1,
		eGeometry		= 0x01 << 2,
		eLighting		= 0x01 << 3,
		eDebug			= 0x01 << 4,
	};

	constexpr uint8_t SEMANTIC_INDEX_COUNT = 3;
	constexpr uint8_t SEMANTIC_INDEX_MAX = 16;
	constexpr uint8_t SEMANTIC_TYPE_COUNT = static_cast<uint8_t>(eVertexSemantic::eGeneric);
	constexpr wtr::StaticArray<const char*, static_cast<size_t>(SEMANTIC_TYPE_COUNT)> SEMANTIC_NAMES =
	{
		"POSITION",
		"NORMAL",
		"TANGENT",
		"COLOR",
		"TEXCOORD",
		"JOINT",
		"WEIGHT",
		"GENERIC"
	};

	inline eDataState operator|(eDataState a, eDataState b)
	{
		return static_cast<eDataState>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eDataState& operator|=(eDataState& a, eDataState b)
	{
		a = a | b;
		return a;
	}

	inline eDataState operator&(eDataState a, eDataState b)
	{
		return static_cast<eDataState>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eDataState& operator&=(eDataState& a, eDataState b)
	{
		a = a & b;
		return a;
	}

	inline eDataStage operator|(eDataStage a, eDataStage b)
	{
		return static_cast<eDataStage>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eDataStage& operator|=(eDataStage& a, eDataStage b)
	{
		a = a | b;
		return a;
	}

	inline eDataStage operator&(eDataStage a, eDataStage b)
	{
		return static_cast<eDataStage>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eDataStage& operator&=(eDataStage& a, eDataStage b)
	{
		a = a & b;
		return a;
	}

	inline eBufferUsage operator|(eBufferUsage a, eBufferUsage b)
	{
		return static_cast<eBufferUsage>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eBufferUsage& operator|=(eBufferUsage& a, eBufferUsage b)
	{
		a = a | b;
		return a;
	}

	inline eBufferUsage operator&(eBufferUsage a, eBufferUsage b)
	{
		return static_cast<eBufferUsage>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eBufferUsage& operator&=(eBufferUsage& a, eBufferUsage b)
	{
		a = a & b;
		return a;
	}

	inline eTextureUsage operator|(eTextureUsage a, eTextureUsage b)
	{
		return static_cast<eTextureUsage>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eTextureUsage& operator|=(eTextureUsage& a, eTextureUsage b)
	{
		a = a | b;
		return a;
	}

	inline eTextureUsage operator&(eTextureUsage a, eTextureUsage b)
	{
		return static_cast<eTextureUsage>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eTextureUsage& operator&=(eTextureUsage& a, eTextureUsage b)
	{
		a = a & b;
		return a;
	}

	inline eClearBuffer operator|(eClearBuffer a, eClearBuffer b)
	{
		return static_cast<eClearBuffer>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eClearBuffer& operator|=(eClearBuffer& a, eClearBuffer b)
	{
		a = a | b;
		return a;
	}

	inline eClearBuffer operator&(eClearBuffer a, eClearBuffer b)
	{
		return static_cast<eClearBuffer>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eClearBuffer& operator&=(eClearBuffer& a, eClearBuffer b)
	{
		a = a & b;
		return a;
	}

	inline eAttachmentType operator|(eAttachmentType a, eAttachmentType b)
	{
		return static_cast<eAttachmentType>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eAttachmentType& operator|=(eAttachmentType& a, eAttachmentType b)
	{
		a = a | b;
		return a;
	}

	inline eAttachmentType operator&(eAttachmentType a, eAttachmentType b)
	{
		return static_cast<eAttachmentType>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eAttachmentType& operator&=(eAttachmentType& a, eAttachmentType b)
	{
		a = a & b;
		return a;
	}

	inline eRenderDirty operator|(eRenderDirty a, eRenderDirty b)
	{
		return static_cast<eRenderDirty>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eRenderDirty& operator|=(eRenderDirty& a, eRenderDirty b)
	{
		a = a | b;
		return a;
	}

	inline eRenderDirty operator&(eRenderDirty a, eRenderDirty b)
	{
		return static_cast<eRenderDirty>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eRenderDirty& operator&=(eRenderDirty& a, eRenderDirty b)
	{
		a = a & b;
		return a;
	}

	inline eRenderPass operator|(eRenderPass a, eRenderPass b)
	{
		return static_cast<eRenderPass>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eRenderPass& operator|=(eRenderPass& a, eRenderPass b)
	{
		a = a | b;
		return a;
	}

	inline eRenderPass operator&(eRenderPass a, eRenderPass b)
	{
		return static_cast<eRenderPass>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eRenderPass& operator&=(eRenderPass& a, eRenderPass b)
	{
		a = a & b;
		return a;
	}
};

#endif // __WTR_RENDERTYPES_H__