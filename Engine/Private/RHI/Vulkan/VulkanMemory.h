#ifndef __WTR_VULKANMEMORY_H__
#define __WTR_VULKANMEMORY_H__

#include <vulkan/vulkan.h>

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class VulkanDevice;
};

namespace wtr
{
	class VulkanMemory
	{
	public :
		VulkanMemory();
		~VulkanMemory() = default;

	public :
		bool Init(Memory::RefPtr<const VulkanDevice> device);
		void Release(Memory::RefPtr<const VulkanDevice> device);
	};
}

#endif // __WTR_VULKANMEMORY_H__