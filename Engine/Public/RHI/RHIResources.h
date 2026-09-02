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
			eBufferType GetBufferType() const;
			eBufferTransfer GetBufferTransfer() const;
			eDataAccess GetAccessType() const;
			eDataType GetDataType() const;
			uint32_t GetNumComponents() const;
			uint32_t GetStride() const;
			uint32_t GetSize() const;
			const RHIBufferDesc& GetDesc() const;

			void SetBufferType(const eBufferType bufferType);
			void SetBufferTransfer(const eBufferTransfer bufferTransfer);
			void SetAccessType(const eDataAccess accessType);
			void SetDataType(const eDataType dataType);
			void SetNumComponents(const uint32_t numComponents);
			void SetStride(const uint32_t stride);
			void SetSize(const uint32_t size);

			void SetDesc(const RHIBufferDesc& desc);

		protected :
			RHIBufferDesc m_desc;
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
			eTextureUsage GetTextureUsage() const;
			eTextureType GetTextureType() const;
			eDataType GetDataType() const;
			const RHITextureDesc& GetDesc() const;

			void SetWidth(const uint32_t width);
			void SetHeight(const uint32_t height);
			void SetDepth(const uint32_t depth);
			void SetMipLevels(const uint32_t mipLevels);
			void SetSampleCount(const uint32_t sampleCount);
			void SetPixelFormat(const ePixelFormat pixelFormat);
			void SetTextureUsage(const eTextureUsage textureUsage);
			void SetTextureType(const eTextureType textureType);
			void SetDataType(const eDataType dataType);

			void SetDesc(const RHITextureDesc& desc);

		protected :
			RHITextureDesc m_desc;
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

	class RHIShader : public RHIResource
	{
	public :
		RHIShader(const RHIShaderDesc& desc);
		virtual ~RHIShader() = default;

	public :
		eShaderType GetShaderType() const;
		const RHIShaderDesc& GetDesc() const;

		void AddInput(const std::string& name, const RHIShaderVariable& input);
		void AddOutput(const std::string& name, const RHIShaderVariable& output);
		void AddBinding(const std::string& name, const RHIShaderBinding& binding);

		const RHIShaderVariable& GetInput(const std::string& name) const;
		const RHIShaderVariable& GetOutput(const std::string& name) const;
		const RHIShaderBinding& GetBinding(const std::string& name) const;
		
		const RHIShaderVariables& GetInputs() const;
		const RHIShaderVariables& GetOutputs() const;
		const RHIShaderBindings& GetBindings() const;

	private :
		RHIShaderDesc m_desc;
		RHIShaderVariables m_inputs;
		RHIShaderVariables m_outputs;
		RHIShaderBindings m_bindings;
	};

	class RHIPipeLine : public RHIResource
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

		void SetDesc(const RHIPipeLineDesc& desc);
		void SetColorState(const RHIColorState& color);
		void SetDepthState(const RHIDepthState& depth);
		void SetStencilState(const RHIStencilState& stencil);
		void SetBlendState(const RHIBlendState& blend);
		void SetRasterizerState(const RHIRasterizerState& rasterizer);

		void AddBinding(const std::string& name, const RHIShaderBinding& binding);
		bool HasBinding(const std::string& name) const;
		bool HasBinding(const eResourceSlot slot) const;

		size_t GetBindingCount() const;
		const RHIShaderBinding& GetBinding(const std::string& name) const;
		const RHIShaderBinding& GetBinding(const eResourceSlot slot) const;

	private :
		RHIPipeLineDesc m_desc;
		RHIShaderBindings m_bindings;
	};

	class RHIGraphicsPipeLine : public RHIPipeLine
	{

	};

	class RHIComputePipeLine : public RHIPipeLine
	{

	};

	class RHIRenderTarget : public RHIResource
	{
	public :
		RHIRenderTarget(const RHIRenderTargetDesc& desc);
		virtual ~RHIRenderTarget() = default;

	public :
		const RHIRenderTargetDesc& GetDesc() const;

		bool HasColorAttach(const uint32_t slot) const;
		bool HasDepthStencilAttach() const;

		size_t GetColorAttachCount() const;

	private :
		RHIRenderTargetDesc m_desc;
	};

	template<typename T>
	struct RHIPendingResource
	{
		Memory::RefPtr<T> resource;
		const size_t frame;

		RHIPendingResource(Memory::RefPtr<T> resource, const size_t frame)
			: resource(resource)
			, frame(frame)
		{}
	};
};

#endif // __WTR_RHIRESOURCES_H__