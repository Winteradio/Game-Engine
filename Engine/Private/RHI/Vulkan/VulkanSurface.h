#ifndef __WTR_VULKANSURFACE_H__
#define __WTR_VULKANSURFACE_H__

#include <vulkan/vulkan.h>

#include <RHI/Vulkan/VulkanResources.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class VulkanDevice;
	class VulkanSystem;
};

namespace wtr
{
	class VulkanSurface
	{
	public :
		VulkanSurface();
		~VulkanSurface() = default;

	public :
		bool InitSurface(void* nativeHandle, Memory::RefPtr<const VulkanDevice> device);
		bool InitSwapChain(void* nativeHandle, VulkanSystem& system,Memory::RefPtr<const VulkanDevice> device);
		void Release(Memory::RefPtr<const VulkanDevice> device);

		VkSurfaceKHR	GetSurface() const;
		VkSwapchainKHR	GetSwapChain() const;
		VkFormat		GetSwapChainFormat() const;
		VkExtent2D		GetSwapChainExtent() const;

		Memory::RefPtr<const VulkanRenderTarget> GetSwapChainRenderTarget(const size_t index) const;

	private :
		bool InitSwapChainImages(VulkanSystem& system,Memory::RefPtr<const VulkanDevice> device);
		bool InitSwapChainRenderTargets(VulkanSystem& system, Memory::RefPtr<const VulkanDevice> device);

		VkSurfaceFormatKHR	ChooseSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& formats);
		VkPresentModeKHR	ChoosePresentMode(const DynamicArray<VkPresentModeKHR>& modes);
		VkExtent2D			ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, void* nativeHandle);
		uint32_t			ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities);

	private :
		VkSurfaceKHR	m_surface;
		VkSwapchainKHR	m_swapChain;
		VkFormat		m_swapChainFormat;
		VkExtent2D		m_swapChainExtent;

		VkSurfaceFormatKHR m_requiredFormat;
		VkPresentModeKHR m_requiredPresent;

		DynamicArray<Memory::RefPtr<VulkanTexture>> m_swapChainTextures;
		DynamicArray<Memory::RefPtr<VulkanRenderTarget>> m_swapChainRenderTargets;
		Memory::RefPtr<VulkanTexture> m_depthStencilTexture;
	};
};

#endif // __WTR_VULKANSURFACE_H__