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
		uint32_t location			= 0;
		uint32_t offset				= 0;
		uint8_t numComponents		= 0;
		uint8_t semanticIndex		= 0;
		eVertexSemantic semantic	= eVertexSemantic::eNone;
		eDataType componentType		= eDataType::eNone;
		bool normalized				= false;
	};

	struct RHIVertexBinding
	{
		uint32_t stride			= 0;
		eDataDivisor divisor	= eDataDivisor::eNone;
	};

	struct RHIVertexStream
	{
		RHIVertexAttribute attribute;
		RHIVertexBinding binding;

		uint32_t bindingIndex = 0;
	};

	template<uint32_t N>
	struct RHIDataRange
	{
		Memory::RefPtr<const RawData> data;

		wtr::StaticArray<uint32_t, N> offset = { 0 };

		static constexpr uint32_t dimension = N;
	};

	struct RHITexturerMipMap : RHIDataRange<1>
	{
		uint32_t level = 0;
	};

	struct RHITextureFace
	{
		wtr::DynamicArray<RHITexturerMipMap> mipMaps;
		uint32_t face = 0;
	};

	struct RHIShaderVariable
	{
		int32_t location	= -1;				// attribute location
		uint32_t row		= 1;				// number of rows in the variable (for matrices)
		uint32_t column		= 1;				// number of columns in the variable (for matrices)
		eDataType type		= eDataType::eNone; // enum: eDataType
	};

	struct RHIShaderBinding
	{
		int32_t binding 	= -1;					// resource binding location
		eBindingType type 	= eBindingType::eNone;	// type of the binding resource
	};

	using RHIShaderVariables = wtr::HashMap<std::string, RHIShaderVariable>;
	using RHIShaderBindings = wtr::HashMap<std::string, RHIShaderBinding>;

	template<enum class eResourceType Type>
	struct RHIDesc
	{
		const eResourceType type = Type;
	};

	struct RHIConstDesc : RHIDesc<eResourceType::eConst>
	{
		eDataType dataType	= eDataType::eNone;

		uint32_t row		= 1;
		uint32_t column		= 1;
		uint32_t offset		= 0;

		bool transpose		= false;

		Memory::RefPtr<const RawData> data;
	};

	struct RHIBufferDesc : RHIDesc<eResourceType::eBuffer>
	{
		eBufferType	bufferType			= eBufferType::eNone;
		eBufferTransfer bufferTransfer	= eBufferTransfer::eNone;
		eDataAccess	accessType			= eDataAccess::eNone;
		eDataType	componentType		= eDataType::eNone;

		uint32_t	numComponents		= 0;
		uint32_t	stride				= 0;
		uint32_t	size				= 0;

		RHIBufferDesc& operator=(const RHIBufferDesc& other)
		{
			bufferType = other.bufferType;
			bufferTransfer = other.bufferTransfer;
			accessType = other.accessType;
			componentType = other.componentType;
			numComponents = other.numComponents;
			stride = other.stride;
			size = other.size;

			return *this;
		}
	};

	struct RHIBufferCreateDesc : RHIBufferDesc
	{
		using DataRange = RHIDataRange<1>;

		wtr::DynamicArray<DataRange> dataRanges;

		RHIBufferCreateDesc() = default;
		RHIBufferCreateDesc(const RHIBufferDesc& desc)
			: RHIBufferDesc(desc)
		{}

		RHIBufferCreateDesc& operator=(const RHIBufferDesc& desc)
		{
			if (this != &desc)
			{
				RHIBufferDesc::operator=(desc);
			}

			return *this;
		}
	};

	struct RHIBufferUpdateDesc : RHIBufferCreateDesc
	{
		eMapAccess mapAccess = eMapAccess::eNone;

		RHIBufferUpdateDesc() = default;
		RHIBufferUpdateDesc(const RHIBufferDesc& desc)
			: RHIBufferCreateDesc(desc)
		{}

		RHIBufferUpdateDesc(const RHIBufferCreateDesc& desc)
			: RHIBufferCreateDesc(desc)
		{}

		RHIBufferUpdateDesc& operator=(const RHIBufferUpdateDesc& desc)
		{
			if (this != &desc)
			{
				RHIBufferCreateDesc::operator=(desc);
				mapAccess = desc.mapAccess;
			}
			return *this;
		}

		RHIBufferUpdateDesc& operator=(const RHIBufferCreateDesc& desc)
		{
			if (this != &desc)
			{
				RHIBufferCreateDesc::operator=(desc);
			}

			return *this;
		}
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

		RHITextureCreateDesc() = default;
		RHITextureCreateDesc(const RHITextureDesc& desc)
			: RHITextureDesc(desc)
		{}

		RHITextureCreateDesc& operator=(const RHITextureDesc& desc)
		{
			if (this != &desc)
			{
				RHITextureDesc::operator=(desc);
			}

			return *this;
		}
	};

	struct RHITextureUpdateDesc : RHITextureCreateDesc
	{
		RHITextureUpdateDesc() = default;
		RHITextureUpdateDesc(const RHITextureDesc& desc)
			: RHITextureCreateDesc(desc)
		{}

		RHITextureUpdateDesc& operator=(const RHITextureDesc& desc)
		{
			if (this != &desc)
			{
				RHITextureDesc::operator=(desc);
			}

			return *this;
		}

		RHITextureUpdateDesc& operator=(const RHITextureCreateDesc& desc)
		{
			if (this != &desc)
			{
				RHITextureCreateDesc::operator=(desc);
			}

			return *this;
		}
	};

	struct RHIVertexLayoutDesc : RHIDesc<eResourceType::eLayout>
	{
		wtr::HashMap<VertexKey, RHIVertexStream> streams;

		RHIVertexLayoutDesc& operator=(const RHIVertexLayoutDesc& other)
		{
			streams = other.streams;

			return *this;
		}
	};

	struct RHIVertexLayoutCreateDesc : RHIVertexLayoutDesc
	{
		RHIVertexLayoutCreateDesc& operator=(const RHIVertexLayoutDesc& desc)
		{
			if (this != &desc)
			{
				RHIVertexLayoutDesc::operator=(desc);
			}

			return *this;
		}
	};

	struct RHIVertexLayoutUpdateDesc : RHIVertexLayoutDesc
	{
		RHIVertexLayoutUpdateDesc& operator=(const RHIVertexLayoutDesc& desc)
		{
			if (this != &desc)
			{
				RHIVertexLayoutDesc::operator=(desc);
			}

			return *this;
		}
	};

	struct RHISamplerDesc : RHIDesc<eResourceType::eSampler>
	{
		eFilterMode			minFilter = eFilterMode::eNone;
		eFilterMode			magFilter = eFilterMode::eNone;
		eWrapMode			wrapS = eWrapMode::eNone;
		eWrapMode			wrapT = eWrapMode::eNone;
		eWrapMode			wrapR = eWrapMode::eNone;
		eWrapBorderColor	wrapBorderColor = eWrapBorderColor::eNone;

		RHISamplerDesc& operator=(const RHISamplerDesc& other)
		{
			minFilter = other.minFilter;
			magFilter = other.magFilter;
			wrapS = other.wrapS;
			wrapT = other.wrapT;
			wrapR = other.wrapR;
			wrapBorderColor = other.wrapBorderColor;

			return *this;
		}

		bool operator==(const RHISamplerDesc& other) const noexcept
		{
			if (minFilter != other.minFilter ||
				magFilter != other.magFilter ||
				wrapS != other.wrapS ||
				wrapT != other.wrapT ||
				wrapR != other.wrapR ||
				wrapBorderColor != other.wrapBorderColor)
			{
				return false;
			}

			return true;
		}
	};

	struct RHISamplerCreateDesc : RHISamplerDesc
	{
		RHISamplerCreateDesc& operator=(const RHISamplerDesc& desc)
		{
			if (this != &desc)
			{
				RHISamplerDesc::operator=(desc);
			}

			return *this;
		}
	};

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
		RHIColorState color;
		RHIDepthState depth;
		RHIStencilState stencil;
		RHIBlendState blend;
		RHIRasterizerState rasterizer;

		Memory::RefPtr<const RHIShader> vertexShader;
		Memory::RefPtr<const RHIShader> geometryShader;
		Memory::RefPtr<const RHIShader> hullShader;
		Memory::RefPtr<const RHIShader> domainShader;
		Memory::RefPtr<const RHIShader> pixelShader;
		Memory::RefPtr<const RHIShader> computeShader;

		RHIPipeLineDesc& operator=(const RHIPipeLineDesc& other)
		{
			color = other.color;
			depth = other.depth;
			stencil = other.stencil;
			blend = other.blend;
			rasterizer = other.rasterizer;

			vertexShader = other.vertexShader;
			geometryShader = other.geometryShader;
			hullShader = other.hullShader;
			domainShader = other.domainShader;
			pixelShader = other.pixelShader;
			computeShader = other.computeShader;

			return *this;
		}

		bool operator==(const RHIPipeLineDesc& other) const noexcept
		{
			if (color != other.color ||
				depth != other.depth ||
				stencil != other.stencil ||
				blend != other.blend ||
				rasterizer != other.rasterizer ||
				vertexShader != other.vertexShader ||
				geometryShader != other.geometryShader ||
				hullShader != other.hullShader ||
				domainShader != other.domainShader ||
				pixelShader != other.pixelShader ||
				computeShader != other.computeShader)
			{
				return false;
			}

			return true;
		}
	};

	struct RHIPipeLineCreateDesc : RHIPipeLineDesc
	{
		Memory::RefPtr<const RHIVertexLayout> layout;
		Memory::RefPtr<const RHIRenderTarget> target;

		RHIPipeLineCreateDesc& operator=(const RHIPipeLineDesc& desc)
		{
			if (this != &desc)
			{
				RHIPipeLineDesc::operator=(desc);
			}

			return *this;
		}

		bool operator==(const RHIPipeLineCreateDesc& other) const noexcept
		{
			if (!RHIPipeLineDesc::operator==(other))
			{
				return false;
			}

			if (layout != other.layout || target != other.target)
			{
				return false;
			}

			return true;
		}
	};

	struct RHIDrawIndexDesc : RHIDesc<eResourceType::eGraphics>
	{
		eDrawMode drawMode = eDrawMode::eNone;
		eDataType indexType = eDataType::eNone;

		uint32_t indexCount = 0;
		uint32_t indexOffset = 0;
		uint32_t baseVertex = 0;
		uint32_t instanceCount = 1;

		bool indirectDraw = false;
	};

	struct RHIDispatchDesc : RHIDesc<eResourceType::eCompute>
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

		RHIRenderTargetDesc& operator=(const RHIRenderTargetDesc& desc)
		{
			if (this != &desc)
			{
				colors = desc.colors;
				depthStencil = desc.depthStencil;
			}

			return *this;
		}
	};

	struct RHIRenderTargetCreateDesc : RHIRenderTargetDesc
	{
		Memory::RefPtr<const RHIVertexLayout> vertexLayout;

		RHIRenderTargetCreateDesc& operator=(const RHIRenderTargetDesc& desc)
		{
			if (this != &desc)
			{
				RHIRenderTargetDesc::operator=(desc);
			}

			return *this;
		}
	};
};


#endif // __WTR_RHIDESCRIPTIONS_H__