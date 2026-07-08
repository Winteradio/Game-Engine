#ifndef __WTR_VULKANCOMMAND_H__
#define __WTR_VULKANCOMMAND_H__

#include <vulkan/vulkan.h>

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class VulkanDevice;
};

namespace wtr
{
	class VulkanCommand
	{
	public:
		VulkanCommand();
		~VulkanCommand() = default;

	public:
		bool Init(Memory::RefPtr<const VulkanDevice> device);
		void Release(Memory::RefPtr<const VulkanDevice> device);
	};
};

#endif // __WTR_VULKANCOMMAND_H__