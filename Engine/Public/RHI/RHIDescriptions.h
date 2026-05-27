#ifndef __WTR_RHIDESCRIPTIONS_H__
#define __WTR_RHIDESCRIPTIONS_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>
#include <Container/include/HashMap.h>
#include <Renderer/RenderTypes.h>
#include <RHI/RHIStates.h>
#include <cstdint>

namespace wtr
{
	class RHIBuffer;
	class RHITexture;
	class RHIShader;
}

namespace wtr
{
	struct RHIVertexAttribute
	{
		uint16_t location = 0;	// vertex attribute binding location
		uint16_t offset = 0;	// byte offset in stream
		bool  normalized = 1;	// normalize fixed-point data
		bool  integer = 0;	// treat as integer input
		uint8_t  divisor = 0;	// instancing divisor
		uint8_t  numComponents = 0;	// number of components (1-4)
		eDataType  componentType = eDataType::eNone;			// enum: eDataType
		eVertexSemantic semantic = eVertexSemantic::eNone;	// bit value for the semantic (e.g. TEXCOORD)
		uint8_t  semanticIndex = 0;	// e.g. TEXCOORD0 -> 0, TEXCOORD1 -> 1
	};

	struct RHIVertexStream
	{
		Memory::RefPtr<const RHIBuffer> buffer;
		RHIVertexAttribute attribute;
	};

	struct RHITextureMipMap
	{
		Memory::RefPtr<RawData> data;
		uint32_t xOffset = 0;
		uint32_t yOffset = 0;
		uint32_t zOffset = 0;
		uint32_t level = 0;
	};

	struct RHITextureFace
	{
		wtr::DynamicArray<RHITextureMipMap> mipMaps;
	};

	struct RHIResourceBinding
	{
		int32_t location 	= -1; // resource binding location
		eBindingType type 	= eBindingType::eNone; // type of the binding resource
	};

	template<enum class eResourceType Type>
	struct RHIDesc
	{
		const eResourceType type = Type;
	};

	template<typename Desc>
	struct RHICreateDesc
	{
		Desc desc;
	};

	struct RHIConstDesc : RHIDesc<eResourceType::eConst>
	{
		eDataType dataType = eDataType::eNone;

		uint32_t dimension = 0;
		uint32_t offset = 0;

		Memory::RefPtr<const RawData> data;
	};

	struct RHIBufferDesc : RHIDesc<eResourceType::eBuffer>
	{
		eBufferType bufferType = eBufferType::eNone;
		eDataAccess	accessType = eDataAccess::eNone;
		eDataType	componentType = eDataType::eNone;

		uint32_t	numComponents = 0;
		uint32_t	count = 0;
		uint32_t	size = 0;
		uint32_t	stride = 0;

		RHIBufferDesc& operator=(const RHIBufferDesc& other)
		{
			bufferType = other.bufferType;
			accessType = other.accessType;
			componentType = other.componentType;
			numComponents = other.numComponents;
			count = other.count;
			size = other.size;
			stride = other.stride;

			return *this;
		}
	};

	struct RHIBufferCreateDesc : RHIBufferDesc
	{
		struct DataRange
		{
			uint32_t offset = 0;
			Memory::RefPtr<const RawData> data;
		};

		wtr::DynamicArray<DataRange> dataRanges;
	};

	struct RHIBufferUpdateDesc : RHIBufferCreateDesc
	{
		eMapAccess mapAccess = eMapAccess::eNone;
	};

	struct RHIVertexLayoutDesc : RHIDesc<eResourceType::eLayout>
	{
		wtr::HashMap<VertexKey, RHIVertexStream> vertexStreams;
		Memory::RefPtr<const RHIBuffer> indexBuffer;

		RHIVertexLayoutDesc& operator=(const RHIVertexLayoutDesc& other)
		{
			vertexStreams = other.vertexStreams;
			indexBuffer = other.indexBuffer;

			return *this;
		}
	};

	struct RHIVertexLayoutCreateDesc : RHIVertexLayoutDesc
	{
	};

	struct RHIVertexLayoutUpdateDesc : RHIVertexLayoutDesc
	{
	};

	struct RHITextureDesc : RHIDesc<eResourceType::eTexture>
	{
		uint32_t		width = 0;
		uint32_t		height = 0;
		uint32_t		depth = 0;
		uint32_t		face = 1;
		uint32_t		mipLevels = 1;
		uint32_t		sampleCount = 1;
		ePixelFormat	format = ePixelFormat::eNone;
		eTextureUsage 	usage = eTextureUsage::eNone;
		eTextureType	textureType = eTextureType::eNone;
		eDataType		dataType = eDataType::eNone;
		bool			generateMips = false;
		bool			compressed = false;

		RHITextureDesc& operator=(const RHITextureDesc& other)
		{
			width = other.width;
			height = other.height;
			depth = other.depth;
			face = other.face;
			mipLevels = other.mipLevels;
			sampleCount = other.sampleCount;
			format = other.format;
			usage = other.usage;
			textureType = other.textureType;
			dataType = other.dataType;
			generateMips = other.generateMips;
			compressed = other.compressed;

			return *this;
		}
	};

	struct RHITextureCreateDesc : RHITextureDesc
	{
		wtr::DynamicArray<RHITextureFace> faces;
	};

	struct RHITextureUpdateDesc : RHITextureCreateDesc
	{
	};

	struct RHISamplerDesc : RHIDesc<eResourceType::eSampler>
	{
		eFilterMode		minFilter = eFilterMode::eNone;
		eFilterMode		magFilter = eFilterMode::eNone;
		eWrapMode		wrapS = eWrapMode::eNone;
		eWrapMode		wrapT = eWrapMode::eNone;
		eWrapMode		wrapR = eWrapMode::eNone;

		RHISamplerDesc& operator=(const RHISamplerDesc& other)
		{
			minFilter = other.minFilter;
			magFilter = other.magFilter;
			wrapS = other.wrapS;
			wrapT = other.wrapT;
			wrapR = other.wrapR;

			return *this;
		}

		bool operator==(const RHISamplerDesc& other) const noexcept
		{
			if (minFilter != other.minFilter ||
				magFilter != other.magFilter ||
				wrapS != other.wrapS ||
				wrapT != other.wrapT ||
				wrapR != other.wrapR)
			{
				return false;
			}

			return true;
		}
	};

	struct RHISamplerCreateDesc : RHISamplerDesc
	{};

	struct RHIShaderDesc : RHIDesc<eResourceType::eShader>
	{
		eShaderType shaderType = eShaderType::eNone;
	};

	struct RHIShaderCreateDesc : RHIShaderDesc
	{
		Memory::RefPtr<RawData> data;
	};

	struct RHIPipeLineDesc : RHIDesc<eResourceType::ePipeLine>
	{
		Memory::RefPtr<RHIClearState> clear;
		Memory::RefPtr<RHIColorState> color;
		Memory::RefPtr<RHIDepthState> depth;
		Memory::RefPtr<RHIStencilState> stencil;
		Memory::RefPtr<RHIBlendState> blend;
		Memory::RefPtr<RHIRasterizerState> rasterizer;

		RHIPipeLineDesc& operator=(const RHIPipeLineDesc& other)
		{
			clear = other.clear;
			color = other.color;
			depth = other.depth;
			stencil = other.stencil;
			blend = other.blend;
			rasterizer = other.rasterizer;

			return *this;
		}

		bool operator==(const RHIPipeLineDesc& other) const noexcept
		{
			if (clear != other.clear ||
				color != other.color ||
				depth != other.depth ||
				stencil != other.stencil ||
				blend != other.blend ||
				rasterizer != other.rasterizer)
			{
				return false;
			}

			return true;
		}
	};

	struct RHIPipeLineCreateDesc : RHIPipeLineDesc
	{
		Memory::RefPtr<const RHIShader> vertexShader;
		Memory::RefPtr<const RHIShader> geometryShader;
		Memory::RefPtr<const RHIShader> hullShader;
		Memory::RefPtr<const RHIShader> domainShader;
		Memory::RefPtr<const RHIShader> computeShader;
		Memory::RefPtr<const RHIShader> pixelShader;

		bool operator==(const RHIPipeLineCreateDesc& other) const noexcept
		{
			if (!RHIPipeLineDesc::operator==(other))
			{
				return false;
			}

			if (vertexShader != other.vertexShader ||
				geometryShader != other.geometryShader ||
				hullShader != other.hullShader ||
				domainShader != other.domainShader ||
				computeShader != other.computeShader ||
				pixelShader != other.pixelShader)
			{
				return false;
			}

			return true;
		}
	};

	struct RHIDrawIndexDesc : RHIDesc<eResourceType::eLayout>
	{
		eDrawMode drawMode = eDrawMode::eNone;
		eDataType indexType = eDataType::eNone;

		uint32_t indexCount = 0;
		uint32_t indexOffset = 0;
		uint32_t baseVertex = 0;
		uint32_t instanceCount = 1;

		bool indirectDraw = false;
	};

	struct RHIDispatchDesc : RHIDesc<eResourceType::ePipeLine>
	{
		uint32_t groupX = 0;
		uint32_t groupY = 0;
		uint32_t groupZ = 0;
	};

	struct RHIAttachment
	{
		eAttachment type = eAttachment::eNone;

		Memory::RefPtr<const RHITexture> texture;
	};

	struct RHIColorAttachment : RHIAttachment
	{
		uint32_t slot = 0;
	};

	struct RHIDepthStencilAttachment : RHIAttachment
	{

	};

	struct RHIRenderTargetDesc : RHIDesc<eResourceType::eTarget>
	{
		wtr::DynamicArray<RHIColorAttachment> colors;
		RHIDepthStencilAttachment depthStencil;
	};

	struct RHIRenderTargetCreateDesc : RHIRenderTargetDesc
	{

	};
};


#endif // __WTR_RHIDESCRIPTIONS_H__