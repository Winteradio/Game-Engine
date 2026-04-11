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

	GLVertexLayout::GLVertexLayout(const RHIVertexLayoutDesc& desc)
		: RHIVertexLayout(desc)
		, GLResource()
	{}

	GLVertexLayout::~GLVertexLayout()
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
	{}

	GLVertexShader::~GLVertexShader()
	{}

	GLGeometryShader::GLGeometryShader(const RHIGeometryShaderDesc& desc)
		: RHIGeometryShader(desc)
	{}

	GLGeometryShader::~GLGeometryShader()
	{}

	GLHullShader::GLHullShader(const RHIHullShaderDesc& desc)
		: RHIHullShader(desc)
	{}

	GLHullShader::~GLHullShader()
	{}

	GLPixelShader::GLPixelShader(const RHIPixelShaderDesc& desc)
		: RHIPixelShader(desc)
	{}

	GLPixelShader::~GLPixelShader()
	{}

	GLComputeShader::GLComputeShader(const RHIComputeShaderDesc& desc)
		: RHIComputeShader(desc)
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