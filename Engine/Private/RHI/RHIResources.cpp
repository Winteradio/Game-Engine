#include <RHI/RHIResources.h>

#include <Renderer/GlobalRenderer.h>
#include <Log/include/Log.h>
#include <Memory/include/Core.h>

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

	eResourceState RHIResource::GetState() const
	{
		return m_state;
	}

	RHIBuffer::RHIBuffer(const RHIBufferDesc& desc)
		: m_desc(desc)
		, m_bufferState()
	{}

	eDataUsage RHIBuffer::GetDataUsage() const
	{
		return m_desc.dataUsage;
	}

	eDataType RHIBuffer::GetDataType() const
	{
		return m_desc.componentType;
	}

	uint32_t RHIBuffer::GetNumComponents() const
	{
		return m_desc.componentCount;
	}

	uint32_t RHIBuffer::GetStride() const
	{
		return m_desc.stride;
	}

	uint32_t RHIBuffer::GetSize() const
	{
		return m_desc.size;
	}

	const RHIBufferDesc& RHIBuffer::GetDesc() const
	{
		return m_desc;
	}

	const RHIBufferState& RHIBuffer::GetBufferState() const
	{
		return m_bufferState;
	}

	void RHIBuffer::SetDataUsage(const eDataUsage dataUsage)
	{
		m_desc.dataUsage = dataUsage;
	}

	void RHIBuffer::SetDataType(const eDataType dataType)
	{
		m_desc.componentType = dataType;
	}

	void RHIBuffer::SetNumComponents(const uint32_t numComponents)
	{
		m_desc.componentCount = numComponents;
	}

	void RHIBuffer::SetStride(const uint32_t stride)
	{
		m_desc.stride = stride;
	}

	void RHIBuffer::SetSize(const uint32_t size)
	{
		m_desc.size = size;
	}

	void RHIBuffer::SetDesc(const RHIBufferDesc& desc)
	{
		m_desc = desc;
	}

	void RHIBuffer::SetBufferState(const RHIBufferState& state) const
	{
		m_bufferState = state;
	}

	RHIVertexLayout::RHIVertexLayout(const RHIVertexLayoutDesc& desc)
		: m_desc(desc)
	{}

	size_t RHIVertexLayout::GetNumLayout() const
	{
		return m_desc.streams.Size();
	}

	RHIVertexAttribute RHIVertexLayout::GetAttribute(const VertexKey& key) const
	{
		auto itr = m_desc.streams.Find(key);
		if (itr != m_desc.streams.End())
		{
			return itr->second.attribute;
		}
		else
		{
			return {};
		}
	}

	RHIVertexBinding RHIVertexLayout::GetBinding(const VertexKey& key) const
	{
		auto itr = m_desc.streams.Find(key);
		if (itr != m_desc.streams.End())
		{
			return itr->second.binding;
		}
		else
		{
			return {};
		}
	}

	const RHIVertexLayoutDesc& RHIVertexLayout::GetDesc() const
	{
		return m_desc;
	}

	void RHIVertexLayout::SetDesc(const RHIVertexLayoutDesc& desc)
	{
		m_desc = desc;
	}

	void RHIVertexLayout::SetAttribute(const VertexKey& key, const RHIVertexAttribute& attribute)
	{
		m_desc.streams[key].attribute = attribute;
	}

	void RHIVertexLayout::SetBinding(const VertexKey& key, const RHIVertexBinding& binding)
	{
		m_desc.streams[key].binding = binding;
	}

	RHITexture::RHITexture(const RHITextureDesc& desc)
		: m_desc(desc)
		, m_textureState()
	{}

	uint32_t RHITexture::GetWidth() const
	{
		return m_desc.width;
	}

	uint32_t RHITexture::GetHeight() const
	{
		return m_desc.height;
	}

	uint32_t RHITexture::GetDepth() const
	{
		return m_desc.depth;
	}

	uint32_t RHITexture::GetMipLevels() const
	{
		return m_desc.level;
	}

	uint32_t RHITexture::GetSampleCount() const
	{
		return m_desc.sample;
	}

	ePixelFormat RHITexture::GetPixelFormat() const
	{
		return m_desc.pixelFormat;
	}

	eTextureDimension RHITexture::GetTextureDimension() const
	{
		return m_desc.textureDimension;
	}

	eDataType RHITexture::GetDataType() const
	{
		return m_desc.componentType;
	}

	const RHITextureDesc& RHITexture::GetDesc() const
	{
		return m_desc;
	}

	const RHITextureState& RHITexture::GetTextureState() const
	{
		return m_textureState;
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
		m_desc.level = mipLevels;
	}

	void RHITexture::SetSampleCount(const uint32_t sampleCount)
	{
		m_desc.sample = sampleCount;
	}

	void RHITexture::SetPixelFormat(const ePixelFormat pixelFormat)
	{
		m_desc.pixelFormat = pixelFormat;
	}

	void RHITexture::SetTextureDimension(const eTextureDimension textureDimension)
	{
		m_desc.textureDimension = textureDimension;
	}

	void RHITexture::SetDataType(const eDataType dataType)
	{
		m_desc.componentType = dataType;
	}

	void RHITexture::SetDesc(const RHITextureDesc& desc)
	{
		m_desc = desc;
	}

	void RHITexture::SetTextureState(const RHITextureState& state) const
	{
		m_textureState = state;
	}

	RHISampler::RHISampler(const RHISamplerDesc& desc)
		: m_desc(desc)
	{}

	eFilterMode RHISampler::GetMinFilter() const
	{
		return m_desc.minFilter;
	}

	eFilterMode RHISampler::GetMagFilter() const
	{
		return m_desc.magFilter;
	}

	eWrapMode RHISampler::GetWrapS() const
	{
		return m_desc.wrapS;
	}

	eWrapMode RHISampler::GetWrapT() const
	{
		return m_desc.wrapT;
	}

	eWrapMode RHISampler::GetWrapR() const
	{
		return m_desc.wrapR;
	}

	const RHISamplerDesc& RHISampler::GetDesc() const
	{
		return m_desc;
	}

	void RHISampler::SetMinFilter(const eFilterMode minFilter)
	{
		m_desc.minFilter = minFilter;
	}

	void RHISampler::SetMagFilter(const eFilterMode magFilter)
	{
		m_desc.magFilter = magFilter;
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

	void RHIStage::Clear()
	{
		m_attribs.Clear();
		m_attaches.Clear();
		m_consts.Clear();
		m_buffers.Clear();
		m_samplers.Clear();
		m_textures.Clear();
	}

	void RHIStage::AddAttrib(const VertexKey& key, const RHIShaderLayout& attrib)
	{
		auto itr = m_attribs.Find(key);
		if (itr != m_attribs.End())
		{
			LOGWARN() << "[RHI] RHIStage::AddAttrib - Already exists attrib for key: " << static_cast<int>(key.semantic) << ", " << static_cast<int>(key.semanticIndex);
			return;
		}

		m_attribs[key] = attrib;
	}

	void RHIStage::AddAttach(const Name& name, const RHIShaderLayout& attach)
	{
		auto itr = m_attaches.Find(name);
		if (itr != m_attaches.End())
		{
			LOGWARN() << "[RHI] RHIStage::AddAttach - Already exists attach for name: " << name;
			return;
		}

		m_attaches[name] = attach;
	}

	void RHIStage::AddConst(const Name& name, const RHIShaderConst& constant)
	{
		auto itr = m_consts.Find(name);
		if (itr != m_consts.End())
		{
			LOGWARN() << "[RHI] RHIStage::AddConst - Already exists constant for name: " << name;
			return;
		}

		m_consts[name] = constant;
	}

	void RHIStage::AddBuffer(const Name& name, const RHIShaderBuffer& buffer)
	{
		auto itr = m_buffers.Find(name);
		if (itr != m_buffers.End())
		{
			LOGWARN() << "[RHI] RHIStage::AddBuffer - Already exists buffer for name: " << name;
			return;
		}

		m_buffers[name] = buffer;
	}

	void RHIStage::AddSampler(const Name& name, const RHIShaderSampler& sampler)
	{
		auto itr = m_samplers.Find(name);
		if (itr != m_samplers.End())
		{
			LOGWARN() << "[RHI] RHIStage::AddSampler - Already exists sampler for name: " << name;
			return;
		}

		m_samplers[name] = sampler;
	}

	void RHIStage::AddTexture(const Name& name, const RHIShaderTexture& texture)
	{
		auto itr = m_textures.Find(name);
		if (itr != m_textures.End())
		{
			LOGWARN() << "[RHI] RHIStage::AddTexture - Already exists texture for name: " << name;
			return;
		}

		m_textures[name] = texture;
	}

	bool RHIStage::HasAttrib(const VertexKey& key) const
	{
		return m_attribs.Find(key) != m_attribs.End();
	}

	bool RHIStage::HasAttach(const Name& name) const
	{
		return m_attaches.Find(name) != m_attaches.End();
	}

	bool RHIStage::HasConst(const Name& name) const
	{
		return m_consts.Find(name) != m_consts.End();
	}

	bool RHIStage::HasBuffer(const Name& name) const
	{
		return m_buffers.Find(name) != m_buffers.End();
	}

	bool RHIStage::HasSampler(const Name& name) const
	{
		return m_samplers.Find(name) != m_samplers.End();
	}

	bool RHIStage::HasTexture(const Name& name) const
	{
		return m_textures.Find(name) != m_textures.End();
	}

	const RHIShaderLayout& RHIStage::GetAttrib(const VertexKey& key) const
	{
		auto itr = m_attribs.Find(key);
		if (itr != m_attribs.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIShaderLayout empty = {};
			return empty;
		}
	}

	const RHIShaderLayout& RHIStage::GetAttach(const Name& name) const
	{
		auto itr = m_attaches.Find(name);
		if (itr != m_attaches.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIShaderLayout empty = {};
			return empty;
		}
	}

	const RHIShaderConst& RHIStage::GetConst(const Name& name) const
	{
		auto itr = m_consts.Find(name);
		if (itr != m_consts.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIShaderConst empty = {};
			return empty;
		}
	}

	const RHIShaderBuffer& RHIStage::GetBuffer(const Name& name) const
	{
		auto itr = m_buffers.Find(name);
		if (itr != m_buffers.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIShaderBuffer empty = {};
			return empty;
		}
	}

	const RHIShaderSampler& RHIStage::GetSampler(const Name& name) const
	{
		auto itr = m_samplers.Find(name);
		if (itr != m_samplers.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIShaderSampler empty = {};
			return empty;
		}
	}

	const RHIShaderTexture& RHIStage::GetTexture(const Name& name) const
	{
		auto itr = m_textures.Find(name);
		if (itr != m_textures.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIShaderTexture empty = {};
			return empty;
		}
	}

	const RHIShaderAttribs& RHIStage::GetAttribs() const
	{
		return m_attribs;
	}
	
	const RHIShaderAttaches& RHIStage::GetAttaches() const
	{
		return m_attaches;
	}

	const RHIShaderConsts& RHIStage::GetConsts() const
	{
		return m_consts;
	}

	const RHIShaderBuffers& RHIStage::GetBuffers() const
	{
		return m_buffers;
	}

	const RHIShaderSamplers& RHIStage::GetSamplers() const
	{
		return m_samplers;
	}

	const RHIShaderTextures& RHIStage::GetTextures() const
	{
		return m_textures;
	}

	RHIShader::RHIShader(const RHIShaderDesc& desc)
		: RHIStage()
		, m_desc(desc)
	{
	}

	eShaderType RHIShader::GetShaderType() const
	{
		return m_desc.shaderType;
	}

	const RHIShaderDesc& RHIShader::GetDesc() const
	{
		return m_desc;
	}

	RHIPipeLine::RHIPipeLine(const RHIPipeLineDesc& desc)
		: RHIStage()
		, m_desc(desc)
	{}

	RHIColorState RHIPipeLine::GetColorState() const
	{
		return m_desc.color;
	}

	RHIDepthState RHIPipeLine::GetDepthState() const
	{
		return m_desc.depth;
	}

	RHIStencilState RHIPipeLine::GetStencilState() const
	{
		return m_desc.stencil;
	}

	RHIBlendState RHIPipeLine::GetBlendState() const
	{
		return m_desc.blend;
	}

	RHIRasterizerState RHIPipeLine::GetRasterizerState() const
	{
		return m_desc.rasterizer;
	}

	const RHIPipeLineDesc& RHIPipeLine::GetDesc() const
	{
		return m_desc;
	}

	void RHIPipeLine::Clear()
	{
		RHIStage::Clear();

		m_desc.vertexShader = nullptr;
		m_desc.geometryShader = nullptr;
		m_desc.hullShader = nullptr;
		m_desc.domainShader = nullptr;
		m_desc.pixelShader = nullptr;
		m_desc.computeShader = nullptr;
	}

	bool RHIPipeLine::HasShader(const eShaderType shaderType) const
	{
		switch (shaderType)
		{
		case eShaderType::eVertex:		return m_desc.vertexShader != nullptr;
		case eShaderType::eGeometry:	return m_desc.geometryShader != nullptr;
		case eShaderType::eHull:		return m_desc.hullShader != nullptr;
		case eShaderType::eDomain:		return m_desc.domainShader != nullptr;
		case eShaderType::ePixel:		return m_desc.pixelShader != nullptr;
		case eShaderType::eCompute:		return m_desc.computeShader != nullptr;
		default:						return false;
		}
	}

	namespace
	{
		template<typename Key, typename Value, typename MergeFunc>
		bool MergeInto(wtr::HashMap<Key, Value>& dst, const wtr::HashMap<Key, Value>& src, MergeFunc merge)
		{
			for (const auto& [key, value] : src)
			{
				auto itr = dst.Find(key);
				if (itr == dst.End())
				{
					dst[key] = value;
				}
				else if (!merge(itr->second, value))
				{
					return false;
				}
			}

			return true;
		}
	}

	bool RHIPipeLine::AddShader(const Memory::RefPtr<const RHIShader>& shader)
	{
		if (!shader || shader->GetState() != eResourceState::eReady)
		{
			LOGWARN() << "[RHI][PipeLine] AddShader - Invalid shader resource.";
			return false;
		}

		if (HasShader(shader->GetShaderType()))
		{
			LOGWARN() << "[RHI][PipeLine] AddShader - The shader type already exists.";
			return false;
		}

		RHIShaderAttribs attribs = m_attribs;
		RHIShaderAttaches attaches = m_attaches;
		RHIShaderConsts consts = m_consts;
		RHIShaderBuffers buffers = m_buffers;
		RHIShaderSamplers samplers = m_samplers;
		RHIShaderTextures textures = m_textures;

		const bool merged =
			MergeInto(attribs, shader->GetAttribs(),
				[](RHIShaderLayout& prev, const RHIShaderLayout& next)
				{
					return prev == next;
				}) &&
			MergeInto(attaches, shader->GetAttaches(),
				[](RHIShaderLayout& prev, const RHIShaderLayout& next)
				{
					return prev == next;
				}) &&
			MergeInto(consts, shader->GetConsts(),
				[](RHIShaderConst& prev, const RHIShaderConst& next)
				{
					if (prev.binding != next.binding)
					{
						return false;
					}

					prev.stage = prev.stage | next.stage;
					return true;
				}) &&
			MergeInto(buffers, shader->GetBuffers(),
				[](RHIShaderBuffer& prev, const RHIShaderBuffer& next)
				{
					if (prev.binding != next.binding || prev.state != next.state || prev.usage != next.usage)
					{
						return false;
					}

					prev.stage = prev.stage | next.stage;
					return true;
				}) &&
			MergeInto(samplers, shader->GetSamplers(),
				[](RHIShaderSampler& prev, const RHIShaderSampler& next)
				{
					if (prev.binding != next.binding || prev.state != next.state || prev.usage != next.usage)
					{
						return false;
					}

					prev.stage = prev.stage | next.stage;
					return true;
				}) &&
			MergeInto(textures, shader->GetTextures(),
				[](RHIShaderTexture& prev, const RHIShaderTexture& next)
				{
					if (prev.location != next.location || prev.state != next.state || prev.usage != next.usage)
					{
						return false;
					}

					prev.stage = prev.stage | next.stage;
					return true;
				});

		if (!merged)
		{
			LOGWARN() << "[RHI][PipeLine] AddShader - Conflicting shader interface.";
			return false;
		}

		m_attribs = std::move(attribs);
		m_attaches = std::move(attaches);
		m_consts = std::move(consts);
		m_buffers = std::move(buffers);
		m_samplers = std::move(samplers);
		m_textures = std::move(textures);

		switch (shader->GetShaderType())
		{
		case eShaderType::eVertex:		m_desc.vertexShader = shader;	break;
		case eShaderType::eGeometry:	m_desc.geometryShader = shader;	break;
		case eShaderType::eHull:		m_desc.hullShader = shader;		break;
		case eShaderType::eDomain:		m_desc.domainShader = shader;	break;
		case eShaderType::ePixel:		m_desc.pixelShader = shader;	break;
		case eShaderType::eCompute:		m_desc.computeShader = shader;	break;
		default:						break;
		}

		return true;
	}

	RHIRenderPass::RHIRenderPass(const RHIRenderPassDesc& desc)
		: m_desc(desc)
	{
	}

	const RHIRenderPassDesc& RHIRenderPass::GetDesc() const
	{
		return m_desc;
	}

	bool RHIRenderPass::HasColorAttach(const uint32_t slot) const
	{
		for (const auto& color : m_desc.colors)
		{
			if (color.slot == slot)
			{
				return true;
			}
		}

		return false;
	}

	bool RHIRenderPass::HasDepthStencilAttach() const
	{
		if (m_desc.depthStencil.texture)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	size_t RHIRenderPass::GetColorAttachCount() const
	{
		return m_desc.colors.Size();
	}
};