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

	class GLShader : virtual public RHIShader, public GLResource
	{
		public :
			GLShader() = default;
			virtual ~GLShader() = default;
	};

	class GLVertexShader : virtual public RHIVertexShader, virtual public GLShader
	{
		public :
			GLVertexShader(const RHIVertexShaderDesc& desc);
			virtual ~GLVertexShader();
	};

	class GLGeometryShader : virtual public RHIGeometryShader, virtual public GLShader
	{
		public :
			GLGeometryShader(const RHIGeometryShaderDesc& desc);
			virtual ~GLGeometryShader();
	};

	class GLHullShader : virtual public RHIHullShader, virtual public GLShader
	{
		public :
			GLHullShader(const RHIHullShaderDesc& desc);
			virtual ~GLHullShader();
	};

	class GLPixelShader : virtual public RHIPixelShader, virtual public GLShader
	{
		public :
			GLPixelShader(const RHIPixelShaderDesc& desc);
			virtual ~GLPixelShader();
	};

	class GLComputeShader : virtual public RHIComputeShader, virtual public GLShader
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