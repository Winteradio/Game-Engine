#include <RHI/RHIResources.h>

namespace wtr
{
	eResourceState operator|(const eResourceState lhs, const eResourceState rhs)
	{
		return static_cast<eResourceState>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	eResourceState operator&(const eResourceState lhs, const eResourceState rhs)
	{
		return static_cast<eResourceState>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}

	eResourceState operator^(const eResourceState lhs, const eResourceState rhs)
	{
		return static_cast<eResourceState>(static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs));
	}

	eResourceState operator~(const eResourceState state)
	{
		return static_cast<eResourceState>(~static_cast<uint8_t>(state));
	}

	eResourceState operator|=(eResourceState lhs, const eResourceState rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	eResourceState operator&=(eResourceState lhs, const eResourceState rhs)
	{
		lhs = lhs & rhs;
		return lhs;
	}

	eResourceState operator^=(eResourceState lhs, const eResourceState rhs)
	{
		lhs = lhs ^ rhs;
		return lhs;
	}

	const void* RHIResource::GetRawBuffer() const
	{
		return nullptr;
	}

	void RHIResource::SetState(const eResourceState eState)
	{
		m_state = eState;
	}

	const eResourceState RHIResource::GetState() const
	{
		return m_state;
	}

	RHIBuffer::RHIBuffer(const RHIBufferDesc& desc)
		: m_desc(desc)
	{}

	const void* RHIBuffer::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const eBufferType RHIBuffer::GetType() const
	{
		return m_desc.bufferType;
	}

	const eDataAccess RHIBuffer::GetAccessType() const
	{
		return m_desc.accessType;
	}

	const uint32_t RHIBuffer::GetSize() const
	{
		return m_desc.size;
	}

	const uint32_t RHIBuffer::GetStride() const
	{
		return m_desc.stride;
	}

	RHITexture::RHITexture(const RHITextureDesc& desc)
		: m_desc(desc)
	{}

	const void* RHITexture::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const uint32_t RHITexture::GetWidth() const
	{
		return m_desc.width;
	}

	const uint32_t RHITexture::GetHeight() const
	{
		return m_desc.height;
	}

	const uint32_t RHITexture::GetDepth() const
	{
		return m_desc.depth;
	}

	const uint32_t RHITexture::GetMipLevels() const
	{
		return m_desc.mipLevels;
	}

	const uint32_t RHITexture::GetSampleCount() const
	{
		return m_desc.sampleCount;
	}

	const ePixelFormat RHITexture::GetPixelFormat() const
	{
		return m_desc.format;
	}

	const eTextureUsage RHITexture::GetTextureUsage() const
	{
		return m_desc.usage;
	}

	RHISampler::RHISampler(const RHISamplerDesc& desc)
		: m_desc(desc)
	{}

	const void* RHISampler::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const eFilterMode RHISampler::GetMinFilter() const
	{
		return m_desc.minFilter;
	}

	const eFilterMode RHISampler::GetMagFilter() const
	{
		return m_desc.magFilter;
	}

	const eFilterMode RHISampler::GetMipFilter() const
	{
		return m_desc.mipFilter;
	}

	const eWrapMode RHISampler::GetWrapS() const
	{
		return m_desc.wrapS;
	}

	const eWrapMode RHISampler::GetWrapT() const
	{
		return m_desc.wrapT;
	}

	const eWrapMode RHISampler::GetWrapR() const
	{
		return m_desc.wrapR;
	}

	RHIVertexShader::RHIVertexShader(const RHIVertexShaderDesc& desc)
		: m_dsec(desc)
	{}

	const void* RHIVertexShader::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	RHIGeometryShader::RHIGeometryShader(const RHIGeometryShaderDesc& desc)
		: m_desc(desc)
	{}

	const void* RHIGeometryShader::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	RHIPixelShader::RHIPixelShader(const RHIPixelShaderDesc& desc)
		: m_desc(desc)
	{}

	const void* RHIPixelShader::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	RHIComputeShader::RHIComputeShader(const RHIComputeShaderDesc& desc)
		: m_desc(desc)
	{}

	const void* RHIComputeShader::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	RHIPipeLine::RHIPipeLine(const RHIPipeLineDesc& desc)
		: m_desc(desc)
	{}

	const void* RHIPipeLine::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const RHIClearState RHIPipeLine::GetClearState() const
	{
		return m_desc.clear;
	}

	const RHIColorState RHIPipeLine::GetColorState() const
	{
		return m_desc.color;
	}

	const RHIDepthState RHIPipeLine::GetDepthState() const
	{
		return m_desc.depth;
	}

	const RHIStencilState RHIPipeLine::GetStencilState() const
	{
		return m_desc.stencil;
	}

	const RHIBlendState	RHIPipeLine::GetBlendState() const
	{
		return m_desc.blend;
	}

	const RHIRasterizerState RHIPipeLine::GetRasterizerState() const
	{
		return m_desc.rasterizer;
	}
};