#ifndef __WTR_GLRESOURCES_H__
#define __WTR_GLRESOURCES_H__

#include <RHI/RHIResources.h>

namespace wtr
{
	class GLResource
	{
	public :
		GLResource();
		virtual ~GLResource() = default;

	public :
		void SetID(const uint32_t resourceID);
		const uint32_t GetID() const;

	private :
		uint32_t m_resourceID;
	};

	class GLBuffer : public RHIBuffer, public GLResource
	{
		public : 
			GLBuffer(const RHIBufferDesc& desc);
			virtual ~GLBuffer();
	};

	class GLTexture : public RHITexture, public GLResource
	{
		public :
			GLTexture(const RHITextureDesc& desc);
			virtual ~GLTexture();
	};

	class GLSampler : public RHISampler, public GLResource
	{
		public :
			GLSampler(const RHISamplerDesc& desc);
			virtual ~GLSampler();
	};

	class GLVertexShader : public RHIVertexShader, public GLResource
	{
		public :
			GLVertexShader(const RHIVertexShaderDesc& desc);
			virtual ~GLVertexShader();
	};

	class GLGeometryShader : public RHIGeometryShader, public GLResource
	{
		public :
			GLGeometryShader(const RHIGeometryShaderDesc& desc);
			virtual ~GLGeometryShader();
	};

	class GLHullShader : public RHIHullShader, public GLResource
	{
		public :
			GLHullShader(const RHIHullShaderDesc& desc);
			virtual ~GLHullShader();
	};

	class GLPixelShader : public RHIPixelShader, public GLResource
	{
		public :
			GLPixelShader(const RHIPixelShaderDesc& desc);
			virtual ~GLPixelShader();
	};

	class GLComputeShader : public RHIComputeShader, public GLResource
	{
		public :
			GLComputeShader(const RHIComputeShaderDesc& desc);
			virtual ~GLComputeShader();
	};

	class GLPipeLine : public RHIPipeLine, public GLResource
	{
		public :
			GLPipeLine(const RHIPipeLineDesc& desc);
			virtual ~GLPipeLine();
	};
};

#endif // __WTR_GLRESOURCES_H__