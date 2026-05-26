#ifndef __WTR_RENDERTYPES_H__
#define __WTR_RENDERTYPES_H__

#include <Container/include/DynamicArray.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <cstdint>
#include <string>

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
		eTarget		= 0x07,
		eConst		= 0x08,
	};

	enum class eBufferType : uint8_t
	{
		eNone		= 0x00,
		eVertex		= 0x01,
		eIndex		= 0x02,
		eConst		= 0x03,
		eStorage	= 0x04,
		eStaging	= 0x05,
		eIndirect	= 0x06
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
		eSampler		= 0x04
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
		eNone				= 0x00,
		eR8_UNorm			= 0x01,
		eR8G8_UNorm			= 0x02,
		eR8G8B8_UNorm		= 0x03,
		eR8G8B8A8_UNorm		= 0x04,
		eR16G16B16A16_UNorm	= 0x05,
		eR8G8B8A8_sRGB		= 0x06,
		
		eR16_Float			= 0x07,
		eR16G16_Float		= 0x08,
		eR16G16B16_Float	= 0x09,
		eR16G16B16A16_Float = 0x10,
		
		eR32_Float			= 0x0A,
		eR32G32_Float		= 0x0B,
		eR32G32B32_Float	= 0x0C,
		eR32G32B32A32_Float = 0x0D,

		eD24_S8				= 0x0E,
		eD32				= 0x0F,
		// TODO : Not yet the compressed pixel format
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
		eDomain			= 0x04,
		ePixel			= 0x05,
		eCompute		= 0x06,
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
		eNone	= 0x00,
		eCCW	= 0x01,
		eCW		= 0x02,
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
		eNone			= 0x00,
		eNever			= 0x01,
		eEqual			= 0x02,
		eNotEqual		= 0x03,
		eLess			= 0x04,
		eLessEqual		= 0x05,
		eGreater		= 0x06,
		eGreatorEqual	= 0x07,
		eAlways			= 0x08,
	};

	enum class eStencilOp : uint8_t
	{
		eNone			= 0x00,
		eZero			= 0x01,
		eKeep			= 0x02,
		eReplace		= 0x03,
		eInc_Clamp		= 0x04,
		eInc_Wrap 		= 0x05,
		eDec_Clamp 		= 0x06,
		eDec_Wrap 		= 0x07,
		eInvert			= 0x08,
	};

	enum class eBlendFunc : uint8_t
	{
		eNone					= 0x00,
		eZero					= 0x01,
		eOne					= 0x02,
		eSrc_Color				= 0x03,
		eOne_Minus_Src_Color	= 0x04,
		eDst_Color				= 0x05,
		eOne_Minus_Dst_Color 	= 0x06,
		eSrc_Alpha				= 0x07,
		eOne_Minus_Src_Alpha	= 0x08,
		eDst_Alpha				= 0x09,
		eOne_Minus_Dst_Alpha	= 0x0A,
		eConst_Color			= 0x0B,
		eOne_Minus_Const_Alpha	= 0x0C,
		eSrc_Alpha_Saturate		= 0x0D,
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

	enum class eResourceSlot : uint8_t
	{
		eNone = 0,

		eBegin_Texture,

		// Texture - Material
		eAmbient = eBegin_Texture,
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
		eEnd_Texture = eGDepth,

		eBegin_Uniform = eEnd_Texture,
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

		eEnd_Uniform
	};

	enum class eVectorSlot : uint8_t
	{
		eNone = 0x00,
		eBaseColor,
		eSpecularColor,
		eEmissiveColor,
	};

	enum class eScalarSlot : uint8_t
	{
		eNone = 0x00,
		eRoughness,
		eMetallic,
		eAmbientOcclusion,
		eOpacity,
		eShininess,
	};

	enum class eShadingModel : uint8_t
	{
		eNone		= 0x00,
		eLit		= 0x01,
		eUnlit		= 0x02,
		eSubsurface	= 0x03,
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

	enum class eShadowType : uint8_t
	{
		eNone			= 0x00,
		eHard			= 0x01,
		eSoft			= 0x02,
	};

	enum class eRenderDirty : uint8_t
	{
		eNone = 0x00,
		eTransform = 0x01 << 0,
		eMesh = 0x01 << 1,
		eMaterial = 0x01 << 2,
		eLight = 0x01 << 3,
		eAll = eTransform | eMesh | eMaterial | eLight
	};

	enum class eAttachment : uint8_t
	{
		eNone = 0x00,
		eColor = 0x01,
		eDepth = 0x02,
		eStencil = 0x03,
		eDepthStencil = 0x04,
	};

	enum class eGBufferSlot : uint8_t
	{
		eNone = 0x00,
		eNormal,
		eAlbedo,
		ePhong,
		ePBR,
		eAlpha,
		eDepth,
	};

	class RawData
	{
	public :
		RawData();
		virtual ~RawData() = default;

	public :
		virtual const void* GetPointer() const = 0;
		virtual const size_t GetSize() const = 0;
		virtual const size_t GetCount() const = 0;
		
	public :
		bool IsEmpty() const;
	};

	template<typename T>
	class ArrayData : public RawData
	{
	public:
		wtr::DynamicArray<T> data;

		ArrayData() = default;
		virtual ~ArrayData() = default;

	public:
		const void* GetPointer() const override
		{
			return data.Data();
		}

		const size_t GetSize() const override
		{
			return data.Size() * sizeof(T);
		}

		const size_t GetCount() const override
		{
			return data.Size();
		}
	};

	template<typename T>
	class ScalarData : public RawData
	{
	public :
		T data;

		ScalarData()
		{}

		virtual ~ScalarData()
		{
		}

	public :
		const void* GetPointer() const override
		{
			return reinterpret_cast<const void*>(&data);
		}

		const size_t GetSize() const override
		{
			return sizeof(T);
		}

		const size_t GetCount() const override
		{
			return 1;
		}
	};

	struct RawBuffer
	{
		Memory::RefPtr<RawData> bulkData;
	};

	struct FormattedBuffer : RawBuffer
	{
		eDataType componentType;
		uint32_t numComponents;
		uint32_t count;
	};

	struct TextureMipMapBuffer : RawBuffer
	{
		uint32_t channels;

		uint32_t width;
		uint32_t height;
		uint32_t depth;
		uint32_t size;
		
		uint32_t level;
	};

	struct TextureFaceBuffer
	{
		wtr::DynamicArray<TextureMipMapBuffer> mipMaps;
	};

	struct TextureBuffer
	{
		// The texture 2D : face 0 is the only face, and it contains all the mip levels
		// The texture cube : face 0-5 are the 6 faces of the cube, 
		// and each face contains all the mip levels
		// The face 0 : the positive X face
		// The face 1 : the negative X face
		// The face 2 : the positive Y face
		// The face 3 : the negative Y face
		// The face 4 : the positive Z face
		// The face 5 : the negative Z face
		// The texture 3D : face 0 is the only face, and it contains all the mip levels
		wtr::DynamicArray<TextureFaceBuffer> faces;

		eDataType componentType;
	};

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

	inline eRenderDirty operator|(eRenderDirty a, eRenderDirty b)
	{
		return static_cast<eRenderDirty>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline eRenderDirty operator&(eRenderDirty a, eRenderDirty b)
	{
		return static_cast<eRenderDirty>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline eRenderDirty operator~(eRenderDirty a)
	{
		return static_cast<eRenderDirty>(~static_cast<uint8_t>(a));
	}

	inline eRenderDirty operator^(eRenderDirty a, eRenderDirty b)
	{
		return static_cast<eRenderDirty>(static_cast<uint8_t>(a) ^ static_cast<uint8_t>(b));
	}

	inline eRenderDirty& operator|=(eRenderDirty& a, eRenderDirty b)
	{
		a = a | b;
		return a;
	}

	inline eRenderDirty& operator&=(eRenderDirty& a, eRenderDirty b)
	{
		a = a & b;
		return a;
	}

	inline eRenderDirty& operator^=(eRenderDirty& a, eRenderDirty b)
	{
		a = a ^ b;
		return a;
	}

	bool IsIntegerDataType(const eDataType dataType);

	size_t GetPixelSize(const ePixelFormat pixelFormat);
	size_t GetDataTypeSize(const eDataType dataType);
	int32_t GetVertexLocation(const VertexKey& vertexKey);
	const VertexKey GetVertexKey(const std::string& attributeName);
	const std::string GetSlotName(const eResourceSlot slot);
	const eResourceSlot GetResourceSlot(const std::string& name);
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

#endif // __WTR_RENDERTYPES_H__