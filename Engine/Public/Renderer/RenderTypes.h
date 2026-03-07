#ifndef __WTR_RENDERTYPES_H__
#define __WTR_RENDERTYPES_H__

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
		eState		= 0x05,
		ePipeLine	= 0x06,
		eView		= 0x07
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

	enum class eVertexSemantic : uint8_t
	{
		eNone		= 0x00,
		ePosition	= 0x00,
		eNormal		= 0x01,
		eTangent	= 0x02,
		eColor		= 0x03,
		eTexCoord	= 0x04,
		eJoint 		= 0x05,
		eWeight		= 0x06
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
		eInc_Clamp	= 0x03,
		eInc_Wrap 	= 0x04,
		eDec_Clamp = 0x05,
		eDec_Wrap 	= 0x06,
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
};


#endif // __WTR_RENDERTYPES_H__N