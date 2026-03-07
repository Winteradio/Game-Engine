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
		eLoaded		= 0x01,
		eReady		= 0x02,
		eAll		= 0x0F,
		eError		= 0x10,
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
			virtual const void* GetRawBuffer() const;
			
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
			virtual const void* GetRawBuffer() const;

			const eBufferType GetType() const;
			const eDataAccess GetAccessType() const;
			const uint32_t GetSize() const;
			const uint32_t GetStride() const;

		protected :
			RHIBufferDesc m_desc;
	};

	class RHITexture : public RHIResource
	{
		public :
			RHITexture(const RHITextureDesc& desc);
			virtual ~RHITexture() = default;

		public :
			virtual const void* GetRawBuffer() const;

			const uint32_t GetWidth() const;
			const uint32_t GetHeight() const;
			const uint32_t GetDepth() const;
			const uint32_t GetMipLevels() const;
			const uint32_t GetSampleCount() const;
			const ePixelFormat GetPixelFormat() const;
			const eTextureUsage GetTextureUsage() const;

		protected :
			RHITextureDesc m_desc;
	};

	class RHISampler : public RHIResource
	{
		public :
			RHISampler(const RHISamplerDesc& desc);
			virtual ~RHISampler() = default;

		public :
			virtual const void* GetRawBuffer() const;

			const eFilterMode GetMinFilter() const;
			const eFilterMode GetMagFilter() const;
			const eFilterMode GetMipFilter() const;
			const eWrapMode GetWrapS() const;
			const eWrapMode GetWrapT() const;
			const eWrapMode GetWrapR() const;

		protected :
			RHISamplerDesc m_desc;
	};

	class RHIShader : public RHIResource
	{
		public :
			RHIShader() = default;
			virtual ~RHIShader() = default;
	};

	class RHIVertexShader : public RHIShader
	{
		public : 
			RHIVertexShader(const RHIVertexShaderDesc& desc);
			virtual ~RHIVertexShader() = default;

		public :
			virtual const void* GetRawBuffer() const;

		private :
			RHIVertexShaderDesc m_dsec;
	};

	class RHIGeometryShader : public RHIShader
	{
		public :
			RHIGeometryShader(const RHIGeometryShaderDesc& desc);
			virtual ~RHIGeometryShader() = default;

		public :
			virtual const void* GetRawBuffer() const;

		private :
			RHIGeometryShaderDesc m_desc;
	};

	class RHIPixelShader : public RHIShader
	{
		public :
			RHIPixelShader(const RHIPixelShaderDesc& desc);
			virtual ~RHIPixelShader() = default;

		public :
			virtual const void* GetRawBuffer() const;

		private :
			RHIPixelShaderDesc m_desc;
	};

	class RHIComputeShader : public RHIShader
	{
		public :
			RHIComputeShader(const RHIComputeShaderDesc& desc);
			virtual ~RHIComputeShader() = default;

		public :
			virtual const void* GetRawBuffer() const;

		private :
			RHIComputeShaderDesc m_desc;
	};

	class RHIPipeLine : public RHIResource
	{
		public :
			RHIPipeLine(const RHIPipeLineDesc& desc);
			virtual ~RHIPipeLine() = default;

		public :
			virtual const void* GetRawBuffer() const;

			const RHIClearState GetClearState() const;
			const RHIColorState GetColorState() const;
			const RHIDepthState GetDepthState() const;
			const RHIStencilState GetStencilState() const;
			const RHIBlendState GetBlendState() const;
			const RHIRasterizerState GetRasterizerState() const;

		protected :
			RHIPipeLineDesc m_desc;
	};
};

#endif // __WTR_RHIRESOURCES_H__