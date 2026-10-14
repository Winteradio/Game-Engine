#ifndef __WTR_RHICONSTANTPOOL_H__
#define __WTR_RHICONSTANTPOOL_H__

#include <RHI/RHIRingBuffer.h>
#include <Renderer/RenderData.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/HashMap.h>
#include <Container/include/DynamicArray.h>

#include <cstdint>

namespace wtr
{
	class RHIContext;
	class RHIBuffer;

	struct RHIConstDesc;
};

namespace wtr
{
	// 상수 : 이름 있는 UBO 블록(binding 하나)의 멤버를 CPU 섀도에 쓰고 바뀐 경우에만 dirty로 표시한다
	// draw / dispatch 직전에 Flush가 dirty인 블록에 링 슬라이스를 받아 올리고 바인딩한다. 장치 호출은 RHIContext의 추상화로만 한다
	// RHIWorker 한 스레드에서만 쓴다고 가정함 (섀도와 링 상태에 락이 없음)
	class RHIConstantPool
	{
	public :
		RHIConstantPool() = default;
		~RHIConstantPool() = default;

	public :
		// context : 링 청크를 만들고 슬라이스를 바인딩하는 데 쓴다. 슬라이스 offset 정렬은 첫 Flush 때 context의 장치 한도를 읽어 정한다
		// (컨텍스트가 Init되기 전에 등록될 수 있으므로 여기서는 가상 함수를 부르지 않음)
		void Init(RHIContext* context);

		// info.offset / info.blockSize / info.blob이 바뀐 값만 dirty로 표시함
		void Set(const RHIConstDesc& info, const int32_t binding);

		// dirty인 블록은 새 슬라이스를 받아 올리고, 아직 바인딩되지 않은 슬라이스는 바인딩한다
		void Flush();

		// 프레임 시작(이전 프레임 fence를 기다린 뒤)에 호출 : 링을 되감고 모든 블록을 dirty로 되돌린다 (이전 슬라이스는 더 이상 유효하지 않음)
		void Reset();

		// 바인딩이 무효화될 수 있는 때(파이프라인 레이아웃 변경 등)에 호출 : 슬라이스는 그대로 두고 다음 Flush에서 다시 바인딩만 한다
		void Invalidate();

		// 링 청크를 모두 내어준다 (호출한 쪽이 지연 해제로 넘긴다). 블록 상태도 비운다
		wtr::DynamicArray<Memory::RefPtr<RHIBuffer>> Release();

	private :
		// 블록 하나(binding 하나)의 상태
		struct Block
		{
			// CPU 섀도. UpdateBuffer의 blob이 이 데이터를 그대로 가리킴 (UpdateBuffer가 호출 안에서 복사한다고 가정)
			Memory::RefPtr<ArrayData<uint8_t>>	shadow;

			RHIRingBuffer::Slice				slice;

			bool								dirty	= true;		// 섀도가 슬라이스와 다름 (또는 슬라이스가 아직 없음)
			bool								bound	= false;	// 현재 슬라이스가 바인딩되어 있음
		};

	private :
		RHIContext*							m_context = nullptr;
		bool								m_aligned = false;

		RHIRingBuffer						m_ring;
		wtr::HashMap<int32_t, Block>		m_blocks;
	};
};

#endif // __WTR_RHICONSTANTPOOL_H__
