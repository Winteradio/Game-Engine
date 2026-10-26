#ifndef __WTR_RHIDESCRIPTIONS_H__
#define __WTR_RHIDESCRIPTIONS_H__

#include <RHI/RHIStates.h>
#include <Renderer/RenderData.h>
#include <Framework/Name.h>

#include <Container/include/DynamicArray.h>
#include <Container/include/HashMap.h>

namespace wtr
{
	class RHIBuffer;
	class RHITexture;
	class RHIShader;
	class RHIVertexLayout;
	class RHIRenderPass;
};

namespace wtr
{
	struct RHIVertexAttribute
	{
		uint32_t		location		= UINT32_MAX;

		uint32_t		componentOffset = 0;
		uint32_t		componentCount	= 0;
		eDataType		componentType	= eDataType::eNone;

		uint8_t			semanticIndex	= 0;
		eVertexSemantic	semantic		= eVertexSemantic::eNone;
		
		bool			normalized		= false;
	};

	struct RHIVertexSlot
	{
		uint32_t		stride	= 0;
		uint32_t 		binding = UINT32_MAX;
		eDataDivisor	divisor	= eDataDivisor::eNone;
	};

	struct RHIVertexStream
	{
		RHIVertexAttribute	attribute;
		RHIVertexSlot		slot;
	};

	struct RHIStageLayout
	{
		uint32_t	location		= UINT32_MAX;

		uint32_t	componentCount	= 0;
		eDataType	componentType	= eDataType::eNone;

		bool operator==(const RHIStageLayout& other) const noexcept
		{
			return (location == other.location) &&
				(componentCount == other.componentCount) &&
				(componentType == other.componentType);
		}

		bool operator!=(const RHIStageLayout& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIStageAttrib : RHIStageLayout
	{
		uint32_t binding			= UINT32_MAX;
		uint32_t componentOffset	= 0;
		bool normalized				= false;

		RHIStageAttrib& operator=(const RHIStageLayout& other)
		{
			RHIStageLayout::operator=(other);
			return *this;
		}

		bool operator==(const RHIStageAttrib& other) const noexcept
		{
			return RHIStageAttrib::operator==(other) &&
				(binding == other.binding) &&
				(componentOffset == other.componentOffset) &&
				(normalized == other.normalized);
		}

		bool operator!=(const RHIStageAttrib& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIStageAttach
	{
		ePixelFormat format = ePixelFormat::eNone;

		bool operator==(const RHIStageAttach& other) const noexcept
		{
			return format == other.format;
		}

		bool operator!=(const RHIStageAttach& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIStageConst
	{
		uint32_t	binding				= UINT32_MAX;

		uint32_t	componentOffset		= 0;
		uint32_t	componentRange		= 0;
		uint32_t	componentCount		= 0;
		eDataType	componentType		= eDataType::eNone;

		eDataStage stage				= eDataStage::eNone;

		bool operator==(const RHIStageConst& other) const noexcept
		{
			return  (binding == other.binding) &&
				(componentOffset == other.componentOffset) &&
				(componentRange == other.componentRange) &&
				(componentCount == other.componentCount) &&
				(componentType == other.componentType) &&
				(stage == other.stage);
		}

		bool operator!=(const RHIStageConst& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIStageBuffer : RHIBufferState
	{
		uint32_t binding = UINT32_MAX;

		bool operator==(const RHIStageBuffer& other) const noexcept
		{
			return RHIBufferState::operator==(other) &&
				(binding == other.binding);
		}

		bool operator!=(const RHIStageBuffer& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIStageSampler : RHITextureState
	{
		uint32_t binding = UINT32_MAX;

		bool operator==(const RHIStageSampler& other) const noexcept
		{
			return RHITextureState::operator==(other) &&
				(binding == other.binding);
		}
		
		bool operator!=(const RHIStageSampler& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIStageTexture : RHITextureState
	{
		uint32_t binding = UINT32_MAX;

		bool operator==(const RHIStageTexture& other) const noexcept
		{
			return RHITextureState::operator==(other) &&
				(binding == other.binding);
		}

		bool operator!=(const RHIStageTexture& other) const noexcept
		{
			return !(*this == other);
		}
	};

	using RHIStageInputs	= wtr::HashMap<VertexKey, RHIStageLayout>;
	using RHIStageOutputs	= wtr::HashMap<Name, RHIStageLayout>;
	using RHIStageAttribs	= wtr::HashMap<VertexKey, RHIStageAttrib>;
	using RHIStageAttaches 	= wtr::HashMap<uint32_t, RHIStageAttach>;
	using RHIStageConsts	= wtr::HashMap<Name, RHIStageConst>;
	using RHIStageBuffers	= wtr::HashMap<Name, RHIStageBuffer>;
	using RHIStageSamplers	= wtr::HashMap<Name, RHIStageSampler>;
	using RHIStageTextures	= wtr::HashMap<Name, RHIStageTexture>;

	struct RHIConstDesc : DataBlob
	{
		bool transpose = false;

		bool operator==(const RHIConstDesc& other) const noexcept
		{
			return DataBlob::operator==(other) && transpose == other.transpose;
		}

		bool operator!=(const RHIConstDesc& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIDataDesc
	{
		eDataUsage	usage	= eDataUsage::eNone;
		uint32_t	size	= 0;
	};

	struct RHIBufferDesc : RHIDataDesc
	{
		RHIBufferDesc() = default;
		RHIBufferDesc(const RHIDataDesc& desc)
			: RHIDataDesc(desc)
		{}

		RHIBufferDesc& operator=(const RHIDataDesc& desc)
		{
			RHIDataDesc::operator=(desc);
			return *this;
		}
	};

	struct RHIBufferCreateDesc : RHIBufferDesc
	{
		eDataType componentType = eDataType::eNone;
		uint32_t componentCount = 0;
		uint32_t stride = 0;

		wtr::DynamicArray<DataBlob> blobs;

		RHIBufferCreateDesc() = default;
		RHIBufferCreateDesc(const RHIDataDesc& desc)
			: RHIBufferDesc(desc)
		{}

		RHIBufferCreateDesc(const RHIBufferDesc& desc)
			: RHIBufferDesc(desc)
		{}

		RHIBufferCreateDesc& operator=(const RHIDataDesc& desc)
		{
			RHIDataDesc::operator=(desc);
			return *this;
		}

		RHIBufferCreateDesc& operator=(const RHIBufferDesc& desc)
		{
			RHIBufferDesc::operator=(desc);
			return *this;
		}
	};

	struct RHIBufferUpdateDesc : RHIBufferCreateDesc
	{
		uint32_t offset = 0;

		RHIBufferUpdateDesc() = default;
		RHIBufferUpdateDesc(const RHIDataDesc& desc)
			: RHIBufferCreateDesc(desc)
		{}

		RHIBufferUpdateDesc(const RHIBufferDesc& desc)
			: RHIBufferCreateDesc(desc)
		{}

		RHIBufferUpdateDesc(const RHIBufferCreateDesc& desc)
			: RHIBufferCreateDesc(desc)
		{}

		RHIBufferUpdateDesc& operator=(const RHIDataDesc& desc)
		{
			RHIDataDesc::operator=(desc);
			return *this;
		}

		RHIBufferUpdateDesc& operator=(const RHIBufferDesc& desc)
		{
			RHIBufferDesc::operator=(desc);
			return *this;
		}

		RHIBufferUpdateDesc& operator=(const RHIBufferCreateDesc& desc)
		{
			RHIBufferCreateDesc::operator=(desc);
			return *this;
		}
	};

	struct RHITextureDesc : RHIDataDesc
	{
		uint32_t			width			= 1;
		uint32_t			height			= 1;
		uint32_t			depth			= 1;

		uint32_t			level			= 1;
		uint32_t			layer			= 1;
		uint32_t			face			= 1;
		uint32_t			sample			= 1;
		
		eTextureFormat		textureFormat	= eTextureFormat::eNone;
		eTextureType		textureType		= eTextureType::eNone;

		bool				generateMips	= false;
		bool				compressed		= false;

		RHITextureDesc() = default;
		RHITextureDesc(const RHIDataDesc& desc)
			: RHIDataDesc(desc)
		{}

		RHITextureDesc& operator=(const RHIDataDesc& desc)
		{
			RHIDataDesc::operator=(desc);
			return *this;
		}
	};

	struct RHITextureCreateDesc : RHITextureDesc
	{
		wtr::DynamicArray<TextureRegion> blobs;

		eTextureUsage textureUsage = eTextureUsage::eNone;

		RHITextureCreateDesc() = default;
		RHITextureCreateDesc(const RHIDataDesc& desc)
			: RHITextureDesc(desc)
		{}

		RHITextureCreateDesc(const RHITextureDesc& desc)
			: RHITextureDesc(desc)
		{}

		RHITextureCreateDesc& operator=(const RHIDataDesc& desc)
		{
			RHIDataDesc::operator=(desc);
			return *this;
		}

		RHITextureCreateDesc& operator=(const RHITextureDesc& desc)
		{
			RHITextureDesc::operator=(desc);
			return *this;
		}
	};

	struct RHITextureUpdateDesc : RHITextureCreateDesc
	{
		RHITextureUpdateDesc() = default;
		RHITextureUpdateDesc(const RHIDataDesc& desc)
			: RHITextureCreateDesc(desc)
		{}

		RHITextureUpdateDesc(const RHITextureDesc& desc)
			: RHITextureCreateDesc(desc)
		{}

		RHITextureUpdateDesc(const RHITextureCreateDesc& desc)
			: RHITextureCreateDesc(desc)
		{}

		RHITextureUpdateDesc& operator=(const RHIDataDesc& desc)
		{
			RHIDataDesc::operator=(desc);
			return *this;
		}

		RHITextureUpdateDesc& operator=(const RHITextureDesc& desc)
		{
			RHITextureDesc::operator=(desc);
			return *this;
		}

		RHITextureUpdateDesc& operator=(const RHITextureCreateDesc& desc)
		{
			RHITextureCreateDesc::operator=(desc);
			return *this;
		}
	};

	struct RHIVertexLayoutDesc
	{
		wtr::HashMap<VertexKey, RHIVertexStream> streams;
	};

	struct RHIVertexLayoutCreateDesc : RHIVertexLayoutDesc
	{
		RHIVertexLayoutCreateDesc() = default;
		RHIVertexLayoutCreateDesc(const RHIVertexLayoutDesc& desc)
			: RHIVertexLayoutDesc(desc)
		{}

		RHIVertexLayoutCreateDesc& operator=(const RHIVertexLayoutDesc& desc)
		{
			RHIVertexLayoutDesc::operator=(desc);
			return *this;
		}
	};

	struct RHIVertexLayoutUpdateDesc : RHIVertexLayoutDesc
	{
		RHIVertexLayoutUpdateDesc() = default;
		RHIVertexLayoutUpdateDesc(const RHIVertexLayoutDesc& desc)
			: RHIVertexLayoutDesc(desc)
		{}

		RHIVertexLayoutUpdateDesc& operator=(const RHIVertexLayoutDesc& desc)
		{
			RHIVertexLayoutDesc::operator=(desc);
			return *this;
		}
	};

	struct RHISamplerDesc
	{
		eFilterMode			minFilter		= eFilterMode::eNone;
		eFilterMode			magFilter		= eFilterMode::eNone;
		eWrapMode			wrapS			= eWrapMode::eNone;
		eWrapMode			wrapT			= eWrapMode::eNone;
		eWrapMode			wrapR			= eWrapMode::eNone;
		eWrapBorderColor	wrapBorderColor	= eWrapBorderColor::eNone;

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

		bool operator!=(const RHISamplerDesc& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHISamplerCreateDesc : RHISamplerDesc
	{
		RHISamplerCreateDesc() = default;
		RHISamplerCreateDesc(const RHISamplerDesc& desc)
			: RHISamplerDesc(desc)
		{}

		RHISamplerCreateDesc& operator=(const RHISamplerDesc& desc)
		{
			RHISamplerDesc::operator=(desc);
			return *this;
		}
	};

	struct RHIShaderDesc
	{
		eShaderType	shaderType	= eShaderType::eNone;
	};

	struct RHIShaderCreateDesc : RHIShaderDesc
	{
		DataBlob blob;

		RHIShaderCreateDesc() = default;
		RHIShaderCreateDesc(const RHIShaderDesc& desc)
			: RHIShaderDesc(desc)
		{}

		RHIShaderCreateDesc& operator=(const RHIShaderDesc& desc)
		{
			RHIShaderDesc::operator=(desc);
			return *this;
		}
	};

	struct RHIPipeLineDesc
	{
		RHIColorState					color;
		RHIDepthState					depth;
		RHIStencilState					stencil;
		RHIBlendState					blend;
		RHIRasterizerState				rasterizer;

		Memory::RefPtr<const RHIShader>	vertexShader;
		Memory::RefPtr<const RHIShader>	geometryShader;
		Memory::RefPtr<const RHIShader>	hullShader;
		Memory::RefPtr<const RHIShader>	domainShader;
		Memory::RefPtr<const RHIShader>	pixelShader;
		Memory::RefPtr<const RHIShader>	computeShader;

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

		bool operator!=(const RHIPipeLineDesc& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIPipeLineCreateDesc : RHIPipeLineDesc
	{
		Memory::RefPtr<const RHIVertexLayout> layout;
		Memory::RefPtr<const RHIRenderPass> target;

		RHIPipeLineCreateDesc() = default;
		RHIPipeLineCreateDesc(const RHIPipeLineDesc& desc)
			: RHIPipeLineDesc(desc)
		{}

		RHIPipeLineCreateDesc& operator=(const RHIPipeLineDesc& desc)
		{
			RHIPipeLineDesc::operator=(desc);
			return *this;
		}

		bool operator==(const RHIPipeLineCreateDesc& other) const noexcept
		{
			if (!RHIPipeLineDesc::operator==(other))
			{
				return false;
			}

			if ((layout != other.layout) || (target != other.target))
			{
				return false;
			}

			return true;
		}

		bool operator!=(const RHIPipeLineCreateDesc& other) const noexcept
		{
			return !(*this == other);
		}
	};

	struct RHIDrawDirectDesc
	{
		eDrawMode	drawMode		= eDrawMode::eNone;
		eDataType	indexType		= eDataType::eNone;

		uint32_t	indexCount		= 0;
		uint32_t	indexOffset		= 0;

		int32_t		baseVertex		= 0;
		uint32_t	instanceCount	= 1;
	};

	struct RHIDrawIndirectDesc
	{
		eDrawMode	drawMode = eDrawMode::eNone;
		eDataType	indexType = eDataType::eNone;

		uint32_t	indexCount = 0;
		uint32_t	indexOffset = 0;

		int32_t		baseVertex = 0;

		uint32_t	baseInstance = 0;
		uint32_t	instanceCount = 1;
	};

	struct RHIDispatchDesc
	{
		uint32_t	groupX	= 0;
		uint32_t	groupY	= 0;
		uint32_t	groupZ	= 0;
	};

	struct RHIAttachment
	{
		eAttachmentType		type	= eAttachmentType::eNone;
		eAttachmentLoadOp	loadOp	= eAttachmentLoadOp::eDontCare;
		eAttachmentStoreOp	storeOp	= eAttachmentStoreOp::eStore;

		Memory::RefPtr<const RHITexture>	texture;
	};

	struct RHIColorAttachment : RHIAttachment
	{
		uint32_t location = UINT32_MAX;

		RHIColorAttachment() = default;
		RHIColorAttachment(const RHIAttachment& desc)
			: RHIAttachment(desc)
		{}

		RHIColorAttachment& operator=(const RHIAttachment& desc)
		{
			RHIAttachment::operator=(desc);
			return *this;
		}
	};

	struct RHIDepthStencilAttachment : RHIAttachment
	{
		RHIDepthStencilAttachment() = default;
		RHIDepthStencilAttachment(const RHIAttachment& desc)
			: RHIAttachment(desc)
		{}

		RHIDepthStencilAttachment& operator=(const RHIAttachment& desc)
		{
			RHIAttachment::operator=(desc);
			return *this;
		}
	};

	struct RHIRenderPassDesc
	{
		wtr::DynamicArray<RHIColorAttachment> colors;
		RHIDepthStencilAttachment depthStencil;
	};

	struct RHIRenderPassCreateDesc : RHIRenderPassDesc
	{
		RHIRenderPassCreateDesc() = default;
		RHIRenderPassCreateDesc(const RHIRenderPassDesc& desc)
			: RHIRenderPassDesc(desc)
		{}

		RHIRenderPassCreateDesc& operator=(const RHIRenderPassDesc& desc)
		{
			RHIRenderPassDesc::operator=(desc);
			return *this;
		}
	};

	struct RHIBufferTransition
	{
		Memory::RefPtr<const RHIBuffer>		buffer;

		RHIBufferState						before;
		RHIBufferState						after;
	};

	struct RHITextureTransition
	{
		Memory::RefPtr<const RHITexture>	texture;

		RHITextureState						before;
		RHITextureState						after;
	};

	struct RHITransitionDesc
	{
		wtr::DynamicArray<RHIBufferTransition>	buffers;
		wtr::DynamicArray<RHITextureTransition>	textures;
	};

	struct RHIBufferView
	{
		Memory::RefPtr<const RHIBuffer>		buffer;
		size_t offset = 0;
		size_t range = 0;
	};

	struct RHITextureView
	{
		// TODO
	};
};


#endif // __WTR_RHIDESCRIPTIONS_H__