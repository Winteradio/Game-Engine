#include <RHI/Vulkan/VulkanMemory.h>

#include <RHI/Vulkan/VulkanDevice.h>

#include <Log/include/Log.h>

namespace wtr
{
	VulkanArena::VulkanArena()
		: TLSFArena()
		, memory(VK_NULL_HANDLE)
		, memoryFlags(VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM)
	{
	}

	VulkanAllocation::VulkanAllocation()
		: Super::Allocation()
		, arena(nullptr)
		, memory(VK_NULL_HANDLE)
		, memoryFlags(VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM)
	{}

	bool VulkanAllocation::IsValid() const
	{
		return arena && memory != VK_NULL_HANDLE && memoryFlags != VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM;
	}
	const VulkanAllocation VulkanAllocation::Null = VulkanAllocation();

	VulkanMemory::VulkanMemory()
		: m_arenas()
		, m_phyProperties()
	{}

	void VulkanMemory::Init(Memory::RefPtr<const VulkanDevice> device)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Memory] The vulkan device is invalid, failed to initialize the vulkan memory";
			return;
		}

		vkGetPhysicalDeviceMemoryProperties(device->GetPhysicalDevice(), &m_phyProperties);
	}

	void VulkanMemory::Release(Memory::RefPtr<const VulkanDevice> device)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Memory] The vulkan device is invalid, failed to release the vulkan memory";
			return;
		}

		VkDevice logicalDevice = device->GetLogicalDevice();

		for (auto& [memorType, arenas] : m_arenas)
		{
			for (auto& arena : arenas)
			{
				if (arena && arena->memory != VK_NULL_HANDLE)
				{
					vkFreeMemory(logicalDevice, arena->memory, nullptr);
				}
			}
		}
		
		m_arenas.Clear();
	}

	VulkanAllocation VulkanMemory::Allocate(Memory::RefPtr<const VulkanDevice> device, const VkMemoryRequirements& requirements, const VkMemoryPropertyFlags& properties)
	{
		const uint32_t memoryTypeIndex = FindMemoryTypeIndex(device, requirements.memoryTypeBits, properties);
		if (memoryTypeIndex == UINT32_MAX)
		{
			return {};
		}

		VkMemoryPropertyFlags memoryType = m_phyProperties.memoryTypes[memoryTypeIndex].propertyFlags;

		TLSFArena::Allocation allocatedInfo{};

		Memory::RefPtr<VulkanArena> arena = nullptr;

		auto& typeArenas = m_arenas[memoryType];
		for (size_t index = 0; index < typeArenas.Size(); index++)
		{
			arena = typeArenas[index];
			if (!arena)
			{
				continue;
			}

			allocatedInfo = arena->Allocate(requirements.size, requirements.alignment);
			if (allocatedInfo.index != TLSFArena::NULL_INDEX)
			{
				break;
			}
		}

		if (allocatedInfo.index == TLSFArena::NULL_INDEX)
		{
			arena = CreateArena(device, requirements.size, FindMemoryTypeIndex(device, requirements.memoryTypeBits, properties));
			if (!arena)
			{
				return {};
			}

			typeArenas.PushBack(arena);
			allocatedInfo = arena->Allocate(requirements.size, requirements.alignment);
		}

		if (allocatedInfo.index != TLSFArena::NULL_INDEX)
		{
			VulkanAllocation allocation{};
			allocation.offset = allocatedInfo.offset;
			allocation.size = allocatedInfo.size;
			allocation.index = allocatedInfo.index;
			allocation.memory = arena->memory;
			allocation.memoryFlags = arena->memoryFlags;
			allocation.arena = arena;

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

		auto& arena = allocation.arena;
		if (!arena)
		{
			return;
		}

		arena->Free(allocation);
		if (arena->IsEmpty())
		{
			ReleaseArena(device, arena);
		}
	}

	uint32_t VulkanMemory::FindMemoryTypeIndex(Memory::RefPtr<const VulkanDevice> device, const uint32_t typeBit, const VkMemoryPropertyFlags properties) const
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

	Memory::RefPtr<VulkanArena> VulkanMemory::CreateArena(Memory::RefPtr<const VulkanDevice> device, const VkDeviceSize blockSize, const uint32_t memoryTypeIndex)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Memory] Failed to create the vulkan arena";
			return {};
		}

		const size_t requiredSize = BLOCK_SIZE > blockSize ? BLOCK_SIZE : blockSize;
		VkDevice logicalDevice = device->GetLogicalDevice();

		VkMemoryAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = requiredSize;
		allocateInfo.memoryTypeIndex = memoryTypeIndex;

		VkDeviceMemory memory = VK_NULL_HANDLE;
		VkResult result = vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &memory);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk][Memory] Failed to allocate a new memory block, size=" << requiredSize << ", type Index =" << memoryTypeIndex;
			return {};
		}

		Memory::RefPtr<VulkanArena> arena = Memory::MakeRef<VulkanArena>();
		if (!arena)
		{
			vkFreeMemory(logicalDevice, memory, nullptr);
			return {};
		}
		else
		{
			arena->memory = memory;
			arena->memoryFlags = m_phyProperties.memoryTypes[memoryTypeIndex].propertyFlags;
			arena->Init(requiredSize);
		}

		return arena;
	}

	void VulkanMemory::ReleaseArena(Memory::RefPtr<const VulkanDevice> device, Memory::RefPtr<VulkanArena> arena)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE)
		{
			return;
		}

		if (!arena || arena->memory == VK_NULL_HANDLE || !arena->IsEmpty())
		{
			return;
		}

		VkDevice logicalDevice = device->GetLogicalDevice();

		vkFreeMemory(logicalDevice, arena->memory, nullptr);
		
		auto itrType = m_arenas.Find(arena->memoryFlags);
		if (itrType == m_arenas.End())
		{
			return;
		}

		auto& typeArenas = itrType->second;
		auto itrArena = typeArenas.Find(arena);
		if (itrArena != typeArenas.End())
		{
			arena->memory = VK_NULL_HANDLE;
			arena->memoryFlags = VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM;

			typeArenas.Erase(itrArena);
		}
	}
};
