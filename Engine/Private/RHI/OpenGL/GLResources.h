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

	class GLVertexLayout : public RHIVertexLayout, public GLResource
	{
		public :
			GLVertexLayout(const RHIVertexLayoutDesc& desc);
			virtual ~GLVertexLayout();
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

	class GLShader : public RHIShader, public GLResource
	{
		public :
			GLShader(const RHIShaderDesc& desc);
			virtual ~GLShader() = default;
	};

	class GLVertexShader : public GLShader
	{
		public :
			GLVertexShader(const RHIShaderDesc& desc);
			virtual ~GLVertexShader();
	};

	class GLGeometryShader : public GLShader
	{
		public :
			GLGeometryShader(const RHIShaderDesc& desc);
			virtual ~GLGeometryShader();
	};

	class GLHullShader : public GLShader
	{
		public :
			GLHullShader(const RHIShaderDesc& desc);
			virtual ~GLHullShader();
	};

	class GLPixelShader : public GLShader
	{
		public :
			GLPixelShader(const RHIShaderDesc& desc);
			virtual ~GLPixelShader();
	};

	class GLComputeShader : public GLShader
	{
		public :
			GLComputeShader(const RHIShaderDesc& desc);
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