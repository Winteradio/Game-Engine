#include <RHI/Vulkan/VulkanMemory.h>

#include <RHI/Vulkan/VulkanDevice.h>

#include <Log/include/Log.h>

namespace wtr
{
	VulkanMemory::VulkanMemory()
	{}

	bool VulkanMemory::Init(Memory::RefPtr<const VulkanDevice> device)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Memory] The vulkan device is invalid, failed to initialize the vulkan memory";
			return false;
		}

		LOGINFO() << "[Vk][Memory] Succeed to initialize the vulkan memory";

		return true;
	}

	void VulkanMemory::Release(Memory::RefPtr<const VulkanDevice> device)
	{
	}
};
