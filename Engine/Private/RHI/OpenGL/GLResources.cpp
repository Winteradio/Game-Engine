#include <RHI/OpenGL/GLResources.h>

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

	GLBuffer::GLBuffer(const RHIBufferDesc& desc)
		: RHIBuffer(desc)
		, GLResource()
	{}

	GLBuffer::~GLBuffer()
	{}

	GLTexture::GLTexture(const RHITextureDesc& desc)
		: RHITexture(desc)
		, GLResource() {}

	GLTexture::~GLTexture()
	{}

	GLSampler::GLSampler(const RHISamplerDesc& desc)
		: RHISampler(desc)
		, GLResource() {}

	GLSampler::~GLSampler()
	{}

	GLVertexShader::GLVertexShader(const RHIVertexShaderDesc& desc)
		: RHIVertexShader(desc)
		, GLResource() 
	{}

	GLVertexShader::~GLVertexShader()
	{}

	GLGeometryShader::GLGeometryShader(const RHIGeometryShaderDesc& desc)
		: RHIGeometryShader(desc)
		, GLResource()
	{}

	GLGeometryShader::~GLGeometryShader()
	{}

	GLHullShader::GLHullShader(const RHIHullShaderDesc& desc)
		: RHIHullShader(desc)
		, GLResource()
	{}

	GLHullShader::~GLHullShader()
	{}

	GLPixelShader::GLPixelShader(const RHIPixelShaderDesc& desc)
		: RHIPixelShader(desc)
		, GLResource()
	{}

	GLPixelShader::~GLPixelShader()
	{}

	GLComputeShader::GLComputeShader(const RHIComputeShaderDesc& desc)
		: RHIComputeShader(desc)
		, GLResource()
	{}

	GLComputeShader::~GLComputeShader()
	{}

	GLPipeLine::GLPipeLine(const RHIPipeLineDesc& desc)
		: RHIPipeLine(desc)
		, GLResource()
	{}

	GLPipeLine::~GLPipeLine()
	{}
}