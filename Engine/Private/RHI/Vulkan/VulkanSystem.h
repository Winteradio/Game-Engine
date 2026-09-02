#ifndef __WTR_VULKANSYSTEM_H__
#define __WTR_VULKANSYSTEM_H__

#include <RHI/RHISystem.h>
#include <RHI/RHIStates.h>
#include <RHI/RHIDescriptions.h>

#include <vulkan/vulkan.h>

namespace wtr
{
	class VulkanDevice;
	class VulkanSurface;
	class VulkanCommand;
	class VulkanMemory;
};

namespace wtr
{
	class VulkanSystem : public RHISystem
	{
	public:
		VulkanSystem();
		~VulkanSystem();

	public:
		bool Init(void* nativeHandle) override;
		void FlushPending() override;

	public:
		void Clear(const RHIClearState& state) override;
		
		void BeginContext() override;
		void EndContext() override;
		void BeginFrame() override;
		void EndFrame() override;

		void Resize(const uint32_t width, const uint32_t height) override;
		void Resize(const uint32_t posX, const uint32_t posY, const uint32_t width, const uint32_t height) override;

		Memory::RefPtr<RHIBuffer> CreateBuffer(const RHIBufferDesc desc) override;
		Memory::RefPtr<RHIVertexLayout> CreateVertexLayout(const RHIVertexLayoutDesc desc) override;
		Memory::RefPtr<RHITexture> CreateTexture(const RHITextureDesc desc) override;
		Memory::RefPtr<RHISampler> CreateSampler(const RHISamplerDesc desc) override;
		Memory::RefPtr<RHIShader> CreateVertexShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIShader> CreateGeometryShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIShader> CreateHullShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIShader> CreateDomainShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIShader> CreatePixelShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIShader> CreateComputeShader(const RHIShaderDesc desc) override;
		Memory::RefPtr<RHIPipeLine> CreatePipeLine(const RHIPipeLineDesc desc) override;
		Memory::RefPtr<RHIRenderTarget> CreateRenderTarget(const RHIRenderTargetDesc desc) override;

		void InitializeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void InitializeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) override;
		void InitializeVertexLayout(const RHIVertexLayoutCreateDesc info, Memory::RefPtr<RHIVertexLayout> layout) override;
		void InitializeSampler(const RHISamplerCreateDesc info, Memory::RefPtr<RHISampler> sampler) override;
		void InitializeShader(const RHIShaderCreateDesc info, Memory::RefPtr<RHIShader> shader) override;
		void InitializePipeLine(const RHIPipeLineCreateDesc info, Memory::RefPtr<RHIPipeLine> pipeline) override;
		void InitializeRenderTarget(const RHIRenderTargetCreateDesc info, Memory::RefPtr<RHIRenderTarget> target) override;

		void InitializeTextureSwapChain(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture);

		void UpdateBuffer(const RHIBufferUpdateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void UpdateTexture(const RHITextureUpdateDesc info, Memory::RefPtr<RHITexture> texture) override;
		void UpdateVertexLayout(const RHIVertexLayoutUpdateDesc info, Memory::RefPtr<RHIVertexLayout> layout) override;

		void ResizeBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer) override;
		void ResizeTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture) override;

		void SetConstant(const RHIConstDesc info, const int32_t binding) override;
		void SetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding) override;
		void SetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding) override;
		void SetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding) override;

		void SetVertexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t bidingIndex, const uint32_t offset) override;
		void SetIndexBuffer(Memory::RefPtr<const RHIBuffer> buffer, const uint32_t offset) override;

		void SetVertexLayout(Memory::RefPtr<const RHIVertexLayout> layout) override;
		void SetPipeLine(Memory::RefPtr<const RHIPipeLine> pipeline) override;

		void UnsetBuffer(Memory::RefPtr<const RHIBuffer> buffer, const int32_t binding) override;
		void UnsetTexture(Memory::RefPtr<const RHITexture> texture, const int32_t binding) override;
		void UnsetSampler(Memory::RefPtr<const RHISampler> sampler, const int32_t binding) override;

		void UnsetVertexBuffer(const uint32_t bidingIndex) override;
		void UnsetIndexBuffer() override;

		void UnsetVertexLayout() override;
		void UnsetPipeLine() override;

		void BeginRenderPass(Memory::RefPtr<const RHIRenderTarget> target) override;
		void EndRenderPass() override;
		void ClearRenderTarget(Memory::RefPtr<const RHIRenderTarget> target, const RHIClearState state) override;

		void DispatchCompute(const RHIDispatchDesc info) override;
		void DrawIndexPrimitive(const RHIDrawIndexDesc info) override;

	public :
		bool AllocateBuffer(const RHIBufferCreateDesc info, Memory::RefPtr<RHIBuffer> buffer);
		bool AllocateTexture(const RHITextureCreateDesc info, Memory::RefPtr<RHITexture> texture);

		const RHIBufferCreateDesc GetStagingDesc(const RHIBufferCreateDesc info);
		const RHIBufferCreateDesc GetStagingDesc(const RHITextureCreateDesc info);

		Memory::RefPtr<RHIBuffer> AcquireStaging(const RHIBufferCreateDesc info);
		void ReleaseStaging(Memory::RefPtr<RHIBuffer> staging);

		void CopyBuffer(Memory::RefPtr<const RHIBuffer> src, const RHIBufferCreateDesc srcInfo, Memory::RefPtr<RHIBuffer> dest, const RHIBufferCreateDesc destInfo);
		void CopyBuffer(Memory::RefPtr<const RHIBuffer> src, const RHITextureCreateDesc srcInfo, Memory::RefPtr<RHITexture> dest, const RHITextureCreateDesc destInfo);

		VkBufferUsageFlags GetBufferUsage(const eBufferType bufferType) const;
		VkBufferUsageFlags GetBufferUsage(const eBufferTransfer bufferTransfer) const;
		VkMemoryPropertyFlags GetMemoryProperty(const eDataAccess dataAccess,const eBufferTransfer bufferTransfer) const;

		VkFormat GetDataFormat(const eDataType data) const;

		VkVertexInputRate GetVertexDivisor(const eDataDivisor divisor) const;
		VkFormat GetVertexFormat(const eDataType data, const uint32_t count, const bool normalized) const;
		VkIndexType GetIndexFormat(const eDataType data) const;

		VkImageAspectFlags GetImageAspect(const ePixelFormat pixel) const;
		VkImageAspectFlags GetImageAspect(const eAttachment attach) const;
		VkImageUsageFlags GetImageUsage(const eTextureUsage usage, const bool mipmaps) const;
		VkImageCreateFlags GetImageCreate(const eTextureType type) const;
		VkImageViewType GetImageViewType(const eTextureType type) const;
		VkImageType GetImageType(const eTextureType type) const;
		VkFormat GetImageFormat(const ePixelFormat pixel) const;

		VkSampleCountFlags GetSampleCount(const uint32_t count) const;

		VkFilter GetFilterMode(const eFilterMode filter) const;
		VkSamplerAddressMode GetWrapMode(const eWrapMode wrap) const;
		VkBorderColor GetWrapBorderColor(const eWrapBorderColor wrapBorderColor) const;

		VkCullModeFlags GetCullFace(const eCullFace face) const;
		VkFrontFace GetFrontFace(const eFrontFace face) const;

		VkPrimitiveTopology GetDrawMode(const eDrawMode mode) const;
		VkPolygonMode GetPrimitiveMode(const ePrimitiveMode mode) const;

		VkCompareOp GetCompareFunc(const eCompareFunc func) const;

		VkStencilOp GetStencilOp(const eStencilOp op) const;
		VkBlendFactor GetBlendFunc(const eBlendFunc func) const;
		VkBlendOp GetBlendOp(const eBlendOp op) const;

		VkShaderStageFlagBits GetShaderType(const eShaderType type) const;

	private :
		void FlushBuffer();
		void FlushVertexLayout();
		void FlushTexture();
		void FlushSampler();
		void FlushShader();
		void FlushPipeline();
		void FlushRenderTarget();

	private:
		Memory::RefPtr<VulkanDevice> m_device;
		Memory::RefPtr<VulkanSurface> m_surface;
		Memory::RefPtr<VulkanMemory> m_memory;
		Memory::RefPtr<VulkanCommand> m_primary;

		wtr::DynamicArray<Memory::RefPtr<RHIBuffer>> m_staging;
		wtr::DynamicArray<Memory::RefPtr<RHIBuffer>> m_releaseStaging;

		VkSemaphore m_acquire;
		VkSemaphore m_present;
		VkFence		m_fence;

		uint32_t m_imageIndex;
	};
};

#endif // __WTR_VULKANSYSTEM_H__