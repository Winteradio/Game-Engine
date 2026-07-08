#include <RHI/Vulkan/VulkanResources.h>

#include <RHI/RHIResources.h>
#include <RHI/RHIDescriptions.h>

namespace wtr
{
	VulkanMemoryResource::VulkanMemoryResource()
	{}

	VulkanMemoryResource::~VulkanMemoryResource()
	{
	}

	VulkanBuffer::VulkanBuffer(const RHIBufferDesc& desc)
		: RHIBuffer(desc)
		, VulkanMemoryResource()
		, m_buffer(VK_NULL_HANDLE)
	{}

	VulkanBuffer::~VulkanBuffer()
	{}

	void VulkanBuffer::SetBuffer(const VkBuffer buffer)
	{
		m_buffer = buffer;
	}

	const VkBuffer VulkanBuffer::GetBuffer() const
	{
		return m_buffer;
	}

	VulkanVertexLayout::VulkanVertexLayout(const RHIVertexLayoutDesc& desc)
		: RHIVertexLayout(desc)
		, VulkanResource()
	{}

	VulkanVertexLayout::~VulkanVertexLayout()
	{}

	VulkanTexture::VulkanTexture(const RHITextureDesc& desc)
		: RHITexture(desc)
		, VulkanMemoryResource()
		, m_image(VK_NULL_HANDLE)
		, m_imageView(VK_NULL_HANDLE)
	{}

	VulkanTexture::~VulkanTexture()
	{}

	void VulkanTexture::SetImage(const VkImage image)
	{
		m_image = image;
	}

	void VulkanTexture::SetImageView(const VkImageView imageView)
	{
		m_imageView = imageView;
	}

	const VkImage VulkanTexture::GetImage() const
	{
		return m_image;
	}

	const VkImageView VulkanTexture::GetImageView() const
	{
		return m_imageView;
	}

	VulkanSampler::VulkanSampler(const RHISamplerDesc& desc)
		: RHISampler(desc)
		, VulkanResource()
		, m_sampler(VK_NULL_HANDLE)
	{}

	VulkanSampler::~VulkanSampler()
	{}

	void VulkanSampler::SetSampler(const VkSampler sampler)
	{
		m_sampler = sampler;
	}

	const VkSampler VulkanSampler::GetSampler() const
	{
		return m_sampler;
	}

	VulkanShader::VulkanShader(const RHIShaderDesc& desc)
		: RHIShader(desc)
		, VulkanResource()
		, m_shaderModule(VK_NULL_HANDLE)
	{}

	void VulkanShader::SetShaderModule(const VkShaderModule shaderModule)
	{
		m_shaderModule = shaderModule;
	}

	const VkShaderModule VulkanShader::GetShaderModule() const
	{
		return m_shaderModule;
	}

	VulkanVertexShader::VulkanVertexShader(const RHIShaderDesc& desc)
		: VulkanShader(desc)
	{}

	VulkanVertexShader::~VulkanVertexShader()
	{}

	VulkanHullShader::VulkanHullShader(const RHIShaderDesc& desc)
		: VulkanShader(desc)
	{}

	VulkanHullShader::~VulkanHullShader()
	{}

	VulkanDomainShader::VulkanDomainShader(const RHIShaderDesc& desc)
		: VulkanShader(desc)
	{}

	VulkanDomainShader::~VulkanDomainShader()
	{}

	VulkanGeometryShader::VulkanGeometryShader(const RHIShaderDesc& desc)
		: VulkanShader(desc)
	{}

	VulkanGeometryShader::~VulkanGeometryShader()
	{}

	VulkanPixelShader::VulkanPixelShader(const RHIShaderDesc& desc)
		: VulkanShader(desc)
	{}

	VulkanPixelShader::~VulkanPixelShader()
	{}

	VulkanComputeShader::VulkanComputeShader(const RHIShaderDesc& desc)
		: VulkanShader(desc)
	{}

	VulkanComputeShader::~VulkanComputeShader()
	{}

	VulkanPipeLine::VulkanPipeLine(const RHIPipeLineDesc& desc)
		: RHIPipeLine(desc)
		, VulkanResource()
		, m_pipeline(VK_NULL_HANDLE)
		, m_layout(VK_NULL_HANDLE)
	{}

	VulkanPipeLine::~VulkanPipeLine()
	{}

	void VulkanPipeLine::SetPipeline(const VkPipeline pipeline)
	{
		m_pipeline = pipeline;
	}

	void VulkanPipeLine::SetLayout(const VkPipelineLayout layout)
	{
		m_layout = layout;
	}

	void VulkanPipeLine::AddLayoutDesc(const VkDescriptorSetLayout layoutDesc)
	{
		m_layoutDesc.PushBack(layoutDesc);
	}

	const VkPipeline VulkanPipeLine::GetPipeline() const
	{
		return m_pipeline;
	}

	const VkPipelineLayout VulkanPipeLine::GetLayout() const
	{
		return m_layout;
	}

	const DynamicArray<VkDescriptorSetLayout>& VulkanPipeLine::GetLayoutDesc() const
	{
		return m_layoutDesc;
	}

	VulkanRenderTarget::VulkanRenderTarget(const RHIRenderTargetDesc& desc)
		: RHIRenderTarget(desc)
		, VulkanResource()
		, m_renderTarget(VK_NULL_HANDLE)
	{}

	VulkanRenderTarget::~VulkanRenderTarget()
	{}

	void VulkanRenderTarget::SetRenderTarget(const VkFramebuffer renderTarget)
	{
		m_renderTarget = renderTarget;
	}

	const VkFramebuffer VulkanRenderTarget::GetRenderTarget() const
	{
		return m_renderTarget;
	}

	VulkanRenderPass::VulkanRenderPass(const RHIRenderPassDesc& desc)
		: RHIRenderPass(desc)
		, VulkanResource()
	{}

	VulkanRenderPass::~VulkanRenderPass()
	{}
};
