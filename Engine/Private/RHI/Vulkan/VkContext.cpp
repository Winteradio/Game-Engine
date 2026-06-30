#include <RHI/Vulkan/VkContext.h>

#include <Log/include/Log.h>
#include <Container/include/StaticArray.h>
#include <Container/include/DynamicArray.h>

#include <vulkan/vulkan_win32.h>

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
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,    // Validation Layer
	};

	const StaticArray<const char*, 1> DEVICE_EXTENSIONS = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT* data,
		void* userData)
	{
		if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			LOGERROR() << "[VK] " << data->pMessage;
		}
		else if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			LOGWARN() << "[VK] " << data->pMessage;
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

	VkContext::VkContext()
		: m_debugMessager(VK_NULL_HANDLE)
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

	VkContext::~VkContext()
	{
		if (m_logicalDevice != VK_NULL_HANDLE)
		{
			vkDestroyDevice(m_logicalDevice, nullptr);
		}

		if (m_physicalDevice != VK_NULL_HANDLE)
		{
			// TODO
		}

		if (m_instance != VK_NULL_HANDLE)
		{
			auto DestroyFunc = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
			if (DestroyFunc)
			{
				DestroyFunc(m_instance, m_debugMessager, nullptr);

				LOGINFO() << "[Vk] Destroy the debug messenger";
			}

			vkDestroyInstance(m_instance, nullptr);

			LOGINFO() << "[Vk] Destroy the vkInstance";
		}

		m_debugMessager = VK_NULL_HANDLE;
		m_instance = VK_NULL_HANDLE;
		m_physicalDevice = VK_NULL_HANDLE;
		m_logicalDevice = VK_NULL_HANDLE;
	}

	bool VkContext::Init(void* nativeHandle)
	{
		if (!InitInstance())
		{
			LOGERROR() << "[Vk] Failed to initialize the vkInstance";
			return false;
		}

		if (!InitDebugMessenger())
		{
			LOGERROR() << "[Vk] Failed to initialize the vkDebugMessenger";
			return false;
		}

		if (!InitPhysicalDevice())
		{
			LOGERROR() << "[Vk] Failed to initialize the vkPhysicalDevice";
			return false;
		}

		if (!InitLogicalDevice())
		{
			LOGERROR() << "[Vk] Failed to initialize the vkDevice";
			return false;
		}

		LOGINFO() << "[Vk] Succeed to initialize the vkContext";

		return true;
	}

	bool VkContext::InitInstance()
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
			LOGERROR() << "[Vk] Failed to create the vk instance";
			return false;
		}

		LOGINFO() << "[Vk] Succeed to create the vkInstance";

		return true;
	}

	bool VkContext::InitDebugMessenger()
	{
		if (!ENABLE_VALIDATION)
		{
			return true;
		}

		auto CreateFunc = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
		if (nullptr == CreateFunc)
		{
			LOGERROR() << "[Vk] Failed to initialize the debug messenger, cause the create func isn't existed";
			return false;
		}

		VkResult result = CreateFunc(m_instance, &DEBUG_CREATE_INFO, nullptr, &m_debugMessager);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to set up the debug messenger";
			return false;
		}

		LOGINFO() << "[Vk] Succeed to create the vkDebugMessenger";

		return true;
	}

	bool VkContext::InitPhysicalDevice()
	{
		if (m_instance == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] The vkInstance is invalid, failed to initialize the vkPhysicalDevice";
			return false;
		}

		uint32_t gpuCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr);
		if (gpuCount == 0)
		{
			LOGERROR() << "[Vk] No GPU with Vulkan support";
			return false;
		}
		else
		{
			LOGINFO() << "[Vk] The GPU with Vulkan support's count is " << gpuCount;
		}

		DynamicArray<VkPhysicalDevice> devices(gpuCount);
		vkEnumeratePhysicalDevices(m_instance, &gpuCount, devices.Data());

		for (auto device : devices)
		{
			if (CheckDevice(device))
			{
				m_physicalDevice = device;
				break;
			}
		}

		if (m_physicalDevice == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] Failed to find a suitable GPU for the vulkan";
			return false;
		}

		VkPhysicalDeviceProperties deviceProperties{};
		vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);

		LOGINFO() << "[Vk] ------ Selected GPU Info -----";
		LOGINFO() << "[Vk] API Version : " << deviceProperties.apiVersion;
		LOGINFO() << "[Vk] Driver Version : " << deviceProperties.driverVersion;
		LOGINFO() << "[Vk] Vender ID : " << deviceProperties.vendorID;
		LOGINFO() << "[Vk] Device Name : " << deviceProperties.deviceName;
		LOGINFO() << "[Vk] Device ID : " << deviceProperties.deviceID;
		
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
		{
			LOGINFO() << "[Vk] Device Type : Intergrated GPU";
		}
		else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			LOGINFO() << "[Vk] Device Type : Discrete GPU";
		}
		else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
		{
			LOGINFO() << "[Vk] Device Type : Virtual GPU";
		}
		else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
		{
			LOGINFO() << "[Vk] Device Type : CPU";
		}
		else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER)
		{
			LOGINFO() << "[Vk] Device Type : Other";
		}
		else
		{
			LOGINFO() << "[Vk] Device Type : None";
		}

		LOGINFO() << "[Vk] ------ Selected GPU Info -----";

		LOGINFO() << "[Vk] Succeed to create the vkPhysicalDevice";

		return true;
	}

	bool VkContext::InitLogicalDevice()
	{
		float priority = 1.f;

		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = m_queueFamily;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &priority;

		/* VkPhysicalDeviceFeatures vs VkPhysicalDeviceFeatures2
		 *
		 * - VkPhysicalDeviceFeatures : a flat struct holding only the Vulkan 1.0 feature flags.
		 *   It cannot grow, so features added by later versions (1.1/1.2/1.3) are NOT included.
		 *
		 * - VkPhysicalDeviceFeatures2 : an extensible wrapper.
		 *   It embeds the 1.0 features in its `.features` member, and provides a `pNext` (void*)
		 *   chain so that newer per-version feature structs can be attached.
		 *
		 * - To enable features from a specific version, chain that version's struct to `pNext`:
		 *     VkPhysicalDeviceVulkan11Features (1.1), VkPhysicalDeviceVulkan13Features (1.3), ...
		 *   The driver walks the pNext chain and identifies each struct by its `sType`
		 *   (so the order in the chain does not matter).
		 */

		// Active the feature that is 'dynamic rendering'
		VkPhysicalDeviceVulkan13Features feature13{};
		feature13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		feature13.dynamicRendering = VK_TRUE;

		VkPhysicalDeviceFeatures2 feature2{};
		feature2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		feature2.pNext = &feature13;

		// Create the logical device;
		VkDeviceCreateInfo deviceInfo{};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pNext = &feature2;
		deviceInfo.queueCreateInfoCount = 1;
		deviceInfo.pQueueCreateInfos = &queueInfo;
		deviceInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.Size());
		deviceInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.Data();

		VkResult result = vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_logicalDevice);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk] Failed to create the vkDevice(logical device)";
			return false;
		}

		vkGetDeviceQueue(m_logicalDevice, m_queueFamily, 0, &m_queue);

		LOGINFO() << "[Vk] Succeed to create the logical device";

		return true;
	}

	bool VkContext::CheckDevice(VkPhysicalDevice device)
	{
		if (device == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] The device is invalid";
			return false;
		}

		// Check the api version;
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(device, &properties);

		const bool validApi = (properties.apiVersion >= m_apiVersion);

		// Check the graphics and compute queue family
		const uint32_t queueFamily = GetQueueFamily(device, m_queueBit);

		const bool validQueue = (queueFamily != UINT32_MAX);

		// Check the device extensions
		const bool validExtension = CheckDeviceExtensions(device);

		// Check the dynamic rendering feature
		VkPhysicalDeviceVulkan13Features feature13{};
		feature13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;

		VkPhysicalDeviceFeatures2 feature2{};
		feature2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		feature2.pNext = &feature13;

		vkGetPhysicalDeviceFeatures2(device, &feature2);

		const bool validFeature = (feature13.dynamicRendering == VK_TRUE);

		if (validApi && validQueue && validExtension && validFeature)
		{
			m_queueFamily = queueFamily;
			
			return true;
		}

		return false;
	}

	bool VkContext::CheckDeviceExtensions(VkPhysicalDevice device)
	{
		if (device == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] The device is invalid";
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

	bool VkContext::CheckValidationLayer()
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

	uint32_t VkContext::GetQueueFamily(VkPhysicalDevice device, const uint32_t queueBit)
	{
		if (device == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk] The device is invalid, failed to get the graphics queue family";
			return UINT32_MAX;
		}

		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

		DynamicArray<VkQueueFamilyProperties> families(count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, families.Data());

		for (size_t index = 0; index < families.Size(); index++)
		{
			if (families[index].queueFlags & queueBit)
			{
				return index;
			}
		}

		return UINT32_MAX;
	}
}