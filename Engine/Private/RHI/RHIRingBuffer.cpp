#include <RHI/RHIRingBuffer.h>

#include <RHI/RHIContext.h>
#include <RHI/RHIResources.h>
#include <Log/include/Log.h>

namespace wtr
{
	namespace
	{
		uint32_t AlignUp(const uint32_t value, const uint32_t alignment)
		{
			return (value + alignment - 1) / alignment * alignment;
		}
	}

	void RHIRingBuffer::Init(RHIContext* context, const uint32_t chunkSize, const uint32_t alignment)
	{
		m_context = context;
		m_chunkSize = chunkSize;
		m_alignment = alignment == 0 ? 1 : alignment;
	}

	void RHIRingBuffer::SetAlignment(const uint32_t alignment)
	{
		if (m_chunks.Empty())
		{
			m_alignment = alignment == 0 ? 1 : alignment;
		}
	}

	bool RHIRingBuffer::Allocate(const uint32_t size, Slice& slice)
	{
		if (!m_context || size == 0 || m_chunkSize == 0)
		{
			return false;
		}

		// 정렬한 크기가 청크보다 크면 청크를 아무리 추가해도 받을 수 없음
		const uint32_t alignedSize = AlignUp(size, m_alignment);
		if (alignedSize > m_chunkSize)
		{
			return false;
		}

		if (Find(alignedSize, size, slice))
		{
			return true;
		}

		// 자리가 없음 : 같은 크기의 청크를 하나 더 만든다 (쓰는 중인 버퍼는 리사이즈할 수 없음)
		if (!AddChunk())
		{
			return false;
		}

		return Find(alignedSize, size, slice);
	}

	bool RHIRingBuffer::Write(const Slice& slice, const RefData& data)
	{
		if (!m_context || !slice.buffer || !data || data->Empty() || data->GetSize() < slice.size)
		{
			return false;
		}

		DataBlob blob;
		blob.bulk = data;
		blob.offset = 0;
		blob.size = slice.size;

		RHIBufferUpdateDesc update;
		update.dataUsage = eDataUsage::eDynamic;
		update.size = slice.size;
		update.offset = slice.offset;
		update.blobs.PushBack(blob);

		m_context->UpdateBuffer(update, slice.buffer);

		return true;
	}

	void RHIRingBuffer::Reset()
	{
		for (size_t index = 0; index < m_chunks.Size(); ++index)
		{
			m_chunks[index].cursor = 0;
		}

		m_current = 0;
	}

	wtr::DynamicArray<Memory::RefPtr<RHIBuffer>> RHIRingBuffer::Release()
	{
		wtr::DynamicArray<Memory::RefPtr<RHIBuffer>> buffers;
		for (size_t index = 0; index < m_chunks.Size(); ++index)
		{
			buffers.PushBack(m_chunks[index].buffer);
		}

		m_chunks.Clear();
		m_current = 0;

		return buffers;
	}

	uint32_t RHIRingBuffer::GetChunkSize() const
	{
		return m_chunkSize;
	}

	uint32_t RHIRingBuffer::GetAlignment() const
	{
		return m_alignment;
	}

	uint32_t RHIRingBuffer::GetChunkCount() const
	{
		return static_cast<uint32_t>(m_chunks.Size());
	}

	bool RHIRingBuffer::Find(const uint32_t alignedSize, const uint32_t size, Slice& slice)
	{
		// 자리가 없으면 다음 청크로 넘어가고 앞 청크의 남은 자리는 이번 프레임에는 버림
		while (m_current < m_chunks.Size())
		{
			Chunk& chunk = m_chunks[m_current];
			if (chunk.cursor + alignedSize <= m_chunkSize)
			{
				slice.buffer = chunk.buffer;
				slice.offset = chunk.cursor;
				slice.size = size;

				chunk.cursor += alignedSize;

				return true;
			}

			++m_current;
		}

		return false;
	}

	bool RHIRingBuffer::AddChunk()
	{
		RHIBufferDesc desc;
		desc.dataUsage = eDataUsage::eDynamic;
		desc.componentType = eDataType::eNone;
		desc.size = m_chunkSize;
		desc.componentCount = 1;
		desc.stride = 0;

		Memory::RefPtr<RHIBuffer> buffer = m_context->CreateBuffer(desc);
		if (!buffer)
		{
			LOGERROR() << "[RHIRingBuffer] Failed to create the chunk buffer, size : " << m_chunkSize;
			return false;
		}

		m_context->InitializeBuffer(RHIBufferCreateDesc(desc), buffer);

		Chunk chunk;
		chunk.buffer = buffer;

		m_chunks.PushBack(chunk);

		return true;
	}
}
