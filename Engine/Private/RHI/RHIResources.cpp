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

	RHIVertexLayout::RHIVertexLayout(const RHIVertexLayoutDesc& desc)
		: m_desc(desc)
	{}

	const size_t RHIVertexLayout::GetNumAttributes() const
	{
		return m_desc.vertexStreams.Size();
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