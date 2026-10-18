#include <RHI/RHIContext.h>

#include <RHI/RHIDescriptions.h>

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

	RHIBufferView RHIContext::GetRingBuffer(const size_t size, const size_t align)
	{
		return m_ringBuffer.GetBuffer(size, align);
	}

	void RHIContext::ResetRingBuffer(const size_t frame)
	{
		m_ringBuffer.Reset(frame);
	}
}