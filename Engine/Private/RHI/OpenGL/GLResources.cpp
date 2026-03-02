#include <RHI/OpenGL/GLResources.h>

#include <glad/glad.h>

namespace wtr
{
	GLBuffer::GLBuffer(const RHIBufferDesc& desc)
		: RHIBuffer(desc)
		, m_bufferID(GL_NONE)
	{}

	GLBuffer::~GLBuffer()
	{}

	const void* GLBuffer::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const uint32_t GLBuffer::GetID() const
	{
		return m_bufferID;
	}

	GLTexture::GLTexture(const RHITextureDesc& desc)
		: RHITexture(desc)
		, m_textureID(GL_NONE)
	{}

	GLTexture::~GLTexture()
	{}

	const void* GLTexture::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const uint32_t GLTexture::GetID() const
	{
		return m_textureID;
	}

	GLSampler::GLSampler(const RHISamplerDesc& desc)
		: RHISampler(desc)
		, m_samplerID(GL_NONE)
	{}

	GLSampler::~GLSampler()
	{}

	const void* GLSampler::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const uint32_t GLSampler::GetID() const
	{
		return m_samplerID;
	}

	GLVertexShader::GLVertexShader(const RHIVertexShaderDesc& desc)
		: RHIVertexShader(desc)
		, m_shaderID(GL_NONE)
	{}

	GLVertexShader::~GLVertexShader()
	{}

	const void* GLVertexShader::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const uint32_t GLVertexShader::GetID() const
	{
		return m_shaderID;
	}

	GLGeometryShader::GLGeometryShader(const RHIGeometryShaderDesc& desc)
		: RHIGeometryShader(desc)
		, m_shaderID(GL_NONE)
	{}

	GLGeometryShader::~GLGeometryShader()
	{}

	const void* GLGeometryShader::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const uint32_t GLGeometryShader::GetID() const
	{
		return m_shaderID;
	}

	GLPixelShader::GLPixelShader(const RHIPixelShaderDesc& desc)
		: RHIPixelShader(desc)
		, m_shaderID(GL_NONE)
	{}

	GLPixelShader::~GLPixelShader()
	{}

	const void* GLPixelShader::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const uint32_t GLPixelShader::GetID() const
	{
		return m_shaderID;
	}

	GLComputeShader::GLComputeShader(const RHIComputeShaderDesc& desc)
		: RHIComputeShader(desc)
		, m_shaderID(GL_NONE)
	{}

	GLComputeShader::~GLComputeShader()
	{}

	const void* GLComputeShader::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const uint32_t GLComputeShader::GetID() const
	{
		return m_shaderID;
	}

	GLPipeLine::GLPipeLine(const RHIPipeLineDesc& desc)
		: RHIPipeLine(desc)
		, m_pipelineID(GL_NONE)
	{}

	GLPipeLine::~GLPipeLine()
	{}

	const void* GLPipeLine::GetRawBuffer() const
	{
		return static_cast<const void*>(this);
	}

	const uint32_t GLPipeLine::GetID() const
	{
		return m_pipelineID;
	}
}