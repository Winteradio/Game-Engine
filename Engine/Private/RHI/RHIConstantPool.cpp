#include <RHI/RHIConstantPool.h>

#include <RHI/RHIContext.h>
#include <RHI/RHIResources.h>
#include <Log/include/Log.h>
#include <Memory/include/Core.h>

#include <cstring>

namespace wtr
{
	namespace
	{
		// 링 청크 하나의 크기. 슬라이스 하나(블록 크기를 정렬한 값)가 이보다 크면 받을 수 없음
		constexpr uint32_t CONSTANT_CHUNK_SIZE = 1u << 20;
	}

	void RHIConstantPool::Init(RHIContext* context)
	{
		m_context = context;
		m_aligned = false;

		// 모든 장치에서 안전한 256으로 시작하고, 첫 Flush에서 장치 한도로 맞춘다
		m_ring.Init(context, CONSTANT_CHUNK_SIZE, 256);
	}

	void RHIConstantPool::Set(const RHIConstDesc& info, const int32_t binding)
	{
		const RefData& bulk = info.blob.bulk;
		if (!bulk || bulk->Empty() || info.blob.offset >= bulk->GetSize())
		{
			LOGWARN() << "[RHIConstantPool] Failed to set the constant, the data is invalid";
			return;
		}

		if (binding < 0 || info.blockSize == 0)
		{
			LOGWARN() << "[RHIConstantPool] Failed to set the constant, the binding or the block size is invalid";
			return;
		}

		// size가 0이면 offset부터 bulk 끝까지
		const uint32_t remainSize = bulk->GetSize() - info.blob.offset;
		const uint32_t dataSize = (info.blob.size == 0 || info.blob.size > remainSize) ? remainSize : info.blob.size;
		if (info.offset + dataSize > info.blockSize)
		{
			LOGERROR() << "[RHIConstantPool] Failed to set the constant, the data exceeds the block, offset : " << info.offset
				<< ", size : " << dataSize << ", block size : " << info.blockSize;
			return;
		}

		Block& block = m_blocks[binding];
		if (!block.shadow)
		{
			block.shadow = Memory::MakeRef<ArrayData<uint8_t>>();
		}

		if (block.shadow->data.Size() != info.blockSize)
		{
			// 처음 쓰거나 블록 크기가 달라짐 : 기존 슬라이스는 맞지 않으므로 새로 받는다
			block.shadow->data.Resize(info.blockSize);
			std::memset(block.shadow->data.Data(), 0, info.blockSize);

			block.dirty = true;
			block.bound = false;
		}

		uint8_t* shadowPointer = block.shadow->data.Data() + info.offset;
		const uint8_t* dataPointer = static_cast<const uint8_t*>(bulk->GetPointer()) + info.blob.offset;

		// 값이 같으면 dirty를 세우지 않아 직전 슬라이스를 그대로 쓴다
		if (std::memcmp(shadowPointer, dataPointer, dataSize) == 0)
		{
			return;
		}

		std::memcpy(shadowPointer, dataPointer, dataSize);

		block.dirty = true;
	}

	void RHIConstantPool::Flush()
	{
		if (!m_context)
		{
			return;
		}

		if (!m_aligned)
		{
			m_ring.SetAlignment(m_context->GetUniformOffsetAlignment());
			m_aligned = true;
		}

		for (auto& [binding, block] : m_blocks)
		{
			if (!block.shadow || block.shadow->data.Empty())
			{
				continue;
			}

			if (block.dirty || !block.slice.buffer)
			{
				const uint32_t size = static_cast<uint32_t>(block.shadow->data.Size());

				RHIRingBuffer::Slice slice;
				if (!m_ring.Allocate(size, slice))
				{
					LOGERROR() << "[RHIConstantPool] Failed to allocate the constant slice, binding : " << binding << ", size : " << size;
					continue;
				}

				if (!m_ring.Write(slice, block.shadow))
				{
					LOGERROR() << "[RHIConstantPool] Failed to write the constant slice, binding : " << binding;
					continue;
				}

				block.slice = slice;
				block.dirty = false;
				block.bound = false;
			}

			if (!block.bound)
			{
				m_context->SetBuffer(block.slice.buffer, binding, block.slice.offset, block.slice.size);

				block.bound = true;
			}
		}
	}

	void RHIConstantPool::Reset()
	{
		m_ring.Reset();

		// 링이 되감겼으므로 이전 프레임의 슬라이스는 다른 데이터로 덮일 수 있음
		for (auto& [binding, block] : m_blocks)
		{
			block.dirty = true;
			block.bound = false;
		}
	}

	void RHIConstantPool::Invalidate()
	{
		for (auto& [binding, block] : m_blocks)
		{
			block.bound = false;
		}
	}

	wtr::DynamicArray<Memory::RefPtr<RHIBuffer>> RHIConstantPool::Release()
	{
		m_blocks.Clear();

		return m_ring.Release();
	}
}
