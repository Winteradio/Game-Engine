#include <RHI/Vulkan/VulkanMemory.h>

#include <RHI/Vulkan/VulkanDevice.h>

#include <Log/include/Log.h>

namespace wtr
{
	VulkanBlock::VulkanBlock()
		: memory(VK_NULL_HANDLE)
		, memoryType(0)
		, arena()
	{}

	VulkanAllocation::VulkanAllocation()
		: Super::Allocation()
		, block(nullptr)
		, memory(VK_NULL_HANDLE)
		, memoryType()
	{}

	VulkanMemory::VulkanMemory()
		: m_blocks()
	{}

	void VulkanMemory::Release(Memory::RefPtr<const VulkanDevice> device)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Memory] The vulkan device is invalid, failed to release the vulkan memory";
			return;
		}

		VkDevice logicalDevice = device->GetLogicalDevice();

		for (auto& [memorType, blocks] : m_blocks)
		{
			for (auto& block : blocks)
			{
				if (block && block->memory != VK_NULL_HANDLE)
				{
					vkFreeMemory(logicalDevice, block->memory, nullptr);
				}
			}
		}

		m_blocks.Clear();
	}

	VulkanAllocation VulkanMemory::Allocate(Memory::RefPtr<const VulkanDevice> device, const VkMemoryRequirements& requirements, const VkMemoryPropertyFlags& properties)
	{
		const uint32_t memoryType = FindMemoryType(device, requirements.memoryTypeBits, properties);
		if (memoryType == UINT32_MAX)
		{
			return {};
		}

		TLSFArena::Allocation allocatedInfo{};
		Memory::RefPtr<VulkanBlock> block = nullptr;

		auto& typeBlocks = m_blocks[memoryType];
		for (size_t index = 0; index < typeBlocks.Size(); index++)
		{
			block = typeBlocks[index];
			if (!block)
			{
				continue;
			}

			allocatedInfo = block->arena.Allocate(requirements.size);
			if (allocatedInfo.index != TLSFArena::NULL_INDEX)
			{
				break;
			}
		}

		if (allocatedInfo.index == TLSFArena::NULL_INDEX)
		{
			block = CreateBlock(device, memoryType, requirements.size);
			if (!block)
			{
				return {};
			}

			typeBlocks.PushBack(block);
			allocatedInfo = block->arena.Allocate(requirements.size);
		}

		if (allocatedInfo.index != TLSFArena::NULL_INDEX)
		{
			VulkanAllocation allocation{};
			allocation.offset = allocatedInfo.offset;
			allocation.size = allocatedInfo.size;
			allocation.index = allocatedInfo.index;
			allocation.memory = block->memory;
			allocation.memoryType = block->memoryType;
			allocation.block = block;

			return allocation;
		}

		return {};
	}

	void VulkanMemory::Free(Memory::RefPtr<const VulkanDevice> device, const VulkanAllocation& allocation)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		VkDevice logicalDevice = device->GetLogicalDevice();

		auto& block = allocation.block;
		if (!block)
		{
			return;
		}

		auto& arena = block->arena;
		arena.Free(allocation);

		if (!arena.IsEmpty())
		{
			return;
		}

		vkFreeMemory(logicalDevice, block->memory, nullptr);

		auto itrType = m_blocks.Find(allocation.memoryType);
		if (itrType == m_blocks.End())
		{
			return;
		}

		auto& typeBlocks = itrType->second;

		auto itrBlock = typeBlocks.Find(block);
		if (itrBlock != typeBlocks.End())
		{
			typeBlocks.Erase(itrBlock);
		}

		if (typeBlocks.Empty())
		{
			m_blocks.Erase(itrType);
		}

	}

	uint32_t VulkanMemory::FindMemoryType(Memory::RefPtr<const VulkanDevice> device, const uint32_t typeBit, const VkMemoryPropertyFlags properties) const
	{
		VkPhysicalDeviceMemoryProperties phyProperties;
		vkGetPhysicalDeviceMemoryProperties(device->GetPhysicalDevice(), &phyProperties);

		for (uint32_t index = 0; index < phyProperties.memoryTypeCount; index++)
		{
			if ((typeBit & (1u << index)) == 0)
			{
				continue;
			}

			if ((phyProperties.memoryTypes[index].propertyFlags & properties) != properties)
			{
				continue;
			}

			return index;
		}

		return UINT32_MAX;
	}

	Memory::RefPtr<VulkanBlock> VulkanMemory::CreateBlock(Memory::RefPtr<const VulkanDevice> device, const uint32_t memoryType, const VkDeviceSize blockSize)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Memory] Failed to create the vulkan block";
			return {};
		}

		const size_t requiredSize = BLOCK_SIZE > blockSize ? BLOCK_SIZE : blockSize;
		VkDevice logicalDevice = device->GetLogicalDevice();

		VkMemoryAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = requiredSize;
		allocateInfo.memoryTypeIndex = memoryType;

		VkDeviceMemory memory = VK_NULL_HANDLE;
		VkResult result = vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &memory);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk][Memory] Failed to allocate a new memory block, size=" << requiredSize << ", type=" << memoryType;
			return {};
		}

		Memory::RefPtr<VulkanBlock> block = Memory::MakeRef<VulkanBlock>();
		if (!block)
		{
			return {};
		}

		block->memory = memory;
		block->memoryType = memoryType;
		block->arena.Init(requiredSize);

		return block;
	}
};
