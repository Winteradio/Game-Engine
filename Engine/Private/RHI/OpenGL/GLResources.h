#ifndef __WTR_GLRESOURCES_H__
#define __WTR_GLRESOURCES_H__

#include <RHI/RHIResources.h>

namespace wtr
{
	class GLBuffer : public RHIBuffer
	{
		public : 
			GLBuffer(const RHIBufferDesc& desc);
			virtual ~GLBuffer();

		public :
			const void* GetRawBuffer() const final;
			const uint32_t GetID() const;

		private :
			uint32_t m_bufferID = 0;
	};

	class GLTexture : public RHITexture
	{
		public :
			GLTexture(const RHITextureDesc& desc);
			virtual ~GLTexture();

		public :
			const void* GetRawBuffer() const final;
			const uint32_t GetID() const;

		private :
			uint32_t m_textureID = 0;
	};

	class GLSampler : public RHISampler
	{
		public :
			GLSampler(const RHISamplerDesc& desc);
			virtual ~GLSampler();

		public :
			const void* GetRawBuffer() const final;
			const uint32_t GetID() const;

		private :
			uint32_t m_samplerID = 0;
	};

	class GLVertexShader : public RHIVertexShader
	{
		public :
			GLVertexShader(const RHIVertexShaderDesc& desc);
			virtual ~GLVertexShader();

		public :
			const void* GetRawBuffer() const final;
			const uint32_t GetID() const;

		private :
			uint32_t m_shaderID = 0;
	};

	class GLGeometryShader : public RHIGeometryShader
	{
		public :
			GLGeometryShader(const RHIGeometryShaderDesc& desc);
			virtual ~GLGeometryShader();

		public :
			const void* GetRawBuffer() const final;
			const uint32_t GetID() const;

		private :
			uint32_t m_shaderID = 0;
	};

	class GLPixelShader : public RHIPixelShader
	{
		public :
			GLPixelShader(const RHIPixelShaderDesc& desc);
			virtual ~GLPixelShader();

		public :
			const void* GetRawBuffer() const final;
			const uint32_t GetID() const;

		private :
			uint32_t m_shaderID = 0;
	};

	class GLComputeShader : public RHIComputeShader
	{
		public :
			GLComputeShader(const RHIComputeShaderDesc& desc);
			virtual ~GLComputeShader();

		public :
			const void* GetRawBuffer() const final;
			const uint32_t GetID() const;

		private :
			uint32_t m_shaderID = 0;
	};

	class GLPipeLine : public RHIPipeLine
	{
		public :
			GLPipeLine(const RHIPipeLineDesc& desc);
			virtual ~GLPipeLine();

		public :
			const void* GetRawBuffer() const final;
			const uint32_t GetID() const;

		private :
			uint32_t m_pipelineID = 0;
	};
};

#endif // __WTR_GLRESOURCES_H__