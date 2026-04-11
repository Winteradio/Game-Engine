#ifndef __WTR_RENDERTYPES_H__
#define __WTR_RENDERTYPES_H__

#include <Container/include/DynamicArray.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <cstdint>

namespace wtr
{
	enum class eRenderType : uint8_t
	{
		eNone		= 0x00,
		eOpenGL		= 0x01,

		// Not Implemented
		eDirectX11	= 0x02,
		eDirectX12	= 0x03,
		eVulkan		= 0x04
	};

	enum class eResourceType : uint8_t
	{
		eNone		= 0x00,
		eBuffer		= 0x01,
		eTexture	= 0x02,
		eSampler	= 0x03,
		eShader		= 0x04,
		ePipeLine	= 0x05,
		eLayout		= 0x06,
	};

	enum class eBufferType : uint8_t
	{
		eNone		= 0x00,
		eVertex		= 0x01,
		eIndex		= 0x02,
		eConst		= 0x03,
		eStorage	= 0x04,
		eStaging	= 0x05
	};

	enum class eDataAccess : uint8_t
	{
		eNone		= 0x00,
		eStatic		= 0x01,
		eDynamic	= 0x02,
		eStream		= 0x03,
		eStaging	= 0x04
	};

	enum class eDataType : uint8_t
	{
		eNone	= 0x00,
		eByte	= 0x01,
		eUByte	= 0x02,
		eShort	= 0x03,
		eUShort = 0x04,
		eInt	= 0x05,
		eUInt	= 0x06,
		eFloat	= 0x07,
		eDouble = 0x08
	};

	enum class eMapAccess : uint8_t
	{
		eNone				= 0x00,
		eRead				= 0x01 << 0,
		eWrite				= 0x01 << 1,
		eInvalidateRange	= 0x01 << 2,
		eInvalidateBuffer	= 0x01 << 3,
		eFlushExplicit		= 0x01 << 4,
		eUnsynchronized		= 0x01 << 5,
	};

	enum class eVertexSemantic : uint8_t
	{
		eNone		= 0x00,
		ePosition	= 0x01,
		eNormal		= 0x02,
		eTangent	= 0x03,
		eColor		= 0x04,
		eTexCoord	= 0x05,
		eJoint 		= 0x06,
		eWeight		= 0x07,
		eGeneric	= 0x08
	};

	enum class eBindingType : uint8_t
	{
		eNone			= 0x00,
		eUniform		= 0x01,
		eUniformBuffer	= 0x02,
		eStorageBuffer 	= 0x03,
		eSampler		= 0x04,
		eTexture		= 0x05,
	};

	enum class eTextureType : uint8_t
	{
		eNone 						= 0x00,
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

	enum class ePixelFormat : uint8_t
	{
		eNone			= 0x00,
		eR8				= 0x01,
		eR8G8			= 0x02,
		eR8G8B8			= 0x03,
		eR8G8B8A8		= 0x04,
		eR8G8B8A8_SRGB	= 0x05,
		
		eD24_S8			= 0x06,
		eD32			= 0x07
	};

	enum class eTextureUsage : uint8_t
	{
		eNone			= 0x00,
		eSampled		= 0x01 << 0,
		eRenderTarget	= 0x01 << 1 ,
		eDepthStencil	= 0x01 << 2,
		eStorage		= 0x01 << 3,
		ePresent		= 0x01 << 4
	};

	enum class eFilterMode : uint8_t
	{
		eNone		= 0x00,
		eNearest	= 0x01,
		eLinear		= 0x02,
	};

	enum class eWrapMode : uint8_t
	{
		eNone			= 0x00,
		eRepeat			= 0x01,
		eMirroedRepeat	= 0x02,
		eClampToEdge	= 0x03,
		eClampToBorder	= 0x04,
	};

	enum class eShaderType : uint8_t
	{
		eNone			= 0x00,
		eVertex			= 0x01,
		eGeometry		= 0x02,
		eHull			= 0x03,
		ePixel			= 0x04,
		eCompute		= 0x05,
	};

	enum class eCullFace : uint8_t
	{
		eNone		= 0x00,
		eFront 		= 0x01,
		eBack		= 0x02,
		eFront_Back = 0x03,
	};

	enum class eFrontFace : uint8_t
	{
		eCCW	= 0x00,
		eCW		= 0x01,
	};

	enum class eDrawMode : uint8_t
	{
		eNone			= 0x00,
		ePoints			= 0x01,
		eLines			= 0x02,
		eLine_Loop		= 0x03,
		eLine_Strip		= 0x04,
		eTriangles		= 0x05,
		eTriangle_Strip	= 0x06,
		eTriangle_Fan	= 0x07,
		eQuad			= 0x08,
		eQuad_Strip		= 0x09,
		ePolygon		= 0x0A,
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
		eNever			= 0x00,
		eEqual			= 0x01,
		eNotEqual		= 0x02,
		eLess			= 0x03,
		eLessEqual		= 0x04,
		eGreater		= 0x05,
		eGreatorEqual	= 0x06,
		eAlways			= 0xFF,
	};

	enum class eStencilOp : uint8_t
	{
		eZero			= 0x00,
		eKeep			= 0x01,
		eReplace		= 0x02,
		eInc_Clamp		= 0x03,
		eInc_Wrap 		= 0x04,
		eDec_Clamp 		= 0x05,
		eDec_Wrap 		= 0x06,
		eInvert			= 0x07,
	};

	enum class eBlendFunc : uint8_t
	{
		eZero					= 0x00,
		eOne					= 0x01,
		eSrc_Color				= 0x02,
		eOne_Minus_Src_Color	= 0x03,
		eDst_Color				= 0x04,
		eOne_Minus_Dst_Color 	= 0x05,
		eSrc_Alpha				= 0x06,
		eOne_Minus_Src_Alpha	= 0x07,
		eDst_Alpha				= 0x08,
		eOne_Minus_Dst_Alpha	= 0x09,
		eConst_Color			= 0x0A,
		eOne_Minus_Const_Alpha	= 0x0B,
		eSrc_Alpha_Saturate		= 0x0C,
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

	enum class eRenderTarget : uint8_t
	{
		eNone			= 0x00,
		eColor0			= 0x01 << 0,
		eColor1			= 0x01 << 1,
		eColor2			= 0x01 << 2,
		eColor3			= 0x01 << 3,
		eDepth			= 0x01 << 4,
		eStencil		= 0x01 << 5,
	};

	enum class eBarrierFlag : uint16_t
	{
		eNone			= 0x00,
		eVertexBuffer	= 0x01 << 0,
		eIndexBuffer	= 0x01 << 1,
		eConstBuffer	= 0x01 << 2,
		eStorageBuffer	= 0x01 << 3,
		eBufferUpdate	= 0x01 << 4,

		eTextureFetch	= 0x01 << 5,
		eTextureUpdate	= 0x01 << 6,

		eImageRead		= 0x01 << 7,
		eImageWrite		= 0x01 << 8,

		eFrameBuffer	= 0x01 << 9,
	};

	enum class eTextureSlot : uint8_t
	{
		eNone				= 0x00,
		eAmbient			= 0x01,
		eDiffuse			= 0x02,
		eSpecular			= 0x03,
		eEmissive			= 0x04,
		eOpacity			= 0x05,
		eBump				= 0x06,
		eNormal				= 0x07,
		eRoughness			= 0x08,
		eMetallic			= 0x09,
		eAmbientOcclusion	= 0x10,
		eSheen				= 0x11,
	};

	enum class eVectorSlot : uint8_t
	{
		eNone			= 0x00,
		eAmbientColor	= 0x01,
		eDiffuseColor	= 0x02,
		eSpecularColor	= 0x03,
		eEmissiveColor	= 0x04,
	};

	enum class eScalarSlot : uint8_t
	{
		eNone			= 0x00,
		eShininess		= 0x01,
		eOpacity		= 0x02,
		eRefraction		= 0x03,
		eRoughness		= 0x04,
		eMetallic		= 0x05,
	};

	struct RawDataDesc
	{
		const void* pointer;
	};

	struct FormattedDataDesc : RawDataDesc
	{
		eDataType componentType;
		uint32_t numComponents;
		uint32_t count;
	};

	template<typename Desc>
	struct BufferContainer
	{
		wtr::DynamicArray<uint8_t> data;
		Desc desc;
	};

	using RawBuffer = BufferContainer<RawDataDesc>;
	using FormattedBuffer = BufferContainer<FormattedDataDesc>;

	struct VertexKey
	{
		eVertexSemantic semantic;
		uint8_t semanticIndex;

		bool operator==(const VertexKey& other) const
		{
			return semantic == other.semantic && semanticIndex == other.semanticIndex;
		}
	};

	inline eMapAccess operator|(eMapAccess a, eMapAccess b)
	{
		return static_cast<eMapAccess>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eMapAccess operator&(eMapAccess a, eMapAccess b)
	{
		return static_cast<eMapAccess>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eTextureUsage operator|(eTextureUsage a, eTextureUsage b)
	{
		return static_cast<eTextureUsage>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eTextureUsage operator&(eTextureUsage a, eTextureUsage b)
	{
		return static_cast<eTextureUsage>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eClearBuffer operator|(eClearBuffer a, eClearBuffer b)
	{
		return static_cast<eClearBuffer>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eClearBuffer operator&(eClearBuffer a, eClearBuffer b)
	{
		return static_cast<eClearBuffer>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eRenderTarget operator|(eRenderTarget a, eRenderTarget b)
	{
		return static_cast<eRenderTarget>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eRenderTarget operator&(eRenderTarget a, eRenderTarget b)
	{
		return static_cast<eRenderTarget>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	bool IsIntegerDataType(const eDataType dataType);

	size_t GetDataTypeSize(const eDataType dataType);
	size_t GetVertexLocation(const VertexKey& vertexKey);
};

namespace std
{
	template<>
	struct hash<wtr::VertexKey>
	{
		size_t operator()(const wtr::VertexKey& key) const
		{
			return static_cast<uint64_t>(key.semanticIndex) << 32 | static_cast<uint64_t>(key.semantic);
		}
	};
};

#endif // __WTR_RENDERTYPES_H__N