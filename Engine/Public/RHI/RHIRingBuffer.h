#ifndef __WTR_RHIRINGBUFFER_H__
#define __WTR_RHIRINGBUFFER_H__

#include <Renderer/RenderData.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>

#include <cstdint>

namespace wtr
{
	class RHIContext;
	class RHIBuffer;
};

namespace wtr
{
	// 프레임 단위로 되감는 선형 할당기. 큰 RHIBuffer(청크)를 offset으로 잘라 슬라이스로 내어준다
	// 청크 생성과 데이터 쓰기는 RHIContext의 추상화(CreateBuffer / InitializeBuffer / UpdateBuffer)로 직접 하므로, 사용하는 쪽은 슬라이스만 받아 바인딩하면 된다
	// RHIWorker 한 스레드에서만 쓴다고 가정함
	class RHIRingBuffer
	{
	public :
		struct Slice
		{
			Memory::RefPtr<RHIBuffer>	buffer;

			uint32_t					offset	= 0;
			uint32_t					size	= 0;
		};

	public :
		RHIRingBuffer() = default;
		~RHIRingBuffer() = default;

	public :
		// context : 청크를 만들고 쓰는 데 쓰는 컨텍스트 (이 링을 소유한 RHIContext이므로 링보다 오래 산다)
		// chunkSize : 청크 하나의 바이트 크기, alignment : 슬라이스 offset 정렬 (minUniformBufferOffsetAlignment 등의 최댓값 이상)
		void Init(RHIContext* context, const uint32_t chunkSize, const uint32_t alignment);

		// 청크가 아직 없을 때만 바뀜 (이미 받은 슬라이스의 정렬은 바꿀 수 없음)
		void SetAlignment(const uint32_t alignment);

		// 슬라이스를 받는다. 현재 청크에 자리가 없으면 다음 청크로 넘어가고, 쓸 청크가 없으면 같은 크기의 청크를 새로 만든다
		// 정렬한 크기가 청크보다 크거나 청크를 만들지 못하면 false
		bool Allocate(const uint32_t size, Slice& slice);

		// 슬라이스에 데이터를 쓴다 (UpdateBuffer). data의 [0, slice.size)를 slice.offset 위치에 복사함
		// UpdateBuffer가 호출 안에서 data를 복사한다고 가정하므로, 호출이 끝나면 data를 바꿔도 됨
		bool Write(const Slice& slice, const RefData& data);

		// 프레임 시작(이전 프레임의 fence를 기다린 뒤)에 호출 : 청크는 그대로 두고 커서만 되감는다
		void Reset();

		// 청크를 모두 내어준다 (RHIContext가 RemoveBuffer로 해제)
		wtr::DynamicArray<Memory::RefPtr<RHIBuffer>> Release();

		uint32_t GetChunkSize() const;
		uint32_t GetAlignment() const;
		uint32_t GetChunkCount() const;

	private :
		struct Chunk
		{
			Memory::RefPtr<RHIBuffer>	buffer;

			uint32_t					cursor	= 0;
		};

	private :
		// 현재 청크부터 차례로 자리를 찾는다 (청크를 만들지 않음)
		bool Find(const uint32_t alignedSize, const uint32_t size, Slice& slice);

		bool AddChunk();

	private :
		RHIContext*					m_context	= nullptr;

		wtr::DynamicArray<Chunk>	m_chunks;

		uint32_t					m_current	= 0;
		uint32_t					m_chunkSize	= 0;
		uint32_t					m_alignment	= 1;
	};
};

#endif // __WTR_RHIRINGBUFFER_H__
