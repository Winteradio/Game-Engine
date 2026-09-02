#ifndef __WTR_VULKANDEVICE_H__
#define __WTR_VULKANDEVICE_H__

#include <vulkan/vulkan.h>

#include <Memory/include/Core.h>
#include <Container/include/DynamicArray.h>

namespace wtr
{
	class VulkanSurface;
};

namespace wtr
{
	class VulkanDevice
	{
	public:
		VulkanDevice();
		~VulkanDevice() = default;

	public:
		bool InitInstance();
		bool InitDebugMessenger();
		bool InitDevice(Memory::RefPtr<const VulkanSurface> surface);
		void Release();

		bool IsValid() const;
		VkInstance GetInstance() const;
		VkDevice GetLogicalDevice() const;
		VkPhysicalDevice GetPhysicalDevice() const;

		const VkPhysicalDeviceLimits& GetPhysicalLimits() const;

		uint32_t GetApiVersion() const;
		uint32_t GetAppVersion() const;
		uint32_t GetEngineVersion() const;

		VkQueue GetQueue() const;
		uint32_t GetQueueIndex() const;

	private:
		bool InitPhysicalDevice(Memory::RefPtr<const VulkanSurface> surface);
		bool InitLogicalDevice();

		bool CheckDevice(VkPhysicalDevice device, Memory::RefPtr<const VulkanSurface> surface);
		bool CheckDeviceExtensions(VkPhysicalDevice device);
		bool CheckValidationLayer();

		uint32_t GetQueueFamily(VkPhysicalDevice device, const uint32_t queueBit);

	private:
		VkDebugUtilsMessengerEXT m_debugMessenger;

		VkInstance				m_instance;
		VkPhysicalDeviceLimits	m_physicalLimits;
		VkPhysicalDevice		m_physicalDevice;
		VkDevice				m_logicalDevice;
		VkQueue					m_queue;

		uint32_t m_apiVersion;
		uint32_t m_appVersion;
		uint32_t m_engineVersion;

		uint32_t m_queueFamily;
		uint32_t m_queueBit;
	};
};

#endif // __WTR_VULKANDEVICE_H__