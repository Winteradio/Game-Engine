#include <RHI/RHIContext.h>

#include <RHI/RHIDescriptions.h>
#include <RHI/RHIResources.h>
#include <Log/include/Log.h>

namespace wtr
{
	RHIContext::RHIContext()
		: m_ringBuffer()
	{}

	bool RHIContext::InitResources(const size_t frameDiff)
	{
		constexpr size_t BLOCK_SIZE = 1024;
		m_ringBuffer.Init(BLOCK_SIZE, frameDiff);

		return true;
	}

	bool RHIContext::ReleaseBufferView(Memory::RefPtr<RHIBufferView> bufferView)
	{
		if (!bufferView)
		{
			LOGWARN() << "[RHI] Failed to release the buffer view, the buffer view is invalid";
			return false;
		}

		bufferView->SetState(eResourceState::eNone);

		return true;
	}

	bool RHIContext::ReleaseTextureView(Memory::RefPtr<RHITextureView> textureView)
	{
		if (!textureView)
		{
			LOGWARN() << "[RHI] Failed to release the texture view, the texture view is invalid";
			return false;
		}

		textureView->SetState(eResourceState::eNone);

		return true;
	}

	Memory::RefPtr<RHIBufferView> RHIContext::GetRingBuffer(const size_t size, const size_t align)
	{
		return m_ringBuffer.GetBuffer(size, align);
	}

	void RHIContext::ResetRingBuffer(const size_t frame)
	{
		m_ringBuffer.Reset(frame);
	}
}