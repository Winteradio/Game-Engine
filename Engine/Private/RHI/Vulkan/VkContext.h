#ifndef __WTR_VKCONTEXT_H__
#define __WTR_VKCONTEXT_H__

#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>
#include <Windows.h>

namespace wtr
{
	class VkContext
	{
	public:
		VkContext();
		~VkContext();

	public:
		bool Init(void* nativeHandle);
		bool MakeCurrent();
		bool ReleaseCurrent();
		bool GetCurrent();
		void Preset();

	private:
		bool InitInstance();
		bool InitDebugMessenger();
		bool InitPhysicalDevice();
		bool InitLogicalDevice();

		bool CheckDevice(VkPhysicalDevice device);
		bool CheckDeviceExtensions(VkPhysicalDevice device);
		bool CheckValidationLayer();

		uint32_t GetQueueFamily(VkPhysicalDevice device, const uint32_t queueBit);

	private:
		VkDebugUtilsMessengerEXT m_debugMessager;

		VkInstance			m_instance;
		VkPhysicalDevice	m_physicalDevice;
		VkDevice			m_logicalDevice;
		VkQueue				m_queue;

		uint32_t m_apiVersion;
		uint32_t m_appVersion;
		uint32_t m_engineVersion;

		uint32_t m_queueFamily;
		uint32_t m_queueBit;
	};
};

#endif // __WTR_VKCONTEXT_H__