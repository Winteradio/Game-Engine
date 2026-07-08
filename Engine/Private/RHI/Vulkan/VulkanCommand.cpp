#include <RHI/Vulkan/VulkanCommand.h>

#include <RHI/Vulkan/VulkanDevice.h>

#include <Log/include/Log.h>

namespace wtr
{
	VulkanCommand::VulkanCommand()
	{}

	bool VulkanCommand::Init(Memory::RefPtr<const VulkanDevice> device)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Command] The vulkan device is invalid, failed to initialize the vulkan command";
			return false;
		}

		LOGINFO() << "[Vk][Command] Succeed to initialize the vulkan command";

		return true;
	}

	void VulkanCommand::Release(Memory::RefPtr<const VulkanDevice> device)
	{
	}
};
