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

	GLShader::GLShader(const RHIShaderDesc& desc)
		: RHIShader(desc)
		, GLResource()
	{
	}

	GLVertexShader::GLVertexShader(const RHIShaderDesc& desc)
		: GLShader(desc)
	{}

	GLVertexShader::~GLVertexShader()
	{}

	GLGeometryShader::GLGeometryShader(const RHIShaderDesc& desc)
		: GLShader(desc)
	{}

	GLGeometryShader::~GLGeometryShader()
	{}

	GLHullShader::GLHullShader(const RHIShaderDesc& desc)
		: GLShader(desc)
	{}

	GLHullShader::~GLHullShader()
	{}

	GLPixelShader::GLPixelShader(const RHIShaderDesc& desc)
		: GLShader(desc)
	{}

	GLPixelShader::~GLPixelShader()
	{}

	GLComputeShader::GLComputeShader(const RHIShaderDesc& desc)
		: GLShader(desc)
	{}

	GLComputeShader::~GLComputeShader()
	{}

	GLPipeLine::GLPipeLine(const RHIPipeLineDesc& desc)
		: RHIPipeLine(desc)
		, GLResource()
	{}

	GLPipeLine::~GLPipeLine()
	{}

	GLRenderTarget::GLRenderTarget(const RHIRenderTargetDesc& desc)
		: RHIRenderTarget(desc)
		, GLResource()
	{
		const size_t totalCount = GetColorAttachCount();
		m_colorAttachments.Assign(totalCount, GL_NONE);

		for (const auto& colorAttach : desc.colors)
		{
			if (colorAttach.slot >= totalCount)
			{
				continue;
			}

			const size_t index = static_cast<const size_t>(colorAttach.slot);
			m_colorAttachments[index] = GL_COLOR_ATTACHMENT0 + static_cast<uint32_t>(index);
		}
	}

	GLRenderTarget::~GLRenderTarget()
	{}

	const wtr::DynamicArray<uint32_t>& GLRenderTarget::GetColorAttachments() const
	{
		return m_colorAttachments;
	}
}