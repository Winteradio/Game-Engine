#ifndef __WTR_VULKANMEMORY_H__
#define __WTR_VULKANMEMORY_H__

#include <vulkan/vulkan.h>

#include <Container/include/TLSFArena.h>
#include <Container/include/HashMap.h>
#include <Container/include/DynamicArray.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class VulkanDevice;
};

namespace wtr
{
	struct VulkanBlock
	{
		VkDeviceMemory			memory;
		VkMemoryPropertyFlags	memoryFlags;
		TLSFArena				arena;

		VulkanBlock();
	};

	struct VulkanAllocation : TLSFArena::Allocation
	{
		using Super = TLSFArena;

		static VulkanAllocation Null;

		Memory::RefPtr<VulkanBlock> block;

		VkDeviceMemory			memory;
		VkMemoryPropertyFlags	memoryFlags;

		VulkanAllocation();
	};

	class VulkanMemory
	{
	public :
		VulkanMemory();
		~VulkanMemory() = default;

	public :
		void Init(Memory::RefPtr<const VulkanDevice> device);
		void Release(Memory::RefPtr<const VulkanDevice> device);

		VulkanAllocation Allocate(Memory::RefPtr<const VulkanDevice> device, const VkMemoryRequirements& requirements, const VkMemoryPropertyFlags& properties);
		void Free(Memory::RefPtr<const VulkanDevice> device, const VulkanAllocation& allocation);

	private :
		uint32_t FindMemoryTypeIndex(Memory::RefPtr<const VulkanDevice> device, const uint32_t typeBit, const VkMemoryPropertyFlags properties) const;

		Memory::RefPtr<VulkanBlock> CreateBlock(Memory::RefPtr<const VulkanDevice> device, const VkDeviceSize blockSize, const uint32_t memoryTypeIndex);

	private :
		static constexpr VkDeviceSize BLOCK_SIZE = 1024ull * 1024ull * 4ull;

		HashMap<VkMemoryPropertyFlags, DynamicArray<Memory::RefPtr<VulkanBlock>>> m_blocks;
		VkPhysicalDeviceMemoryProperties m_phyProperties;
	};
}

#endif // __WTR_VULKANMEMORY_H__