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

	RHIVertexLayout::RHIVertexLayout(const RHIVertexLayoutDesc& desc)
		: m_desc(desc)
	{
	}

	size_t RHIVertexLayout::GetNumLayout() const
	{
		return m_desc.streams.Size();
	}

	const RHIVertexStream& RHIVertexLayout::GetStream(const VertexKey& key) const
	{
		auto itr = m_desc.streams.Find(key);
		if (itr != m_desc.streams.End())
		{
			return itr->second;
		}
		else
		{
			static RHIVertexStream emptyStream{};
			return emptyStream;
		}
	}

	const RHIVertexAttribute& RHIVertexLayout::GetAttrib(const VertexKey& key) const
	{
		auto itr = m_desc.streams.Find(key);
		if (itr != m_desc.streams.End())
		{
			return itr->second.attribute;
		}
		else
		{
			static RHIVertexAttribute emptyAttrib{};
			return emptyAttrib;
		}
	}

	const RHIVertexSlot& RHIVertexLayout::GetSlot(const VertexKey& key) const
	{
		auto itr = m_desc.streams.Find(key);
		if (itr != m_desc.streams.End())
		{
			return itr->second.slot;
		}
		else
		{
			static RHIVertexSlot emptySlot{};
			return emptySlot;
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

	void RHIVertexLayout::SetStream(const VertexKey& key, const RHIVertexStream& stream)
	{
		m_desc.streams[key] = stream;
	}

	void RHIVertexLayout::SetAttrib(const VertexKey& key, const RHIVertexAttribute& attribute)
	{
		m_desc.streams[key].attribute = attribute;
	}

	void  RHIVertexLayout::SetSlot(const VertexKey& key, const RHIVertexSlot& slot)
	{
		m_desc.streams[key].slot = slot;
	}

	RHIBuffer::RHIBuffer(const RHIBufferDesc& desc)
		: m_desc(desc)
	{}

	eDataUsage RHIBuffer::GetUsage() const
	{
		return m_desc.usage;
	}

	uint32_t RHIBuffer::GetSize() const
	{
		return m_desc.size;
	}

	const RHIBufferDesc& RHIBuffer::GetDesc() const
	{
		return m_desc;
	}

	void RHIBuffer::SetDesc(const RHIBufferDesc& desc)
	{
		m_desc = desc;
	}

	RHIBufferView::RHIBufferView()
		: m_buffer(nullptr)
		, m_size(0)
		, m_offset(0)
		, m_stride(0)
		, m_componentCount(0)
		, m_componentSize(0)
		, m_componentType(eDataType::eNone)
		, m_state()
	{
	}

	Memory::RefPtr<RHIBuffer> RHIBufferView::GetBuffer()
	{
		return m_buffer;
	}

	uint32_t RHIBufferView::GetSize() const
	{
		return m_size;
	}

	uint32_t RHIBufferView::GetOffset() const
	{
		return m_offset;
	}

	uint32_t RHIBufferView::GetStride() const
	{
		return m_stride;
	}

	uint32_t RHIBufferView::GetComponentCount() const
	{
		return m_componentCount;
	}

	uint32_t RHIBufferView::GetComponentSize() const
	{
		return m_componentSize;
	}

	eDataType RHIBufferView::GetComponentType() const
	{
		return m_componentType;
	}

	const RHIBufferState& RHIBufferView::GetBufferState() const
	{
		return m_state;
	}

	void RHIBufferView::SetSize(const uint32_t size)
	{
		m_size = size;
	}

	void RHIBufferView::SetOffset(const uint32_t offset)
	{
		m_offset = offset;
	}

	void RHIBufferView::SetStride(const uint32_t stride)
	{
		m_stride = stride;
	}

	void RHIBufferView::SetComponentSize(const uint32_t componentSize)
	{
		m_componentSize = componentSize;
	}

	void RHIBufferView::SetComponentCount(const uint32_t componentCount)
	{
		m_componentCount = componentCount;
	}

	void RHIBufferView::SetComponentType(const eDataType componentType)
	{
		m_componentType = componentType;
	}

	void RHIBufferView::SetBufferState(const RHIBufferState& state) const
	{
		m_state = state;
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

	eTextureFormat RHITexture::GetTextureFormat() const
	{
		return m_desc.textureFormat;
	}

	eTextureType RHITexture::GetTextureType() const
	{
		return m_desc.textureType;
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

	void RHITexture::SetTextureFormat(const eTextureFormat textureFormat)
	{
		m_desc.textureFormat = textureFormat;
	}

	void RHITexture::SetTextureType(const eTextureType textureType)
	{
		m_desc.textureType = textureType;
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

	eSamplerFilter RHISampler::GetMinFilter() const
	{
		return m_desc.minFilter;
	}

	eSamplerFilter RHISampler::GetMagFilter() const
	{
		return m_desc.magFilter;
	}

	eSamplerWrap RHISampler::GetWrapS() const
	{
		return m_desc.wrapS;
	}

	eSamplerWrap RHISampler::GetWrapT() const
	{
		return m_desc.wrapT;
	}

	eSamplerWrap RHISampler::GetWrapR() const
	{
		return m_desc.wrapR;
	}

	const RHISamplerDesc& RHISampler::GetDesc() const
	{
		return m_desc;
	}

	void RHISampler::SetDesc(const RHISamplerDesc& desc)
	{
		m_desc = desc;
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

		bool IsFloatType(const eDataType type)
		{
			return type == eDataType::eFloat || type == eDataType::eDouble;
		}

		bool IsIntegerType(const eDataType type)
		{
			switch (type)
			{
			case eDataType::eByte:
			case eDataType::eUByte:
			case eDataType::eShort:
			case eDataType::eUShort:
			case eDataType::eInt:
			case eDataType::eUInt:
				return true;

			default:
				return false;
			}
		}

		bool IsCompatible(const eDataType required, const RHIVertexAttribute& attribute)
		{
			if (IsFloatType(required))
			{
				return IsFloatType(attribute.componentType) || attribute.normalized;
			}

			if (IsIntegerType(required))
			{
				return IsIntegerType(attribute.componentType) && !attribute.normalized;
			}

			return false;
		}
	}

	void RHIStage::Clear()
	{
		m_consts.Clear();
		m_buffers.Clear();
		m_samplers.Clear();
		m_textures.Clear();
	}

	bool RHIStage::AddConst(const Name& name, const RHIStageConst& constant)
	{
		auto itr = m_consts.Find(name);
		if (itr == m_consts.End())
		{
			m_consts[name] = constant;
			return true;
		}

		auto& prevConst = itr->second;
		if (prevConst.binding != constant.binding)
		{
			LOGERROR() << "[RHI] Binding mismatch: " << name << ", prev: " << prevConst.binding << ", new: " << constant.binding;
			return false;
		}

		if ((prevConst.componentOffset != constant.componentOffset) ||
			(prevConst.componentRange != constant.componentRange) ||
			(prevConst.componentCount != constant.componentCount) ||
			(prevConst.componentType != constant.componentType))
		{
			LOGERROR() << "[RHI] Component mismatch: " << name;
			return false;
		}

		prevConst.stage |= constant.stage;
		return true;
	}

	bool RHIStage::AddBuffer(const Name& name, const RHIStageBuffer& buffer)
	{
		auto itr = m_buffers.Find(name);
		if (itr == m_buffers.End())
		{
			m_buffers[name] = buffer;
			return true;
		}

		auto& prevBuffer = itr->second;
		if (prevBuffer.binding != buffer.binding)
		{
			LOGERROR() << "[RHI] Binding mismatch: " << name << ", prev: " << prevBuffer.binding << ", new: " << buffer.binding;
			return false;
		}

		prevBuffer = buffer;
		return true;
	}

	bool RHIStage::AddSampler(const Name& name, const RHIStageSampler& sampler)
	{
		auto itr = m_samplers.Find(name);
		if (itr != m_samplers.End())
		{
			LOGWARN() << "[RHI] Sampler already exists: " << name;
			return false;
		}

		m_samplers[name] = sampler;
		return true;
	}
	
	bool RHIStage::AddTexture(const Name& name, const RHIStageTexture& texture)
	{
		auto itr = m_textures.Find(name);
		if (itr != m_textures.End())
		{
			LOGWARN() << "[RHI] Texture already exists: " << name;
			return false;
		}

		m_textures[name] = texture;
		return true;
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

	const RHIStageConst& RHIStage::GetConst(const Name& name) const
	{
		auto itr = m_consts.Find(name);
		if (itr != m_consts.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIStageConst empty = {};
			return empty;
		}
	}

	const RHIStageBuffer& RHIStage::GetBuffer(const Name& name) const
	{
		auto itr = m_buffers.Find(name);
		if (itr != m_buffers.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIStageBuffer empty = {};
			return empty;
		}
	}

	const RHIStageSampler& RHIStage::GetSampler(const Name& name) const
	{
		auto itr = m_samplers.Find(name);
		if (itr != m_samplers.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIStageSampler empty = {};
			return empty;
		}
	}

	const RHIStageTexture& RHIStage::GetTexture(const Name& name) const
	{
		auto itr = m_textures.Find(name);
		if (itr != m_textures.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIStageTexture empty = {};
			return empty;
		}
	}

	const RHIStageConsts& RHIStage::GetConsts() const
	{
		return m_consts;
	}

	const RHIStageBuffers& RHIStage::GetBuffers() const
	{
		return m_buffers;
	}

	const RHIStageSamplers& RHIStage::GetSamplers() const
	{
		return m_samplers;
	}

	const RHIStageTextures& RHIStage::GetTextures() const
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

	void RHIShader::Clear()
	{
		RHIStage::Clear();

		m_inputs.Clear();
		m_outputs.Clear();
	}

	bool RHIShader::AddInput(const VertexKey& key, const RHIStageLayout& input)
	{
		auto itr = m_inputs.Find(key);
		if (itr != m_inputs.End())
		{
			LOGWARN() << "[RHI] Input already exists: " << static_cast<int>(key.semantic) << ", " << static_cast<int>(key.semanticIndex);
			return false;
		}

		m_inputs[key] = input;
		return true;
	}

	bool RHIShader::AddOutput(const Name& name, const RHIStageLayout& output)
	{
		auto itr = m_outputs.Find(name);
		if (itr != m_outputs.End())
		{
			LOGWARN() << "[RHI] Output already exists: " << name;
			return false;
		}

		m_outputs[name] = output;
		return true;
	}

	bool RHIShader::HasInput(const VertexKey& key) const
	{
		return m_inputs.Find(key) != m_inputs.End();
	}

	bool RHIShader::HasOutput(const Name& name) const
	{
		return m_outputs.Find(name) != m_outputs.End();
	}

	const RHIStageLayout& RHIShader::GetInput(const VertexKey& key) const
	{
		auto itr = m_inputs.Find(key);
		if (itr != m_inputs.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIStageLayout empty = {};
			return empty;
		}
	}

	const RHIStageLayout& RHIShader::GetOutput(const Name& name) const
	{
		auto itr = m_outputs.Find(name);
		if (itr != m_outputs.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIStageLayout empty = {};
			return empty;
		}
	}

	const RHIStageInputs& RHIShader::GetInputs() const
	{
		return m_inputs;
	}

	const RHIStageOutputs& RHIShader::GetOutputs() const
	{
		return m_outputs;
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

		m_attribs.Clear();
		m_colorAttaches.Clear();
		m_depthStencilAttach = {};

		m_desc.vertexShader = nullptr;
		m_desc.geometryShader = nullptr;
		m_desc.hullShader = nullptr;
		m_desc.domainShader = nullptr;
		m_desc.pixelShader = nullptr;
		m_desc.computeShader = nullptr;
	}

	bool RHIPipeLine::AddShader(const Memory::RefPtr<const RHIShader>& shader)
	{
		if (!shader || (shader->GetState() != eResourceState::eReady))
		{
			LOGWARN() << "[RHI] Invalid shader";
			return false;
		}

		if (HasShader(shader->GetShaderType()))
		{
			LOGWARN() << "[RHI] Shader type already exists";
			return false;
		}

		for (const auto& [name, constant] : shader->GetConsts())
		{
			if (!AddConst(name, constant))
			{
				LOGWARN() << "[RHI] Failed to add constant: " << name;
				return false;
			}
		}

		for (const auto& [name, buffer] : shader->GetBuffers())
		{
			if (!AddBuffer(name, buffer))
			{
				LOGWARN() << "[RHI] Failed to add buffer: " << name;
				return false;
			}
		}
		
		for (const auto& [name, sampler] : shader->GetSamplers())
		{
			if (!AddSampler(name, sampler))
			{
				LOGWARN() << "[RHI] Failed to add sampler: " << name;
				return false;
			}
		}

		for (const auto& [name, texture] : shader->GetTextures())
		{
			if (!AddTexture(name, texture))
			{
				LOGWARN() << "[RHI] Failed to add texture: " << name;
				return false;
			}
		}

		switch (shader->GetShaderType())
		{
		case eShaderType::eVertex:		m_desc.vertexShader = shader;	break;
		case eShaderType::eGeometry:	m_desc.geometryShader = shader;	break;
		case eShaderType::eHull:		m_desc.hullShader = shader;		break;
		case eShaderType::eDomain:		m_desc.domainShader = shader;	break;
		case eShaderType::ePixel:		m_desc.pixelShader = shader;	break;
		case eShaderType::eCompute:		m_desc.computeShader = shader;	break;
		default:						
			LOGWARN() << "[RHI] Unknown shader type : " << static_cast<int>(shader->GetShaderType());
			return false;
		}

		return true;
	}

	bool RHIPipeLine::AddLayout(Memory::RefPtr<const RHIVertexLayout>& layout)
	{
		if (!layout || (layout->GetState() != eResourceState::eReady))
		{
			LOGWARN() << "[RHI] Invalid vertex layout";
			return false;
		}

		const auto& layoutDesc = layout->GetDesc();
		for (const auto& [key, stream] : layoutDesc.streams)
		{
			RHIStageAttrib attrib = {};
			attrib.location = stream.attribute.location;
			attrib.componentType = stream.attribute.componentType;
			attrib.componentCount = stream.attribute.componentCount;
			attrib.componentOffset = stream.attribute.componentOffset;
			attrib.normalized = stream.attribute.normalized;
			attrib.binding = stream.slot.binding;

			if (!AddAttrib(key, attrib))
			{
				LOGWARN() << "[RHI] Failed to add vertex stream: " << static_cast<int>(key.semantic) << ", " << static_cast<int>(key.semanticIndex);
				return false;
			}
		}

		return true;
	}

	bool RHIPipeLine::AddPass(Memory::RefPtr<const RHIRenderPass>& pass)
	{
		if (!pass || (pass->GetState() != eResourceState::eReady))
		{
			LOGWARN() << "[RHI] Invalid render pass";
			return false;
		}

		const auto& passDesc = pass->GetDesc();
		for (const auto& color : passDesc.colors)
		{
			if (!color.texture)
			{
				LOGWARN() << "[RHI] Invalid color texture";
				return false;
			}

			RHIStageAttach attach = {};
			attach.format = color.texture->GetPixelFormat();

			if (!AddColorAttach(color.location, attach))
			{
				LOGWARN() << "[RHI] Failed to add color attach: " << color.location;
				return false;
			}
		}

		if (passDesc.depthStencil.type != eAttachmentType::eNone)
		{
			if (!passDesc.depthStencil.texture)
			{
				LOGWARN() << "[RHI] Invalid depth stencil texture";
				return false;
			}

			RHIStageAttach attach = {};
			attach.format = passDesc.depthStencil.texture->GetPixelFormat();
			if (!AddDepthStencilAttach(attach))
			{
				LOGWARN() << "[RHI] Failed to add depth stencil attach";
				return false;
			}
		}

		return true;
	}

	bool RHIPipeLine::AddDepthStencilAttach(const RHIStageAttach& attach)
	{
		if (attach.format == ePixelFormat::eNone)
		{
			LOGWARN() << "[RHI] Invalid depth stencil attach format : " << static_cast<int>(attach.format);
			return false;
		}

		if (m_depthStencilAttach.format != ePixelFormat::eNone && m_depthStencilAttach != attach)
		{
			LOGWARN() << "[RHI] Depth stencil attach already exists with a different format : " << static_cast<int>(m_depthStencilAttach.format) << ", new: " << static_cast<int>(attach.format);
		}

		m_depthStencilAttach = attach;
		return true;
	}

	bool RHIPipeLine::AddAttrib(const VertexKey& key, const RHIStageAttrib& attrib)
	{
		auto itr = m_attribs.Find(key);
		if (itr != m_attribs.End())
		{
			LOGWARN() << "[RHI] Attrib already exists: " << static_cast<int>(key.semantic) << ", " << static_cast<int>(key.semanticIndex);
			return false;
		}

		m_attribs[key] = attrib;
		return true;
	}

	bool RHIPipeLine::AddColorAttach(const uint32_t location, const RHIStageAttach& attach)
	{
		auto itr = m_colorAttaches.Find(location);
		if (itr != m_colorAttaches.End())
		{
			LOGWARN() << "[RHI] Color attach already exists: " << location;
			return false;
		}

		m_colorAttaches[location] = attach;
		return true;
	}

	bool RHIPipeLine::HasAttrib(const VertexKey& key) const
	{
		return m_attribs.Find(key) != m_attribs.End();
	}

	bool RHIPipeLine::HasColorAttach(const uint32_t location) const
	{
		return m_colorAttaches.Find(location) != m_colorAttaches.End();
	}

	bool RHIPipeLine::HasDepthStencilAttach() const
	{
		return m_depthStencilAttach.format != eTextureFormat::eNone;
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

	const RHIStageAttrib& RHIPipeLine::GetAttrib(const VertexKey& key) const
	{
		auto itr = m_attribs.Find(key);
		if (itr != m_attribs.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIStageAttrib empty = {};
			return empty;
		}
	}

	const RHIStageAttach& RHIPipeLine::GetColorAttach(const uint32_t location) const
	{
		auto itr = m_colorAttaches.Find(location);
		if (itr != m_colorAttaches.End())
		{
			return itr->second;
		}
		else
		{
			static const RHIStageAttach empty = {};
			return empty;
		}
	}

	const RHIStageAttach& RHIPipeLine::GetDepthStencilAttach() const
	{
		return m_depthStencilAttach;
	}

	const RHIStageAttribs& RHIPipeLine::GetAttribs() const
	{
		return m_attribs;
	}

	const RHIStageAttaches& RHIPipeLine::GetColorAttaches() const
	{
		return m_colorAttaches;
	}

	RHIRenderPass::RHIRenderPass(const RHIRenderPassDesc& desc)
		: m_desc(desc)
	{
	}

	const RHIRenderPassDesc& RHIRenderPass::GetDesc() const
	{
		return m_desc;
	}

	bool RHIRenderPass::HasColorAttach(const uint32_t location) const
	{
		for (const auto& color : m_desc.colors)
		{
			if (color.location == location)
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