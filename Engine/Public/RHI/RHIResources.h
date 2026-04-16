#ifndef __WTR_RHIRESOURCES_H__
#define __WTR_RHIRESOURCES_H__

#include <RHI/RHIDescriptions.h>

#include <atomic>
#include <cstdint>

namespace wtr
{
	enum class eResourceState : uint8_t
	{
		eNone		= 0x00,
		eError		= 0x01,
		eLoaded		= 0x10,
		eDirty		= 0x20,
		eReady		= 0x40,
		eAll		= 0xF0,
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
			const eResourceState GetState() const;

		private :
			std::atomic<eResourceState> m_state = eResourceState::eNone;
	};

	class RHIBuffer : public RHIResource 
	{
		public :
			RHIBuffer(const RHIBufferDesc& desc);
			virtual ~RHIBuffer() = default;

		public :
			const eBufferType GetBufferType() const;
			const eDataAccess GetAccessType() const;
			const eDataType GetComponentType() const;
			const uint32_t GetNumComponents() const;
			const uint32_t GetCount() const;
			const uint32_t GetSize() const;
			const uint32_t GetStride() const;

			void SetBufferType(const eBufferType bufferType);
			void SetAccessType(const eDataAccess accessType);
			void SetComponentType(const eDataType componentType);
			void SetNumComponents(const uint32_t numComponents);
			void SetCount(const uint32_t count);
			void SetSize(const uint32_t size);
			void SetStride(const uint32_t stride);

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
			const size_t GetNumAttributes() const;

			void SetDesc(const RHIVertexLayoutDesc& desc);

		protected :
			RHIVertexLayoutDesc m_desc;
	};

	class RHITexture : public RHIResource
	{
		public :
			RHITexture(const RHITextureDesc& desc);
			virtual ~RHITexture() = default;

		public :
			const uint32_t GetWidth() const;
			const uint32_t GetHeight() const;
			const uint32_t GetDepth() const;
			const uint32_t GetMipLevels() const;
			const uint32_t GetSampleCount() const;
			const ePixelFormat GetPixelFormat() const;
			const eTextureUsage GetTextureUsage() const;
			const eTextureType GetTextureType() const;

			void SetWidth(const uint32_t width);
			void SetHeight(const uint32_t height);
			void SetDepth(const uint32_t depth);
			void SetMipLevels(const uint32_t mipLevels);
			void SetSampleCount(const uint32_t sampleCount);
			void SetPixelFormat(const ePixelFormat pixelFormat);
			void SetTextureUsage(const eTextureUsage textureUsage);
			void SetTextureType(const eTextureType textureType);

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
			const eFilterMode GetMinFilter() const;
			const eFilterMode GetMagFilter() const;
			const eFilterMode GetMipFilter() const;
			const eWrapMode GetWrapS() const;
			const eWrapMode GetWrapT() const;
			const eWrapMode GetWrapR() const;

			void SetMinFilter(const eFilterMode minFilter);
			void SetMagFilter(const eFilterMode magFilter);
			void SetMipFilter(const eFilterMode mipFilter);
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
		const eShaderType GetShaderType() const;

	private :
		RHIShaderDesc m_desc;
	};

	class RHIPipeLine : public RHIResource
	{
		public :
			RHIPipeLine(const RHIPipeLineDesc& desc);
			virtual ~RHIPipeLine() = default;

		public :
			const RHIClearState GetClearState() const;
			const RHIColorState GetColorState() const;
			const RHIDepthState GetDepthState() const;
			const RHIStencilState GetStencilState() const;
			const RHIBlendState GetBlendState() const;
			const RHIRasterizerState GetRasterizerState() const;

			void SetClearState(const RHIClearState clear);
			void SetColorState(const RHIColorState color);
			void SetDepthState(const RHIDepthState depth);
			void SetStencilState(const RHIStencilState stencil);
			void SetBlendState(const RHIBlendState blend);
			void SetRasterizerState(const RHIRasterizerState rasterizer);

			void SetDesc(const RHIPipeLineDesc& desc);

			void AddSlot(const std::string& name, const RHIResourceBinding& binding);
			bool HasSlot(const std::string& name) const;

			size_t GetSlotCount() const;
			const RHIResourceBinding GetBindingSlot(const std::string& name) const;

		private :
			RHIPipeLineDesc m_desc;
			wtr::HashMap<std::string, RHIResourceBinding> m_slots;
	};
};

#endif // __WTR_RHIRESOURCES_H__