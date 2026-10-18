#ifndef __WTR_RHIRESOURCES_H__
#define __WTR_RHIRESOURCES_H__

#include <RHI/RHIDescriptions.h>
#include <Container/include/HashMap.h>
#include <Memory/include/Pointer/RefPtr.h>

#include <atomic>
#include <cstdint>

namespace wtr
{
	enum class eResourceState : uint8_t
	{
		eNone		= 0x00,
		eReady		= 0x01,
		eError		= 0xF0,
		eAll		= 0xFF,
	};

	eResourceState operator|(const eResourceState lhs, const eResourceState rhs);
	eResourceState operator&(const eResourceState lhs, const eResourceState rhs);
	eResourceState operator^(const eResourceState lhs, const eResourceState rhs);
	eResourceState operator~(const eResourceState state);
	eResourceState& operator|=(eResourceState& lhs, const eResourceState rhs);
	eResourceState& operator&=(eResourceState& lhs, const eResourceState rhs);
	eResourceState& operator^=(eResourceState& lhs, const eResourceState rhs);

	class RHIResource
	{
		public :
			RHIResource() = default;
			virtual ~RHIResource() = default;

		public :
			virtual eResourceType GetResourceType() const = 0;

		public :
			void* GetRawBuffer();
			const void* GetRawBuffer() const;

			void SetState(const eResourceState eState);
			eResourceState GetState() const;

		private :
			std::atomic<eResourceState> m_state = eResourceState::eNone;
	};

	class RHIBuffer : public RHIResource
	{
		public :
			RHIBuffer(const RHIBufferDesc& desc);
			virtual ~RHIBuffer() = default;

		public :
			eDataUsage GetDataUsage() const;
			eDataType GetDataType() const;
			uint32_t GetNumComponents() const;
			uint32_t GetStride() const;
			uint32_t GetSize() const;
			const RHIBufferDesc& GetDesc() const;
			const RHIBufferState& GetBufferState() const;

			void SetDataUsage(const eDataUsage dataUsage);
			void SetDataType(const eDataType dataType);
			void SetNumComponents(const uint32_t numComponents);
			void SetStride(const uint32_t stride);
			void SetSize(const uint32_t size);

			void SetDesc(const RHIBufferDesc& desc);

		private :
			friend class RHICommandList;
			void SetBufferState(const RHIBufferState& state) const;

		protected :
			RHIBufferDesc m_desc;

		private :
			mutable RHIBufferState m_bufferState;
	};

	class RHIVertexLayout : public RHIResource
	{
		public :
			RHIVertexLayout(const RHIVertexLayoutDesc& desc);
			virtual ~RHIVertexLayout() = default;

		public :
			size_t GetNumLayout() const;
			RHIVertexAttribute GetAttribute(const VertexKey& key) const;
			RHIVertexBinding GetBinding(const VertexKey& key) const;
			const RHIVertexLayoutDesc& GetDesc() const;

			void SetDesc(const RHIVertexLayoutDesc& desc);
			void SetAttribute(const VertexKey& key, const RHIVertexAttribute& attribute);
			void SetBinding(const VertexKey& key, const RHIVertexBinding& binding);

		protected :
			RHIVertexLayoutDesc m_desc;
	};

	class RHITexture : public RHIResource
	{
		public :
			RHITexture(const RHITextureDesc& desc);
			virtual ~RHITexture() = default;

		public :
			uint32_t GetWidth() const;
			uint32_t GetHeight() const;
			uint32_t GetDepth() const;
			uint32_t GetMipLevels() const;
			uint32_t GetSampleCount() const;
			ePixelFormat GetPixelFormat() const;
			eTextureDimension GetTextureDimension() const;
			eDataType GetDataType() const;
			const RHITextureDesc& GetDesc() const;
			const RHITextureState& GetTextureState() const;

			void SetWidth(const uint32_t width);
			void SetHeight(const uint32_t height);
			void SetDepth(const uint32_t depth);
			void SetMipLevels(const uint32_t mipLevels);
			void SetSampleCount(const uint32_t sampleCount);
			void SetPixelFormat(const ePixelFormat pixelFormat);
			void SetTextureDimension(const eTextureDimension textureDimension);
			void SetDataType(const eDataType dataType);

			void SetDesc(const RHITextureDesc& desc);

		private :
			friend class RHICommandList;
			void SetTextureState(const RHITextureState& state) const;

		protected :
			RHITextureDesc m_desc;

		private :
			mutable RHITextureState m_textureState;
	};

	class RHISampler : public RHIResource
	{
		public :
			RHISampler(const RHISamplerDesc& desc);
			virtual ~RHISampler() = default;

		public :
			eFilterMode GetMinFilter() const;
			eFilterMode GetMagFilter() const;
			eWrapMode GetWrapS() const;
			eWrapMode GetWrapT() const;
			eWrapMode GetWrapR() const;
			const RHISamplerDesc& GetDesc() const;

			void SetMinFilter(const eFilterMode minFilter);
			void SetMagFilter(const eFilterMode magFilter);
			void SetWrapS(const eWrapMode wrapS);
			void SetWrapT(const eWrapMode wrapT);
			void SetWrapR(const eWrapMode wrapR);

			void SetDesc(const RHISamplerDesc& desc);

		protected :
			RHISamplerDesc m_desc;
	};

	class RHIStage : public RHIResource
	{
	public :
		RHIStage() = default;
		virtual ~RHIStage() = default;

	public :
		void Clear();

		void AddAttrib(const VertexKey& key, const RHIShaderLayout& attrib);
		void AddAttach(const Name& name, const RHIShaderLayout& attach);

		void AddConst(const Name& name, const RHIShaderConst& constant);
		void AddBuffer(const Name& name, const RHIShaderBuffer& buffer);
		void AddSampler(const Name& name, const RHIShaderSampler& sampler);
		void AddTexture(const Name& name, const RHIShaderTexture& texture);

		bool HasAttrib(const VertexKey& key) const;
		bool HasAttach(const Name& name) const;
		bool HasConst(const Name& name) const;
		bool HasBuffer(const Name& name) const;
		bool HasSampler(const Name& name) const;
		bool HasTexture(const Name& name) const;

		const RHIShaderLayout& GetAttrib(const VertexKey& key) const;
		const RHIShaderLayout& GetAttach(const Name& name) const;
		const RHIShaderConst& GetConst(const Name& name) const;
		const RHIShaderBuffer& GetBuffer(const Name& name) const;
		const RHIShaderSampler& GetSampler(const Name& name) const;
		const RHIShaderTexture& GetTexture(const Name& name) const;

		const RHIShaderAttribs& GetAttribs() const;
		const RHIShaderAttaches& GetAttaches() const;
		const RHIShaderConsts& GetConsts() const;
		const RHIShaderBuffers& GetBuffers() const;
		const RHIShaderSamplers& GetSamplers() const;
		const RHIShaderTextures& GetTextures() const;

	protected :
		RHIShaderAttribs m_attribs;
		RHIShaderAttaches m_attaches;
		RHIShaderConsts m_consts;
		RHIShaderBuffers m_buffers;
		RHIShaderSamplers m_samplers;
		RHIShaderTextures m_textures;
	};

	class RHIShader : public RHIStage
	{
	public :
		RHIShader(const RHIShaderDesc& desc);
		virtual ~RHIShader() = default;

	public :
		eShaderType GetShaderType() const;
		const RHIShaderDesc& GetDesc() const;

	private :
		RHIShaderDesc m_desc;
	};

	class RHIPipeLine : public RHIStage
	{
	public :
		RHIPipeLine(const RHIPipeLineDesc& desc);
		virtual ~RHIPipeLine() = default;

	public :
		RHIColorState GetColorState() const;
		RHIDepthState GetDepthState() const;
		RHIStencilState GetStencilState() const;
		RHIBlendState GetBlendState() const;
		RHIRasterizerState GetRasterizerState() const;
		const RHIPipeLineDesc& GetDesc() const;

		void Clear();
		bool AddShader(const Memory::RefPtr<const RHIShader>& shader);
		bool HasShader(const eShaderType shaderType) const;

	private :
		RHIPipeLineDesc m_desc;
	};

	class RHIGraphicsPipeLine : public RHIPipeLine
	{
	public :
		RHIGraphicsPipeLine(const RHIPipeLineDesc& desc)
			: RHIPipeLine(desc)
		{}
	};

	class RHIComputePipeLine : public RHIPipeLine
	{
	public :
		RHIComputePipeLine(const RHIPipeLineDesc& desc)
			: RHIPipeLine(desc)
		{}
	};

	class RHIRenderPass : public RHIResource
	{
	public :
		RHIRenderPass(const RHIRenderPassDesc& desc);
		virtual ~RHIRenderPass() = default;

	public :
		const RHIRenderPassDesc& GetDesc() const;

		bool HasColorAttach(const uint32_t slot) const;
		bool HasDepthStencilAttach() const;

		size_t GetColorAttachCount() const;

	private :
		RHIRenderPassDesc m_desc;
	};
};

#endif // __WTR_RHIRESOURCES_H__