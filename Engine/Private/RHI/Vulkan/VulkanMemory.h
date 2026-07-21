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
		VkDeviceMemory	memory;
		uint32_t		memoryType;
		TLSFArena		arena;

		VulkanBlock();
	};

	struct VulkanAllocation : TLSFArena::Allocation
	{
		using Super = TLSFArena;

		Memory::RefPtr<VulkanBlock> block;

		VkDeviceMemory	memory;
		uint32_t		memoryType;

		VulkanAllocation();
	};

	class VulkanMemory
	{
	public :
		VulkanMemory();
		~VulkanMemory() = default;

	public :
		void Release(Memory::RefPtr<const VulkanDevice> device);

		VulkanAllocation Allocate(Memory::RefPtr<const VulkanDevice> device, const VkMemoryRequirements& requirements, const VkMemoryPropertyFlags& properties);
		void Free(Memory::RefPtr<const VulkanDevice> device, const VulkanAllocation& allocation);

	private :
		uint32_t FindMemoryType(Memory::RefPtr<const VulkanDevice> device, const uint32_t typeBit, const VkMemoryPropertyFlags properties) const;

		Memory::RefPtr<VulkanBlock> CreateBlock(Memory::RefPtr<const VulkanDevice> device, const uint32_t memoryType, const VkDeviceSize blockSize);

	private :
		static constexpr VkDeviceSize BLOCK_SIZE = 1024ull * 1024ull * 4ull;

		HashMap<uint32_t, DynamicArray<Memory::RefPtr<VulkanBlock>>> m_blocks;
	};
}

#endif // __WTR_VULKANMEMORY_H__