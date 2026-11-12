#include <RHI/OpenGL/GLResources.h>

#include <RHI/RHIResources.h>
#include <RHI/RHIDescriptions.h>

#include <glad/glad.h>

namespace wtr
{
	GLResource::GLResource()
		: m_resourceID(GL_NONE)
	{}

	void GLResource::SetID(const uint32_t resourceID)
	{
		m_resourceID = resourceID;
	}

	const uint32_t GLResource::GetID() const
	{
		return m_resourceID;
	}

	GLVertexLayout::GLVertexLayout(const RHIVertexLayoutDesc& desc)
		: RHIVertexLayout(desc)
		, GLResource()
	{}

	GLBuffer::GLBuffer(const RHIBufferDesc& desc)
		: RHIBuffer(desc)
		, GLResource()
	{}

	GLBufferView::GLBufferView(Memory::RefPtr<RHIBuffer> buffer)
		: RHIBufferView(buffer)
	{}

	GLTexture::GLTexture(const RHITextureDesc& desc)
		: RHITexture(desc)
		, GLResource() 
	{}

	GLTextureView::GLTextureView(Memory::RefPtr<RHITexture> texture)
		: RHITextureView(texture)
	{}

	GLSampler::GLSampler(const RHISamplerDesc& desc)
		: RHISampler(desc)
		, GLResource() 
	{}

	GLShader::GLShader(const RHIShaderDesc& desc)
		: RHIShader(desc)
		, GLResource()
	{}

	GLVertexShader::GLVertexShader(const RHIShaderDesc& desc)
		: GLShader(desc)
	{}

	GLGeometryShader::GLGeometryShader(const RHIShaderDesc& desc)
		: GLShader(desc)
	{}

	GLHullShader::GLHullShader(const RHIShaderDesc& desc)
		: GLShader(desc)
	{}

	GLDomainShader::GLDomainShader(const RHIShaderDesc& desc)
		: GLShader(desc)
	{}

	GLPixelShader::GLPixelShader(const RHIShaderDesc& desc)
		: GLShader(desc)
	{}

	GLComputeShader::GLComputeShader(const RHIShaderDesc& desc)
		: GLShader(desc)
	{}

	GLPipeLine::GLPipeLine(const RHIPipeLineDesc& desc)
		: RHIPipeLine(desc)
		, GLResource()
	{}

	GLRenderPass::GLRenderPass(const RHIRenderPassDesc& desc)
		: RHIRenderPass(desc)
		, GLResource()
	{}
}