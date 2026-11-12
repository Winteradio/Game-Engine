#ifndef __WTR_RHIRINGBUFFER_H__
#define __WTR_RHIRINGBUFFER_H__

#include <RHI/RHIDescriptions.h>

#include <cstdint>

namespace wtr
{
	class RHIRingBuffer
	{
	public :
		RHIRingBuffer();
		~RHIRingBuffer() = default;

	public :
		void Init(const size_t blockSize, const size_t frameDiff);
		void Reset(const size_t frame);

		Memory::RefPtr<RHIBufferView> GetBuffer(const size_t size, const size_t align = 256);

	private :
		bool IsValid(const size_t blockIndex, const size_t size, const size_t align) const;
		bool Create(const size_t size);

	private :
		struct Block
		{
			Memory::RefPtr<RHIBuffer> buffer;
			size_t offset	= 0;
			size_t frame	= 0;
		};

		wtr::DynamicArray<Memory::RefPtr<RHIBuffer>> m_buffers;

		wtr::DynamicArray<Block> m_blocks;
		
		size_t m_blockSize;
		size_t m_frameDiff;
		size_t m_frame;
	};
};

#endif // __WTR_RHIRINGBUFFER_H__
