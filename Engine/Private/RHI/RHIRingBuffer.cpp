#include <RHI/RHIRingBuffer.h>

#include <RHI/RHISystem.h>
#include <RHI/RHIResources.h>
#include <Log/include/Log.h>

#include <algorithm>

#define ALIGNSIZE(offset, align) ((align != 0) ? ((offset + align - 1) / align) * align : offset)

namespace wtr
{
	RHIRingBuffer::RHIRingBuffer()
		: m_blockSize(0)
		, m_frameDiff(0)
		, m_frame(0)
	{}

	void RHIRingBuffer::Init(const size_t blockSize, const size_t frameDiff)
	{
		m_blockSize = blockSize;
		m_frameDiff = frameDiff;
	}

	void RHIRingBuffer::Reset(const size_t frame)
	{
		m_frame = frame;
		
		auto itr = m_blocks.begin();
		while (itr != m_blocks.end())
		{
			auto& block = *itr;
			const bool needRemoved = (frame - itr->frame) > m_frameDiff;
			if (needRemoved)
			{
				RHISystem::RemoveBuffer(block.buffer);
				itr = m_blocks.Erase(itr);
			}
			else
			{
				block.offset = 0;
				itr++;
			}
		}
	}

	RHIBufferView RHIRingBuffer::GetBuffer(const size_t size, const size_t align)
	{ 
		if (size == 0)
		{
			return {};
		}

		size_t blockIndex = -1;
		for (size_t index = 0; index < m_blocks.Size(); index++)
		{
			if (IsValid(index, size, align))
			{
				blockIndex = index;
				break;
			}
		}

		if (blockIndex >= m_blocks.Size())
		{
			if (!Create(size))
			{
				LOGERROR() << "[RHI][RingBuffer] Failed to create buffer of size " << size << ".";
				return {};
			}

			blockIndex = m_blocks.Size() - 1;
		}

		auto& block = m_blocks[blockIndex];
		const size_t alignedOffset = ALIGNSIZE(block.offset, align);

		RHIBufferView view;
		view.buffer = block.buffer;
		view.offset = alignedOffset;
		view.range = size;

		block.offset = alignedOffset + size;
		block.frame = m_frame;

		return view;
	}

	bool RHIRingBuffer::IsValid(const size_t blockIndex, const size_t size, const size_t align) const
	{
		if (blockIndex >= m_blocks.Size())
		{
			return false;
		}

		const auto& block = m_blocks[blockIndex];
		if (!block.buffer)
		{
			return false;
		}

		const size_t offset = ALIGNSIZE(block.offset, align);
		if ((offset + size) > block.buffer->GetSize())
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool RHIRingBuffer::Create(const size_t size)
	{
		if (size == 0)
		{
			LOGERROR() << "[RHI][RingBuffer] Invalid size to create buffer.";
			return false;
		}

		RHIBufferCreateDesc desc;
		desc.size = std::max(size, m_blockSize);

		Memory::RefPtr<RHIBuffer> buffer = RHISystem::CreateBuffer(desc);
		if (!buffer)
		{
			LOGERROR() << "[RHI][RingBuffer] Failed to create buffer of size " << desc.size << ".";
			return false;
		}

		RHISystem::InitializeBuffer(buffer, desc);
		if (buffer->GetState() == eResourceState::eError)
		{
			RHISystem::RemoveBuffer(buffer);

			LOGERROR() << "[RHI][RingBuffer] Failed to initialize buffer of size " << desc.size << ".";
			return false;
		}

		Block block;
		block.buffer = buffer;
		block.offset = 0;
		block.frame = 0;

		m_blocks.PushBack(block);
		
		return true;
	}
}
