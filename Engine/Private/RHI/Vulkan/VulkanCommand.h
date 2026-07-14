#ifndef __WTR_VULKANCOMMAND_H__
#define __WTR_VULKANCOMMAND_H__

#include <vulkan/vulkan.h>

#include <Container/include/DynamicArray.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class VulkanDevice;
};

namespace wtr
{
	// Command Buffer Level (Primary vs Secondary)
	// Primary   - the only level submittable to a queue.
	//				One Primary, owned by the RHI Thread.
	// Secondary - cannot be submitted directly, only run via a Primary's vkCmdExecuteCommands (no nesting).
	//				One Secondary per RHI Worker Thread, each with its own VkCommandPool (VkCommandPool is not thread-safe).
	//
	// Uses the classic VkRenderPass / VkFramebuffer model (dynamicRendering is NOT enabled) -
	// attachment load/store ops, clear values, and layout transitions are declared once on the
	// VkRenderPass (initialLayout/finalLayout + subpass dependency), not re-specified per call.
	//
	// ---------------------------------------------------------------------------------------
	// Per-Frame vkCmd Call Flow
	// ---------------------------------------------------------------------------------------
	//
	// RHI Thread::Run()
	//   vkAcquireNextImageKHR
	//		-> imageIndex, imageAvailableSemaphore
	//   VulkanCommand(Primary)
	//		-> vkBeginCommandBuffer
	//		-> vkCmdBeginRenderPass(VkRenderPassBeginInfo{renderPass, framebuffer, clearValues}, SECONDARY_COMMAND_BUFFERS)
	//
	//		RHI Worker Thread::Run()   (x N, ASync work with RHI Thread)
	//			VulkanCommand(Secondary)
	//				-> vkBeginCommandBuffer(RENDER_PASS_CONTINUE_BIT, VkCommandBufferInheritanceInfo{renderPass, subpass, framebuffer})
	//				-> vkCmdBindPipeline / vkCmdBindVertexBuffers / vkCmdBindDescriptorSets / vkCmdDrawIndexed ...
	//				-> vkEndCommandBuffer
	//
	//				-> (CPU) Work done
	//
	// ---- CPU Join: RHI Thread wait the all RHI Worker Thread ----
	//
	//		-> The command buffer's handle by secondary vulkan command will be re-ordered by the user's priority
	//
	//		-> vkCmdExecuteCommands(secondaries[])
	//		-> vkCmdEndRenderPass
	//		-> vkEndCommandBuffer
	//		-> vkQueueSubmit(wait: imageAvailableSemaphore, signal: renderFinishedSemaphore, fence)
	//		-> vkQueuePresentKHR(wait: renderFinishedSemaphore)

	class VulkanCommand
	{
	public:
		VulkanCommand(const bool primary = true);
		~VulkanCommand() = default;

	public:
		bool Init(Memory::RefPtr<const VulkanDevice> device, const uint32_t bufferCount = 1);
		bool AllocateBuffer(Memory::RefPtr<const VulkanDevice> device, const uint32_t bufferCount = 1);

		void Release(Memory::RefPtr<const VulkanDevice> device);
		void ReleaseBuffer(Memory::RefPtr<const VulkanDevice> device, const size_t index = 0);

		void ResetPool(Memory::RefPtr<const VulkanDevice> device);
		void ResetBuffer(const size_t index = 0);

		bool IsPrimary() const;

		VkCommandBuffer GetCommandBuffer(const size_t index = 0) const;
		uint32_t GetCommandBufferCount() const;

	private :
		VkCommandPool m_commandPool;
		DynamicArray<VkCommandBuffer> m_commandBuffers;

		const bool m_primary;
	};
};

#endif // __WTR_VULKANCOMMAND_H__