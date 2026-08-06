#ifndef __WTR_VULKANRESOURCES_H__
#define __WTR_VULKANRESOURCES_H__

#include <RHI/RHIResources.h>

#include <RHI/Vulkan/VulkanMemory.h>
#include <vulkan/vulkan.h>

namespace wtr
{
	class VulkanResource
	{
	public:
		VulkanResource() = default;
		virtual ~VulkanResource() = default;
	};

	class VulkanMemoryResource : public VulkanResource
	{
	public:
		VulkanMemoryResource();
		virtual ~VulkanMemoryResource();

	public :
		void SetAllocation(const VulkanAllocation& allocation);
		const VulkanAllocation& GetAllocation() const;

	private :
		VulkanAllocation m_allocation;
	};

	class VulkanBuffer : public RHIBuffer, public VulkanMemoryResource
	{
	public :
		VulkanBuffer(const RHIBufferDesc& desc);
		virtual ~VulkanBuffer();

	public :
		void SetBuffer(const VkBuffer buffer);
		void SetBufferView(const VkBufferView bufferView);

		const VkBuffer GetBuffer() const;
		const VkBufferView GetBufferView() const;

	private :
		VkBuffer m_buffer;
		VkBufferView m_bufferView;
	};

	class VulkanVertexLayout : public RHIVertexLayout, public VulkanResource
	{
	public:
		VulkanVertexLayout(const RHIVertexLayoutDesc& desc);
		virtual ~VulkanVertexLayout();
	};

	class VulkanTexture : public RHITexture, public VulkanMemoryResource
	{
	public :
		VulkanTexture(const RHITextureDesc& desc);
		virtual ~VulkanTexture();

	public :
		void SetImage(const VkImage image);
		void SetImageView(const VkImageView imageView);

		const VkImage     GetImage()     const;
		const VkImageView GetImageView() const;

	private :
		VkImage     m_image;
		VkImageView m_imageView;
	};

	class VulkanSampler : public RHISampler, public VulkanResource
	{
	public :
		VulkanSampler(const RHISamplerDesc& desc);
		virtual ~VulkanSampler();

	public :
		void      SetSampler(const VkSampler sampler);
		const VkSampler GetSampler() const;

	private :
		VkSampler m_sampler;
	};

	class VulkanShader : public RHIShader, public VulkanResource
	{
	public :
		VulkanShader(const RHIShaderDesc& desc);
		virtual ~VulkanShader() = default;

	public :
		void SetShaderModule(const VkShaderModule shaderModule);
		const VkShaderModule GetShaderModule() const;

	private :
		VkShaderModule m_shaderModule;
	};

	class VulkanVertexShader : public VulkanShader
	{
	public :
		VulkanVertexShader(const RHIShaderDesc& desc);
		virtual ~VulkanVertexShader();
	};

	class VulkanHullShader : public VulkanShader
	{
	public:
		VulkanHullShader(const RHIShaderDesc& desc);
		virtual ~VulkanHullShader();
	};

	class VulkanDomainShader : public VulkanShader
	{
	public:
		VulkanDomainShader(const RHIShaderDesc& desc);
		virtual ~VulkanDomainShader();
	};

	class VulkanGeometryShader : public VulkanShader
	{
	public :
		VulkanGeometryShader(const RHIShaderDesc& desc);
		virtual ~VulkanGeometryShader();
	};

	class VulkanPixelShader : public VulkanShader
	{
	public :
		VulkanPixelShader(const RHIShaderDesc& desc);
		virtual ~VulkanPixelShader();
	};

	class VulkanComputeShader : public VulkanShader
	{
	public :
		VulkanComputeShader(const RHIShaderDesc& desc);
		virtual ~VulkanComputeShader();
	};

	class VulkanPipeLine : public RHIPipeLine, public VulkanResource
	{
	public :
		VulkanPipeLine(const RHIPipeLineDesc& desc);
		virtual ~VulkanPipeLine();

	public :
		void SetPipeline(const VkPipeline pipeline);
		void SetLayout(const VkPipelineLayout layout);
		void AddLayoutDesc(const VkDescriptorSetLayout layoutDesc);

		const VkPipeline GetPipeline()   const;
		const VkPipelineLayout GetLayout()     const;
		const DynamicArray<VkDescriptorSetLayout>& GetLayoutDesc() const;

	private :
		VkPipeline m_pipeline;
		VkPipelineLayout m_layout;
		DynamicArray<VkDescriptorSetLayout> m_layoutDesc;
	};

	class VulkanRenderTarget : public RHIRenderTarget, public VulkanResource
	{
	public :
		VulkanRenderTarget(const RHIRenderTargetDesc& desc);
		virtual ~VulkanRenderTarget();

	public :
		void SetRenderTarget(const VkFramebuffer renderTarget);
		const VkFramebuffer GetRenderTarget() const;

	private :
		VkFramebuffer m_renderTarget;
	};

	class VulkanRenderPass : public RHIRenderPass, public VulkanResource
	{
	public :
		VulkanRenderPass(const RHIRenderPassDesc& desc);
		virtual ~VulkanRenderPass();

	public :
		VkRenderPass m_renderPass;
	};
};

#endif // __WTR_VULKANRESOURCES_H__
