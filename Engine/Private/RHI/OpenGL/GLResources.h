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

	class GLVertexLayout : public RHIVertexLayout, public GLResource
	{
	public:
		GLVertexLayout(const RHIVertexLayoutDesc& desc);
		virtual ~GLVertexLayout() = default;
	};

	class GLBuffer : public RHIBuffer, public GLResource
	{
	public : 
		GLBuffer(const RHIBufferDesc& desc);
		virtual ~GLBuffer() = default;
	};

	class GLBufferView : public RHIBufferView
	{
	public:
		GLBufferView(Memory::RefPtr<RHIBuffer> buffer);
		virtual ~GLBufferView() = default;
	};

	class GLTexture : public RHITexture, public GLResource
	{
	public :
		GLTexture(const RHITextureDesc& desc);
		virtual ~GLTexture() = default;
	};

	class GLTextureView : public RHITextureView
	{
	public:
		GLTextureView(Memory::RefPtr<RHITexture> texture);
		virtual ~GLTextureView() = default;
	};

	class GLSampler : public RHISampler, public GLResource
	{
	public :
		GLSampler(const RHISamplerDesc& desc);
		virtual ~GLSampler() = default;
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
		virtual ~GLVertexShader() = default;
	};

	class GLHullShader : public GLShader
	{
	public:
		GLHullShader(const RHIShaderDesc& desc);
		virtual ~GLHullShader() = default;
	};

	class GLDomainShader : public GLShader
	{
	public:
		GLDomainShader(const RHIShaderDesc& desc);
		virtual ~GLDomainShader() = default;
	};

	class GLGeometryShader : public GLShader
	{
	public :
		GLGeometryShader(const RHIShaderDesc& desc);
		virtual ~GLGeometryShader() = default;
	};

	class GLPixelShader : public GLShader
	{
	public :
		GLPixelShader(const RHIShaderDesc& desc);
		virtual ~GLPixelShader() = default;
	};

	class GLComputeShader : public GLShader
	{
	public :
		GLComputeShader(const RHIShaderDesc& desc);
		virtual ~GLComputeShader() = default;
	};

	class GLPipeLine : public RHIPipeLine, public GLResource
	{
	public :
		GLPipeLine(const RHIPipeLineDesc& desc);
		virtual ~GLPipeLine() = default;
	};

	class GLRenderPass : public RHIRenderPass, public GLResource
	{
	public :
		GLRenderPass(const RHIRenderPassDesc& desc);
		virtual ~GLRenderPass() = default;
	};
};

#endif // __WTR_GLRESOURCES_H__