#include <RHI/Vulkan/VulkanCommand.h>

#include <RHI/Vulkan/VulkanDevice.h>

#include <Log/include/Log.h>

namespace wtr
{
	VulkanCommand::VulkanCommand(const bool primary)
		: m_commandPool(VK_NULL_HANDLE)
		, m_commandBuffers()
		, m_primary(primary)
	{}

	bool VulkanCommand::Init(Memory::RefPtr<const VulkanDevice> device, const uint32_t bufferCount)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE || bufferCount == 0)
		{
			LOGERROR() << "[Vk][Command] The vulkan device or buffer count is invalid, failed to initialize the vulkan command";
			return false;
		}

		VkDevice logicalDevice = device->GetLogicalDevice();

		// Command Pool Flags
		// Transient Bit - frequently change and delete, it is suitable for the frame data.
		// Reset Command Buffer Bit - possible reset individual.

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.pNext = nullptr;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

		VkResult result = vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &m_commandPool);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk][Command] Failed to create the vkCommandPool";
			return false;
		}

		if (!AllocateBuffer(device, bufferCount))
		{
			LOGERROR() << "[Vk][Command] Failed to allocate the vkCommandBuffer";
			return false;
		}

		LOGINFO() << "[Vk][Command] Succeed to initialize the vulkan command";

		return true;
	}

	bool VulkanCommand::AllocateBuffer(Memory::RefPtr<const VulkanDevice> device, const uint32_t bufferCount)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE || m_commandPool == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Command] The vulkan device or command pool is invalid, failed to allocate the vkCommandBuffer";
			return false;
		}

		if (bufferCount == 0)
		{
			LOGERROR() << "[Vk][Command] The required buffer count is zero, failed to allocate the vkCommandBuffer";
			return false;
		}

		VkDevice logicalDevice = device->GetLogicalDevice();

		VkCommandBufferAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.pNext = nullptr;
		allocateInfo.commandPool = m_commandPool;
		allocateInfo.level = IsPrimary() ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		allocateInfo.commandBufferCount = bufferCount;

		DynamicArray<VkCommandBuffer> commandBuffers(bufferCount);

		VkResult result = vkAllocateCommandBuffers(logicalDevice, &allocateInfo, commandBuffers.Data());
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk][Command] Failed to allocate the command buffers";
			return false;
		}

		m_commandBuffers.Insert(m_commandBuffers.End(), commandBuffers.Begin(), commandBuffers.End());

		return true;
	}

	void VulkanCommand::Release(Memory::RefPtr<const VulkanDevice> device)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE || m_commandPool == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Command] The vulkan device or comand poool is invalid, failed to release the vulkan command";
			return;
		}

		for (size_t index = 0; index < m_commandBuffers.Size(); index++)
		{
			ReleaseBuffer(device, index);
		}

		VkDevice logicalDevice = device->GetLogicalDevice();
		vkDestroyCommandPool(logicalDevice, m_commandPool, nullptr);

		m_commandBuffers.Clear();
		m_commandPool = VK_NULL_HANDLE;

		LOGINFO() << "[Vk][Command] Succeed to release the vulkan command";
	}

	void VulkanCommand::ReleaseBuffer(Memory::RefPtr<const VulkanDevice> device, const size_t index)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE || m_commandPool == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Command] The vulkan device or command pool is invalid, failed to reset the command buffer";
			return;
		}

		if (index >= m_commandBuffers.Size() || m_commandBuffers[index] == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Command] The required index over than command buffer's size, failed to reset the command buffer";
			return;
		}

		VkDevice logicalDevice = device->GetLogicalDevice();

		vkFreeCommandBuffers(logicalDevice, m_commandPool, 1, &m_commandBuffers[index]);
	}

	void VulkanCommand::ResetPool(Memory::RefPtr<const VulkanDevice> device)
	{
		if (!device || device->GetLogicalDevice() == VK_NULL_HANDLE || m_commandPool == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Command] The vulkan device or command pool is invalid, failed to reset the command pool";
			return;
		}

		VkDevice logicalDevice = device->GetLogicalDevice();

		VkResult result = vkResetCommandPool(logicalDevice, m_commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk][Command] Failed to reset the vkCommandPool";
			return;
		}
	}

	void VulkanCommand::ResetBuffer(const size_t index)
	{
		if (index >= m_commandBuffers.Size() || m_commandBuffers[index] == VK_NULL_HANDLE)
		{
			LOGERROR() << "[Vk][Command] The required index over than the command buffer's size, failed to reset the command buffer";
			return;
		}

		VkCommandBuffer commandBuffer = m_commandBuffers[index];
		VkResult result = vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
		if (result != VK_SUCCESS)
		{
			LOGERROR() << "[Vk][Command] Failed to reset the vkCommandBuffer";
			return;
		}
	}

	bool VulkanCommand::IsPrimary() const
	{
		return m_primary;
	}

	VkCommandBuffer VulkanCommand::GetCommandBuffer(const size_t index) const
	{
		if (index >= m_commandBuffers.Size())
		{
			return {};
		}

		return m_commandBuffers[index];
	}

	uint32_t VulkanCommand::GetCommandBufferCount() const
	{
		return m_commandBuffers.Size();
	}
};
