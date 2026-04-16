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

	eResourceState& operator|=(eResourceState& lhs, const eResourceState rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	eResourceState& operator&=(eResourceState& lhs, const eResourceState rhs)
	{
		lhs = lhs & rhs;
		return lhs;
	}

	eResourceState& operator^=(eResourceState& lhs, const eResourceState rhs)
	{
		lhs = lhs ^ rhs;
		return lhs;
	}

	void* RHIResource::GetRawBuffer()
	{
		return reinterpret_cast<void*>(this);
	}

	const void* RHIResource::GetRawBuffer() const
	{
		return reinterpret_cast<const void*>(this);
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

	const eBufferType RHIBuffer::GetBufferType() const
	{
		return m_desc.bufferType;
	}

	const eDataAccess RHIBuffer::GetAccessType() const
	{
		return m_desc.accessType;
	}

	const eDataType RHIBuffer::GetComponentType() const
	{
		return m_desc.componentType;
	}

	const uint32_t RHIBuffer::GetNumComponents() const
	{
		return m_desc.numComponents;
	}

	const uint32_t RHIBuffer::GetCount() const
	{
		return m_desc.count;
	}

	const uint32_t RHIBuffer::GetSize() const
	{
		return m_desc.size;
	}

	const uint32_t RHIBuffer::GetStride() const
	{
		return m_desc.stride;
	}

	void RHIBuffer::SetBufferType(const eBufferType bufferType)
	{
		m_desc.bufferType = bufferType;
	}

	void RHIBuffer::SetAccessType(const eDataAccess accessType)
	{
		m_desc.accessType = accessType;
	}

	void RHIBuffer::SetComponentType(const eDataType componentType)
	{
		m_desc.componentType = componentType;
	}

	void RHIBuffer::SetNumComponents(const uint32_t numComponents)
	{
		m_desc.numComponents = numComponents;
	}

	void RHIBuffer::SetCount(const uint32_t count)
	{
		m_desc.count = count;
	}

	void RHIBuffer::SetSize(const uint32_t size)
	{
		m_desc.size = size;
	}

	void RHIBuffer::SetStride(const uint32_t stride)
	{
		m_desc.stride = stride;
	}

	void RHIBuffer::SetDesc(const RHIBufferDesc& desc)
	{
		m_desc = desc;
	}

	RHIVertexLayout::RHIVertexLayout(const RHIVertexLayoutDesc& desc)
		: m_desc(desc)
	{}

	const size_t RHIVertexLayout::GetNumAttributes() const
	{
		return m_desc.vertexStreams.Size();
	}

	void RHIVertexLayout::SetDesc(const RHIVertexLayoutDesc& desc)
	{
		m_desc = desc;
	}

	RHITexture::RHITexture(const RHITextureDesc& desc)
		: m_desc(desc)
	{}

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

	const eTextureType RHITexture::GetTextureType() const
	{
		return m_desc.textureType;
	}

	void RHITexture::SetWidth(const uint32_t width)
	{
		m_desc.width = width;
	}

	void RHITexture::SetHeight(const uint32_t height)
	{
		m_desc.height = height;
	}

	void RHITexture::SetDepth(const uint32_t depth)
	{
		m_desc.depth = depth;
	}

	void RHITexture::SetMipLevels(const uint32_t mipLevels)
	{
		m_desc.mipLevels = mipLevels;
	}

	void RHITexture::SetSampleCount(const uint32_t sampleCount)
	{
		m_desc.sampleCount = sampleCount;
	}

	void RHITexture::SetPixelFormat(const ePixelFormat pixelFormat)
	{
		m_desc.format = pixelFormat;
	}

	void RHITexture::SetTextureUsage(const eTextureUsage textureUsage)
	{
		m_desc.usage = textureUsage;
	}

	void RHITexture::SetTextureType(const eTextureType textureType)
	{
		m_desc.textureType = textureType;
	}

	void RHITexture::SetDesc(const RHITextureDesc& desc)
	{
		m_desc = desc;
	}

	RHISampler::RHISampler(const RHISamplerDesc& desc)
		: m_desc(desc)
	{}

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

	void RHISampler::SetMinFilter(const eFilterMode minFilter)
	{
		m_desc.minFilter = minFilter;
	}

	void RHISampler::SetMagFilter(const eFilterMode magFilter)
	{
		m_desc.magFilter = magFilter;
	}

	void RHISampler::SetMipFilter(const eFilterMode mipFilter)
	{
		m_desc.mipFilter = mipFilter;
	}

	void RHISampler::SetWrapS(const eWrapMode wrapS)
	{
		m_desc.wrapS = wrapS;
	}

	void RHISampler::SetWrapT(const eWrapMode wrapT)
	{
		m_desc.wrapT = wrapT;
	}

	void RHISampler::SetWrapR(const eWrapMode wrapR)
	{
		m_desc.wrapR = wrapR;
	}

	void RHISampler::SetDesc(const RHISamplerDesc& desc)
	{
		m_desc = desc;
	}

	RHIShader::RHIShader(const RHIShaderDesc& desc)
		: m_desc(desc)
	{
	}

	const eShaderType RHIShader::GetShaderType() const
	{
		return m_desc.shaderType;
	}

	RHIPipeLine::RHIPipeLine(const RHIPipeLineDesc& desc)
		: m_desc(desc)
	{}

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

	void RHIPipeLine::SetClearState(const RHIClearState clear)
	{
		m_desc.clear = clear;
	}

	void RHIPipeLine::SetColorState(const RHIColorState color)
	{
		m_desc.color = color;
	}

	void RHIPipeLine::SetDepthState(const RHIDepthState depth)
	{
		m_desc.depth = depth;
	}

	void RHIPipeLine::SetStencilState(const RHIStencilState stencil)
	{
		m_desc.stencil = stencil;
	}

	void RHIPipeLine::SetBlendState(const RHIBlendState blend)
	{
		m_desc.blend = blend;
	}

	void RHIPipeLine::SetRasterizerState(const RHIRasterizerState rasterizer)
	{
		m_desc.rasterizer = rasterizer;
	}

	void RHIPipeLine::SetDesc(const RHIPipeLineDesc& desc)
	{
		m_desc = desc;
	}

	void RHIPipeLine::AddSlot(const std::string& name, const RHIResourceBinding& binding)
	{
		m_slots.Insert(std::make_pair(name, binding));
	}

	bool RHIPipeLine::HasSlot(const std::string& name) const
	{
		return m_slots.Find(name) != m_slots.End();
	}

	size_t RHIPipeLine::GetSlotCount() const
	{
		return m_slots.Size();
	}

	const RHIResourceBinding RHIPipeLine::GetBindingSlot(const std::string& name) const
	{
		auto itr = m_slots.Find(name);
		if (itr != m_slots.End())
		{
			return itr->second;
		}
		else
		{
			return RHIResourceBinding{};
		}
	}
};