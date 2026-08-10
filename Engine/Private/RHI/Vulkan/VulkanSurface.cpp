#include <RHI/Vulkan/VulkanSurface.h>

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#else
// TODO
#endif

#include <RHI/Vulkan/VulkanDevice.h>
#include <RHI/Vulkan/VulkanSystem.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	VulkanSurface::VulkanSurface()
		: m_surface(VK_NULL_HANDLE)
		, m_swapChain(VK_NULL_HANDLE)
		, m_swapChainFormat(VK_FORMAT_UNDEFINED)
		, m_swapChainExtent()
		, m_requiredFormat{VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }
		, m_requiredPresent(VK_PRESENT_MODE_FIFO_KHR)
		, m_swapChainTextures()
		, m_swapChainRenderTargets()
		, m_depthStencilTexture()
	{
	}

	bool VulkanSurface::InitSurface(void* nativeHandle, Memory::RefPtr<const VulkanDevice> device)
	{
		if (nativeHandle == nullptr || !device || device->GetInstance() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Surface] The native handle or vulkan device is invalid, failed to initialize the vulkan surface";
			return false;
		}

#ifdef _WIN32
		VkWin32SurfaceCreateInfoKHR surfaceInfo{};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hwnd = static_cast<HWND>(nativeHandle);
		surfaceInfo.hinstance = GetModuleHandle(nullptr);

		VkInstance instance = device->GetInstance();
		VkResult result = vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &m_surface);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk][Surface] Failed to create the win32 vkSurface";
			return false;
		}

		LOGINFO() << "[Vk][Surface] Succeed to create the vkSurface for win32";

		return true;
#else
		return false;
#endif
	}

	bool VulkanSurface::InitSwapChain(void* nativeHandle, VulkanSystem& system, Memory::RefPtr<const VulkanDevice> device)
	{
		if (nativeHandle == nullptr)
		{ 
			LOGERROR() << "[Vk][Surface] The native handle is invalid, failed to initialize the vkSwapChain";
			return false;
		}

		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE || device->GetPhysicalDevice() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Surface] The vulkan device's logical and physical is invalid, failed to initialize the vkSwapChain";
			return false;
		}

		if (m_surface == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Surface] The vulkan surface is invalid, failed to initialize the vkSwapChain";
			return false;
		}

		VkPhysicalDevice physicalDevice = device->GetPhysicalDevice();

		// Find the surface format
		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatCount, nullptr);
		if (formatCount == 0)
		{
			LOGERROR() << "[Vk][Surface] The supported format count is zero, failed to initialize the vkSwapChain";
			return false;
		}

		DynamicArray<VkSurfaceFormatKHR> formats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatCount, formats.Data());

		// Find the present mode
		uint32_t presentCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentCount, nullptr);
		if (presentCount == 0)
		{
			LOGERROR() << "[Vk][Surface] The supported present count is zero, failed to initialize the vkSwapChain";
			return false;
		}

		DynamicArray<VkPresentModeKHR> presents(presentCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentCount, presents.Data());

		VkSurfaceCapabilitiesKHR capabilities{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_surface, &capabilities);

		// Set the swap chain's properties
		VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(formats);
		VkPresentModeKHR presentMode = ChoosePresentMode(presents);
		VkExtent2D extent = ChooseExtent(capabilities, nativeHandle);
		uint32_t imageCount = ChooseImageCount(capabilities);

		// Create the swap chain
		VkSwapchainCreateInfoKHR swapChainInfo{};
		swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainInfo.surface = m_surface;
		swapChainInfo.minImageCount = imageCount;
		swapChainInfo.imageFormat = surfaceFormat.format;
		swapChainInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapChainInfo.imageExtent = extent;
		swapChainInfo.imageArrayLayers = 1;
		swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainInfo.preTransform = capabilities.currentTransform;
		swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapChainInfo.presentMode = presentMode;
		swapChainInfo.clipped = VK_TRUE;
		swapChainInfo.oldSwapchain = VK_NULL_HANDLE;

		VkDevice logicalDevice = device->GetLogicalDevice();

		VkResult result = vkCreateSwapchainKHR(logicalDevice, &swapChainInfo, nullptr, &m_swapChain);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk][Surface] Failed to create the vkSwapChain";
			return false;
		}

		m_swapChainFormat = surfaceFormat.format;
		m_swapChainExtent = extent;

		if (!InitSwapChainImages(system, device))
		{
			LOGERROR() << "[Vk][Surface] Failed to initialize the images for the swap chain";
			return false;
		}

		if (!InitSwapChainRenderTargets(system, device))
		{
			LOGERROR() << "[Vk][Surface] Failed to initialize the render targets for the swap chain";
			return false;
		}

		return true;
	}

	bool VulkanSurface::InitSwapChainImages(VulkanSystem& system, Memory::RefPtr<const VulkanDevice> device)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Surface] The vulkan device is invalid, failed to initialize the swap chain's image";
			return false;
		}

		if (m_swapChain == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Surface] The vulkan swap chain is invalid, failed to initialize the swap chain's image";
			return false;
		}

		VkDevice logicalDevice = device->GetLogicalDevice();

		// Get the image for the swap chain
		uint32_t swapChainImageCount = 0;
		vkGetSwapchainImagesKHR(logicalDevice, m_swapChain, &swapChainImageCount, nullptr);
		if (swapChainImageCount == 0)
		{
			LOGERROR() << "[Vk] The supported the images for the swap chain is zero, failed to intialize the swap chain";
			return false;
		}

		DynamicArray<VkImage> swapChainImages(swapChainImageCount);
		vkGetSwapchainImagesKHR(logicalDevice, m_swapChain, &swapChainImageCount, swapChainImages.Data());

		m_swapChainTextures.Reserve(swapChainImageCount);

		RHITextureCreateDesc colorDesc{};
		colorDesc.width = m_swapChainExtent.width;
		colorDesc.height = m_swapChainExtent.height;
		colorDesc.depth = 1;
		colorDesc.face = 1;
		colorDesc.mipLevels = 1;
		colorDesc.sampleCount = 1;
		colorDesc.format = system.GetPixelFormat(m_swapChainFormat);
		colorDesc.usage = eTextureUsage::eRenderTarget;
		colorDesc.textureType = eTextureType::eTexture2D;
		colorDesc.dataType = eDataType::eUByte;
		colorDesc.generateMips = false;
		colorDesc.compressed = false;

		for (size_t index = 0; index < swapChainImages.Size(); ++index)
		{
			VkImage image = swapChainImages[index];
			if (image == VK_NULL_HANDLE)
			{
				LOGERROR() << "[Vk] The image for the swap chain is invalid, failed to initialize the swap chain's image";
				return false;
			}

			Memory::RefPtr<VulkanTexture> texture = Memory::Cast<VulkanTexture>(system.CreateTexture(colorDesc));
			if (!texture)
			{
				LOGERROR() << "[Vk] Failed to create the texture for the swap chain's image";
				return false;
			}

			texture->SetImage(image);

			system.InitializeTextureSwapChain(colorDesc, texture);
			if (texture->GetImage() == VK_NULL_HANDLE || texture->GetImageView() == VK_NULL_HANDLE || texture->GetState() != eResourceState::eReady)
			{
				LOGERROR() << "[Vk] The texture's image or image view is invalid, failed to initialize the swap chain's image";
				return false;
			}

			m_swapChainTextures.PushBack(texture);
		}

		RHITextureCreateDesc depthStencilDesc{};
		depthStencilDesc.width = m_swapChainExtent.width;
		depthStencilDesc.height = m_swapChainExtent.height;
		depthStencilDesc.depth = 1;
		depthStencilDesc.face = 1;
		depthStencilDesc.mipLevels = 1;
		depthStencilDesc.sampleCount = 1;
		depthStencilDesc.format = ePixelFormat::eD24_S8;
		depthStencilDesc.usage = eTextureUsage::eDepthStencil;
		depthStencilDesc.textureType = eTextureType::eTexture2D;
		depthStencilDesc.dataType = eDataType::eFloat;
		depthStencilDesc.generateMips = false;
		depthStencilDesc.compressed = false;

		Memory::RefPtr<VulkanTexture> texture = Memory::Cast<VulkanTexture>(system.CreateTexture(depthStencilDesc));
		if (!texture)
		{
			LOGERROR() << "[Vk] Failed to create the depth stencil texture for the swap chain";
			return false;
		}

		system.InitializeTexture(depthStencilDesc, texture);
		if (texture->GetImage() == VK_NULL_HANDLE || texture->GetImageView() == VK_NULL_HANDLE || texture->GetState() != eResourceState::eReady)
		{
			LOGERROR() << "[Vk] The depth stencil texture's image or image view is invalid, failed to initialize the swap chain's depth stencil texture";
			return false;
		}

		m_depthStencilTexture = texture;

		LOGINFO() << "[Vk] Succeed to create the vkSwapChain( image Count : " << swapChainImages.Size();

		return true;
	}

	bool VulkanSurface::InitSwapChainRenderTargets(VulkanSystem& system, Memory::RefPtr<const VulkanDevice> device)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Surface] The vulkan device is invalid, failed to initialize the swap chain's render targets";
			return false;
		}

		if (m_swapChainTextures.Empty())
		{
			LOGERROR() << "[Vk][Surface] The swap chain's textures is empty, failed to initialize the swap chain's render targets";
			return false;
		}

		m_swapChainRenderTargets.Reserve(m_swapChainTextures.Size());
		for (const auto& texture : m_swapChainTextures)
		{
			if (!texture || texture->GetImage() == VK_NULL_HANDLE || texture->GetImageView() == VK_NULL_HANDLE || texture->GetState() != eResourceState::eReady)
			{
				continue;
			}

			RHIRenderTargetCreateDesc targetDesc{};
			targetDesc.colors = { RHIColorAttachment{eAttachment::eColor, texture, 0 } };
			targetDesc.depthStencil.texture = m_depthStencilTexture;
			targetDesc.depthStencil.type = eAttachment::eDepthStencil;

			Memory::RefPtr<VulkanRenderTarget> renderTarget = Memory::Cast<VulkanRenderTarget>(system.CreateRenderTarget(targetDesc));
			if (!renderTarget)
			{
				LOGERROR() << "[Vk][Surface] Failed to create the render target for the swap chain";
				return false;
			}

			system.InitializeRenderTarget(targetDesc, renderTarget);
			if (renderTarget->GetRenderTarget() == VK_NULL_HANDLE || renderTarget->GetState() != eResourceState::eReady)
			{
				LOGERROR() << "[Vk][Surface] The render target is not ready, failed to initialize the swap chain's render targets";
				return false;
			}

			m_swapChainRenderTargets.PushBack(renderTarget);
		}

		LOGINFO() << "[Vk][Surface] Succeed to create the vkSwapChain's render targets( render target Count : " << m_swapChainRenderTargets.Size();

		return true;
	}

	void VulkanSurface::Release(Memory::RefPtr<const VulkanDevice> device)
	{
		if (!device || device->GetInstance() == VK_NULL_HANDLE || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			LOGINFO() << "[Vk][Surface] The vulkan device is invalid, failed to release the vulkan surface";
			return;
		}

		VkInstance instance = device->GetInstance();
		VkDevice logicalDevice = device->GetLogicalDevice();

		for (auto& texture : m_swapChainTextures)
		{
			if (!texture || texture->GetImage() == VK_NULL_HANDLE || texture->GetImageView() == VK_NULL_HANDLE)
			{
				continue;
			}

			VkImageView imageView = texture->GetImageView();
			vkDestroyImageView(logicalDevice, imageView, nullptr);
		}

		if (m_depthStencilTexture && m_depthStencilTexture->GetImage() != VK_NULL_HANDLE && m_depthStencilTexture->GetImageView() != VK_NULL_HANDLE)
		{
			VkImageView imageView = m_depthStencilTexture->GetImageView();
			VkImage image = m_depthStencilTexture->GetImage();

			vkDestroyImageView(logicalDevice, imageView, nullptr);
			vkDestroyImage(logicalDevice, image, nullptr);
		}

		for (auto& renderTarget : m_swapChainRenderTargets)
		{
			if (!renderTarget || renderTarget->GetRenderTarget() == VK_NULL_HANDLE)
			{
				continue;
			}

			VkFramebuffer framebuffer = renderTarget->GetRenderTarget();
			vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
		}

		vkDestroySwapchainKHR(logicalDevice, m_swapChain, nullptr);
		vkDestroySurfaceKHR(instance, m_surface, nullptr);

		m_surface = VK_NULL_HANDLE;
		m_swapChain = VK_NULL_HANDLE;
		m_depthStencilTexture.Reset();
		m_swapChainTextures.Clear();
		m_swapChainRenderTargets.Clear();

		LOGINFO() << "[Vk][Surface] Succeed to release the vulkan surface";
	}

	VkSurfaceKHR VulkanSurface::GetSurface() const
	{
		return m_surface;
	}

	VkSwapchainKHR VulkanSurface::GetSwapChain() const
	{
		return m_swapChain;
	}

	VkFormat VulkanSurface::GetSwapChainFormat() const
	{
		return m_swapChainFormat;
	}

	VkExtent2D VulkanSurface::GetSwapChainExtent() const
	{
		return m_swapChainExtent;
	}

	Memory::RefPtr<const VulkanRenderTarget> VulkanSurface::GetSwapChainRenderTarget(const size_t index) const
	{
		if (index >= m_swapChainRenderTargets.Size())
		{
			return nullptr;
		}

		return m_swapChainRenderTargets[index];
	}

	VkSurfaceFormatKHR VulkanSurface::ChooseSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& formats)
	{
		for (const auto& format : formats)
		{
			if (format.format == m_requiredFormat.format && format.colorSpace == m_requiredFormat.colorSpace)
			{
				return format;
			}
		}

		VkSurfaceFormatKHR nullFormat{};
		nullFormat.format = VK_FORMAT_UNDEFINED;
		nullFormat.colorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR;

		return nullFormat;
	}

	VkPresentModeKHR VulkanSurface::ChoosePresentMode(const DynamicArray<VkPresentModeKHR>& modes)
	{
		for (const auto& mode : modes)
		{
			if (mode == m_requiredPresent)
			{
				return mode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanSurface::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, void* nativeHandle)
	{
		if (nativeHandle == nullptr ||
			capabilities.currentExtent.width == UINT32_MAX ||
			capabilities.currentExtent.height == UINT32_MAX)
		{
			return {};
		}

		HWND windowHandle = static_cast<HWND>(nativeHandle);

		RECT rect{};
		GetClientRect(windowHandle, &rect);

		uint32_t width = static_cast<uint32_t>(rect.right - rect.left);
		uint32_t height = static_cast<uint32_t>(rect.bottom - rect.top);

		auto Clamp = [](uint32_t value, uint32_t minValue, uint32_t maxValue)
			{
				return (value < minValue ? minValue : (value > maxValue ? maxValue : value));
			};

		VkExtent2D extent{};
		extent.width = Clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = Clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return extent;
	}

	uint32_t VulkanSurface::ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		uint32_t count = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0 && count > capabilities.maxImageCount)
		{
			count = capabilities.maxImageCount;
		}

		return count;
	}
}