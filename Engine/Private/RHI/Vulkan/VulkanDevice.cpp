#include <RHI/Vulkan/VulkanDevice.h>

#include <RHI/Vulkan/VulkanSurface.h>

#include <Log/include/Log.h>
#include <Container/include/StaticArray.h>

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#else
// TODO
#endif

#include <string.h>

namespace wtr
{
#ifdef __VK_DEBUG__
	constexpr bool ENABLE_VALIDATION = true;
#else
	constexpr bool ENABLE_VALIDATION = false;
#endif

	const StaticArray<const char*, 1> INSTANCE_LAYERS = {
		"VK_LAYER_KHRONOS_validation"
	};

	const StaticArray<const char*, 3> INSTANCE_EXTENTIONS = {
		VK_KHR_SURFACE_EXTENSION_NAME,        // Common
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,  // HWND
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
	};

	const StaticArray<const char*, 1> DEVICE_EXTENSIONS = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT* data,
		void* userData)
	{
		if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			LOGERROR() << "[VK] Error :" << data->pMessage;
		}
		else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			LOGWARN() << "[VK] Warning :" << data->pMessage;
		}
		else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		{
			LOGINFO() << "[VK] Info :" << data->pMessage;
		}
		else
		{
			LOGINFO() << "[VK] " << data->pMessage;
		}

		return VK_FALSE;
	}

	const VkDebugUtilsMessengerCreateInfoEXT DebugCreateInfo()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr;

		return createInfo;
	}

	const VkDebugUtilsMessengerCreateInfoEXT DEBUG_CREATE_INFO = DebugCreateInfo();

	VulkanDevice::VulkanDevice()
		: m_debugMessenger(VK_NULL_HANDLE)
		, m_instance(VK_NULL_HANDLE)
		, m_physicalDevice(VK_NULL_HANDLE)
		, m_logicalDevice(VK_NULL_HANDLE)
		, m_queue(VK_NULL_HANDLE)
		, m_apiVersion(VK_MAKE_API_VERSION(0, 1, 4, 0))
		, m_appVersion(VK_MAKE_VERSION(1, 0, 0))
		, m_engineVersion(VK_MAKE_VERSION(1, 0, 0))
		, m_queueFamily(UINT32_MAX)
		, m_queueBit(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT)
	{}

	bool VulkanDevice::InitInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Winteradio";
		appInfo.applicationVersion = m_appVersion;
		appInfo.pEngineName = "Winteradio";
		appInfo.engineVersion = m_engineVersion;
		appInfo.apiVersion = m_apiVersion;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(INSTANCE_EXTENTIONS.Size());
		createInfo.ppEnabledExtensionNames = INSTANCE_EXTENTIONS.Data();

		if (ENABLE_VALIDATION && CheckValidationLayer())
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(INSTANCE_LAYERS.Size());
			createInfo.ppEnabledLayerNames = INSTANCE_LAYERS.Data();
			createInfo.pNext = &DEBUG_CREATE_INFO;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
		if (VK_SUCCESS != result)
		{
			LOGERROR() << "[Vk][Device] Failed to create the vk instance";
			return false;
		}

		LOGINFO() << "[Vk][Device] Succeed to create the vkInstance";

		return true;
	}

	bool VulkanDevice::InitDebugMessenger()
	{
		if (!ENABLE_VALIDATION)
		{
			return true;
		}

		auto CreateFunc = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
		if (nullptr == CreateFunc)
		{
			LOGERROR() << "[Vk][Device] Failed to initialize the debug messenger, cause the create func isn't existed";
			return false;
		}

		VkResult result = CreateFunc(m_instance, &DEBUG_CREATE_INFO, nullptr, &m_debugMessenger);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk][Device] Failed to set up the debug messenger";
			return false;
		}

		LOGINFO() << "[Vk][Device] Succeed to create the vkDebugMessenger";

		return true;
	}

	bool VulkanDevice::InitDevice(Memory::RefPtr<const VulkanSurface> surface)
	{
		if (!InitPhysicalDevice(surface))
		{
			return false;
		}

		if (!InitLogicalDevice())
		{
			return false;
		}

		LOGINFO() << "[Vk][Device] Succeed to initialize the physical and logical device";
		return true;
	}

	void VulkanDevice::Release()
	{
		if (m_instance == VK_NULL_HANDLE || m_logicalDevice == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Device] The instance or logical device is invalid, failed to release the vulkan device";
			return;
		}

		vkDestroyDevice(m_logicalDevice, nullptr);

		auto DestroyFunc = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
		if (DestroyFunc)
		{
			DestroyFunc(m_instance, m_debugMessenger, nullptr);

			LOGINFO() << "[Vk][Device] Destroy the debug messenger";
		}

		vkDestroyInstance(m_instance, nullptr);

		m_debugMessenger = VK_NULL_HANDLE;
		m_instance = VK_NULL_HANDLE;
		m_physicalDevice = VK_NULL_HANDLE;
		m_logicalDevice = VK_NULL_HANDLE;

		LOGINFO() << "[Vk][Device] Destroy the vkInstance";
	}

	bool VulkanDevice::InitPhysicalDevice(Memory::RefPtr<const VulkanSurface> surface)
	{
		if (m_instance == VK_NULL_HANDLE || !surface || surface->GetSurface() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Device] The vkInstance or vulkan surface is invalid, failed to initialize the vkPhysicalDevice";
			return false;
		}

		uint32_t gpuCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr);
		if (gpuCount == 0)
		{
			LOGERROR() << "[Vk][Device] No GPU with Vulkan support";
			return false;
		}
		else
		{
			LOGINFO() << "[Vk][Device] The GPU with Vulkan support's count is " << gpuCount;
		}

		DynamicArray<VkPhysicalDevice> devices(gpuCount);
		vkEnumeratePhysicalDevices(m_instance, &gpuCount, devices.Data());

		for (auto device : devices)
		{
			if (CheckDevice(device, surface))
			{
				m_physicalDevice = device;
				break;
			}
		}

		if (m_physicalDevice == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Device] Failed to find a suitable GPU for the vulkan";
			return false;
		}

		VkPhysicalDeviceProperties deviceProperties{};
		vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);

		m_physicalLimits = deviceProperties.limits;

		LOGINFO() << "[Vk][Device] ------ Selected GPU Info -----";
		LOGINFO() << "[Vk][Device] API Version : " << deviceProperties.apiVersion;
		LOGINFO() << "[Vk][Device] Driver Version : " << deviceProperties.driverVersion;
		LOGINFO() << "[Vk][Device] Vender ID : " << deviceProperties.vendorID;
		LOGINFO() << "[Vk][Device] Device Name : " << deviceProperties.deviceName;
		LOGINFO() << "[Vk][Device] Device ID : " << deviceProperties.deviceID;
		
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
		{
			LOGINFO() << "[Vk][Device] Device Type : Intergrated GPU";
		}
		else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			LOGINFO() << "[Vk][Device] Device Type : Discrete GPU";
		}
		else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
		{
			LOGINFO() << "[Vk][Device] Device Type : Virtual GPU";
		}
		else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
		{
			LOGINFO() << "[Vk][Device] Device Type : CPU";
		}
		else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER)
		{
			LOGINFO() << "[Vk][Device] Device Type : Other";
		}
		else
		{
			LOGINFO() << "[Vk][Device] Device Type : None";
		}

		LOGINFO() << "[Vk][Device] ------ Selected GPU Info -----";

		LOGINFO() << "[Vk][Device] Succeed to create the vkPhysicalDevice";

		return true;
	}

	bool VulkanDevice::InitLogicalDevice()
	{
		float priority = 1.f;

		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = m_queueFamily;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &priority;

		// Create the logical device;
		VkDeviceCreateInfo deviceInfo{};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.queueCreateInfoCount = 1;
		deviceInfo.pQueueCreateInfos = &queueInfo;
		deviceInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.Size());
		deviceInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.Data();

		VkResult result = vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_logicalDevice);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk][Device] Failed to create the vkDevice(logical device)";
			return false;
		}

		vkGetDeviceQueue(m_logicalDevice, m_queueFamily, 0, &m_queue);

		LOGINFO() << "[Vk][Device] Succeed to create the logical device";

		return true;
	}

	bool VulkanDevice::CheckDevice(VkPhysicalDevice device, Memory::RefPtr<const VulkanSurface> surface)
	{
		if (device == VK_NULL_HANDLE || !surface || surface->GetSurface() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Device] The device or vulkan surfaceis invalid";
			return false;
		}

		VkSurfaceKHR surfaceKHR = surface->GetSurface();

		// Check the api version;
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(device, &properties);

		const bool validApi = (properties.apiVersion >= m_apiVersion);

		// Check the graphics and compute queue family
		const uint32_t queueFamily = GetQueueFamily(device, m_queueBit);

		const bool validQueue = (queueFamily != UINT32_MAX);

		// Check the present support on that family
		VkBool32 presentSupport = VK_FALSE;
		if (validQueue)
		{
			vkGetPhysicalDeviceSurfaceSupportKHR(device, queueFamily, surfaceKHR, &presentSupport);
		}

		const bool validPresent = (presentSupport == VK_TRUE);

		// Check the device extensions
		const bool validExtension = CheckDeviceExtensions(device);

		if (validApi && validQueue && validPresent && validExtension)
		{
			m_queueFamily = queueFamily;
			
			return true;
		}

		return false;
	}

	bool VulkanDevice::CheckDeviceExtensions(VkPhysicalDevice device)
	{
		if (device == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Device] The device is invalid";
			return false;
		}

		uint32_t count = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);

		DynamicArray<VkExtensionProperties> extensionProperties(count);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensionProperties.Data());

		for (const char* pName : DEVICE_EXTENSIONS)
		{
			bool found = false;
			for (const auto& property : extensionProperties)
			{
				if (strcmp(pName, property.extensionName) == 0)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				return false;
			}
		}

		return true;
	}

	bool VulkanDevice::CheckValidationLayer()
	{
		if (!ENABLE_VALIDATION)
		{
			return true;
		}

		uint32_t count = 0;
		vkEnumerateInstanceLayerProperties(&count, nullptr);

		DynamicArray<VkLayerProperties> layerProperties(count);
		vkEnumerateInstanceLayerProperties(&count, layerProperties.Data());

		for (const char* pName : INSTANCE_LAYERS)
		{
			bool found = false;
			for (const auto& property : layerProperties)
			{
				if (strcmp(pName, property.layerName) == 0)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				return false;
			}
		}

		return true;
	}

	uint32_t VulkanDevice::GetQueueFamily(VkPhysicalDevice device, const uint32_t queueBit)
	{
		if (device == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Device] The device is invalid, failed to get the graphics queue family";
			return UINT32_MAX;
		}

		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

		DynamicArray<VkQueueFamilyProperties> families(count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, families.Data());

		for (size_t index = 0; index < families.Size(); index++)
		{
			if ((families[index].queueFlags & queueBit) == queueBit)
			{
				return index;
			}
		}

		return UINT32_MAX;
	}

	VkInstance VulkanDevice::GetInstance() const
	{
		return m_instance;
	}

	const VkPhysicalDeviceLimits& VulkanDevice::GetPhysicalLimits() const
	{
		return m_physicalLimits;
	}

	VkPhysicalDevice VulkanDevice::GetPhysicalDevice() const
	{
		return m_physicalDevice;
	}

	VkDevice VulkanDevice::GetLogicalDevice() const
	{
		return m_logicalDevice;
	}

	uint32_t VulkanDevice::GetApiVersion() const
	{
		return m_apiVersion;
	}

	uint32_t VulkanDevice::GetAppVersion() const
	{
		return m_appVersion;
	}

	uint32_t VulkanDevice::GetEngineVersion() const
	{
		return m_engineVersion;
	}

	VkQueue VulkanDevice::GetQueue() const
	{
		return m_queue;
	}

	uint32_t VulkanDevice::GetQueueIndex() const
	{
		return m_queueFamily;
	}
}