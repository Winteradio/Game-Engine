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
	struct VulkanArena : TLSFArena
	{
		VkDeviceMemory			memory;
		VkMemoryPropertyFlags	memoryFlags;

		VulkanArena();
	};

	struct VulkanAllocation : TLSFArena::Allocation
	{
		using Super = TLSFArena;

		static const VulkanAllocation Null;

		Memory::RefPtr<VulkanArena> arena;

		VkDeviceMemory			memory;
		VkMemoryPropertyFlags	memoryFlags;

		VulkanAllocation();

		bool IsValid() const;
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

		Memory::RefPtr<VulkanArena> CreateArena(Memory::RefPtr<const VulkanDevice> device, const VkDeviceSize blockSize, const uint32_t memoryTypeIndex);
		void ReleaseArena(Memory::RefPtr<const VulkanDevice> device, Memory::RefPtr<VulkanArena> arena);

	private :
		static constexpr VkDeviceSize BLOCK_SIZE = 1024ull * 1024ull * 4ull;

		HashMap<VkMemoryPropertyFlags, DynamicArray<Memory::RefPtr<VulkanArena>>> m_arenas;
		VkPhysicalDeviceMemoryProperties m_phyProperties;
	};
}

#endif // __WTR_VULKANMEMORY_H__