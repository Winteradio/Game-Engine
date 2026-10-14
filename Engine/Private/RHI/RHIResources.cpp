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

	RHIShader::RHIShader(const RHIShaderDesc& desc)
		: m_desc(desc)
		, m_inputs()
		, m_outputs()
		, m_consts()
		, m_buffers()
		, m_samplers()
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

	void RHIShader::AddInput(const std::string& name, const RHIShaderAttrib& input)
	{
		if (name.empty())
		{
			return;
		}

		m_inputs[name] = input;
	}

	void RHIShader::AddOutput(const std::string& name, const RHIShaderAttrib& output)
	{
		if (name.empty())
		{
			return;
		}

		m_outputs[name] = output;
	}

	void RHIShader::AddConst(const std::string& name, const RHIShaderConst& constant)
	{
		if (name.empty())
		{
			return;
		}

		m_consts[name] = constant;
	}

	void RHIShader::AddBuffer(const std::string& name, const RHIShaderBuffer& buffer)
	{
		if (name.empty())
		{
			return;
		}

		m_buffers[name] = buffer;
	}

	void RHIShader::AddSampler(const std::string& name, const RHIShaderSampler& sampler)
	{
		if (name.empty())
		{
			return;
		}

		m_samplers[name] = sampler;
	}

	const RHIShaderAttribs& RHIShader::GetInputs() const
	{
		return m_inputs;
	}

	const RHIShaderAttribs& RHIShader::GetOutputs() const
	{
		return m_outputs;
	}

	const RHIShaderConsts& RHIShader::GetConsts() const
	{
		return m_consts;
	}

	const RHIShaderBuffers& RHIShader::GetBuffers() const
	{
		return m_buffers;
	}

	const RHIShaderSamplers& RHIShader::GetSamplers() const
	{
		return m_samplers;
	}

	namespace
	{
		template<typename Map>
		const typename Map::MappedType& FindOrEmpty(const Map& map, const typename Map::KeyType& key)
		{
			auto itr = map.Find(key);
			if (itr != map.End())
			{
				return itr->second;
			}
			else
			{
				static const typename Map::MappedType empty = {};
				return empty;
			}
		}
	};

	RHIPipeLine::RHIPipeLine(const RHIPipeLineDesc& desc)
		: m_desc(desc)
		, m_inputs()
		, m_outputs()
		, m_consts()
		, m_buffers()
		, m_samplers()
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

	void RHIPipeLine::SetColorState(const RHIColorState& color)
	{
		m_desc.color = color;
	}

	void RHIPipeLine::SetDepthState(const RHIDepthState& depth)
	{
		m_desc.depth = depth;
	}

	void RHIPipeLine::SetStencilState(const RHIStencilState& stencil)
	{
		m_desc.stencil = stencil;
	}

	void RHIPipeLine::SetBlendState(const RHIBlendState& blend)
	{
		m_desc.blend = blend;
	}

	void RHIPipeLine::SetRasterizerState(const RHIRasterizerState& rasterizer)
	{
		m_desc.rasterizer = rasterizer;
	}

	void RHIPipeLine::SetDesc(const RHIPipeLineDesc& desc)
	{
		m_desc = desc;
	}

	void RHIPipeLine::AddShader(const RHIShader& shader)
	{
		const eShaderType shaderType = shader.GetShaderType();

		// 정점 입력은 정점 셰이더의 입력만, 어태치먼트 출력은 픽셀 셰이더의 출력만 본다 (나머지는 단계 사이 varying)
		if (shaderType == eShaderType::eVertex)
		{
			for (const auto& [name, input] : shader.GetInputs())
			{
				// gl_VertexID 같은 내장 변수는 VertexKey가 없음
				const VertexKey key = RenderUtils::GetVertexKey(name);
				if (key.semantic == eVertexSemantic::eNone)
				{
					continue;
				}

				AddInput(key, input);
			}
		}
		else if (shaderType == eShaderType::ePixel)
		{
			for (const auto& [name, output] : shader.GetOutputs())
			{
				AddOutput(name, output);
			}
		}

		for (const auto& [name, constant] : shader.GetConsts())
		{
			AddConst(name, constant);
		}

		for (const auto& [name, buffer] : shader.GetBuffers())
		{
			AddBuffer(name, buffer);
		}

		for (const auto& [name, sampler] : shader.GetSamplers())
		{
			AddSampler(name, sampler);
		}
	}

	void RHIPipeLine::AddInput(const VertexKey& key, const RHIShaderAttrib& input)
	{
		auto itr = m_inputs.Find(key);
		if (itr != m_inputs.End() && itr->second.location != input.location)
		{
			LOGWARN() << "[RHI] Input location mismatch — existing : " << itr->second.location << ", new : " << input.location;
			return;
		}

		m_inputs[key] = input;
	}

	void RHIPipeLine::AddOutput(const std::string& name, const RHIShaderAttrib& output)
	{
		if (name.empty())
		{
			return;
		}

		auto itr = m_outputs.Find(name);
		if (itr != m_outputs.End() && itr->second.location != output.location)
		{
			LOGWARN() << "[RHI] Output location mismatch for '" << name << "' — existing : " << itr->second.location << ", new : " << output.location;
			return;
		}

		m_outputs[name] = output;
	}

	void RHIPipeLine::AddConst(const std::string& name, const RHIShaderConst& constant)
	{
		if (name.empty())
		{
			return;
		}

		auto itr = m_consts.Find(name);
		if (itr == m_consts.End())
		{
			m_consts[name] = constant;
			return;
		}

		RHIShaderConst& existing = itr->second;
		if (existing.location != constant.location || existing.offset != constant.offset)
		{
			LOGWARN() << "[RHI] Const location mismatch for '" << name << "' — existing : " << existing.location << ", new : " << constant.location;
			return;
		}

		// 여러 셰이더 단계에서 같은 상수를 쓰면 stage만 합친다
		existing.stage = existing.stage | constant.stage;
	}

	void RHIPipeLine::AddBuffer(const std::string& name, const RHIShaderBuffer& buffer)
	{
		if (name.empty())
		{
			return;
		}

		auto itr = m_buffers.Find(name);
		if (itr == m_buffers.End())
		{
			m_buffers[name] = buffer;
			return;
		}

		RHIShaderBuffer& existing = itr->second;
		if (existing.binding != buffer.binding || existing.usage != buffer.usage)
		{
			LOGWARN() << "[RHI] Buffer binding mismatch for '" << name << "' — existing : " << existing.binding << ", new : " << buffer.binding;
			return;
		}

		// 여러 셰이더 단계에서 같은 버퍼를 쓰면 state / stage를 합친다
		existing.Merge(buffer);
	}

	void RHIPipeLine::AddSampler(const std::string& name, const RHIShaderSampler& sampler)
	{
		if (name.empty())
		{
			return;
		}

		auto itr = m_samplers.Find(name);
		if (itr == m_samplers.End())
		{
			m_samplers[name] = sampler;
			return;
		}

		RHIShaderSampler& existing = itr->second;
		if (existing.binding != sampler.binding || existing.usage != sampler.usage)
		{
			LOGWARN() << "[RHI] Sampler binding mismatch for '" << name << "' — existing : " << existing.binding << ", new : " << sampler.binding;
			return;
		}

		// 여러 셰이더 단계에서 같은 텍스처를 쓰면 state / stage를 합친다
		existing.Merge(sampler);
	}

	bool RHIPipeLine::HasInput(const VertexKey& key) const
	{
		return m_inputs.Find(key) != m_inputs.End();
	}

	bool RHIPipeLine::HasOutput(const std::string& name) const
	{
		return m_outputs.Find(name) != m_outputs.End();
	}

	bool RHIPipeLine::HasConst(const std::string& name) const
	{
		return m_consts.Find(name) != m_consts.End();
	}

	bool RHIPipeLine::HasConst(const eResourceSlot slot) const
	{
		return HasConst(RenderUtils::GetSlotName(slot));
	}

	bool RHIPipeLine::HasBuffer(const std::string& name) const
	{
		return m_buffers.Find(name) != m_buffers.End();
	}

	bool RHIPipeLine::HasBuffer(const eResourceSlot slot) const
	{
		return HasBuffer(RenderUtils::GetSlotName(slot));
	}

	bool RHIPipeLine::HasSampler(const std::string& name) const
	{
		return m_samplers.Find(name) != m_samplers.End();
	}

	bool RHIPipeLine::HasSampler(const eResourceSlot slot) const
	{
		return HasSampler(RenderUtils::GetSlotName(slot));
	}

	const RHIShaderAttrib& RHIPipeLine::GetInput(const VertexKey& key) const
	{
		return FindOrEmpty(m_inputs, key);
	}

	const RHIShaderAttrib& RHIPipeLine::GetOutput(const std::string& name) const
	{
		return FindOrEmpty(m_outputs, name);
	}

	const RHIShaderConst& RHIPipeLine::GetConst(const std::string& name) const
	{
		return FindOrEmpty(m_consts, name);
	}

	const RHIShaderConst& RHIPipeLine::GetConst(const eResourceSlot slot) const
	{
		return GetConst(RenderUtils::GetSlotName(slot));
	}

	const RHIShaderBuffer& RHIPipeLine::GetBuffer(const std::string& name) const
	{
		return FindOrEmpty(m_buffers, name);
	}

	const RHIShaderBuffer& RHIPipeLine::GetBuffer(const eResourceSlot slot) const
	{
		return GetBuffer(RenderUtils::GetSlotName(slot));
	}

	const RHIShaderSampler& RHIPipeLine::GetSampler(const std::string& name) const
	{
		return FindOrEmpty(m_samplers, name);
	}

	const RHIShaderSampler& RHIPipeLine::GetSampler(const eResourceSlot slot) const
	{
		return GetSampler(RenderUtils::GetSlotName(slot));
	}

	const RHIShaderInputs& RHIPipeLine::GetInputs() const
	{
		return m_inputs;
	}

	const RHIShaderAttribs& RHIPipeLine::GetOutputs() const
	{
		return m_outputs;
	}

	const RHIShaderConsts& RHIPipeLine::GetConsts() const
	{
		return m_consts;
	}

	const RHIShaderBuffers& RHIPipeLine::GetBuffers() const
	{
		return m_buffers;
	}

	const RHIShaderSamplers& RHIPipeLine::GetSamplers() const
	{
		return m_samplers;
	}

	RHIBufferState RHIPipeLine::GetInputState(const VertexKey& key) const
	{
		if (!HasInput(key))
		{
			return {};
		}

		return RHIBufferState(eDataState::eRead, eDataStage::eVertexInput, eBufferUsage::eVertex);
	}

	RHIBufferState RHIPipeLine::GetIndexState() const
	{
		// 컴퓨트 파이프라인은 인덱스 버퍼를 쓰지 않음
		if (!m_desc.vertexShader)
		{
			return {};
		}

		return RHIBufferState(eDataState::eRead, eDataStage::eVertexInput, eBufferUsage::eIndex);
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