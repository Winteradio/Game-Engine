#ifndef __WTR_RHIFRAMEEXECUTOR_H__
#define __WTR_RHIFRAMEEXECUTOR_H__

#include <RHI/RHIExecutor.h>

#include <Container/include/DynamicArray.h>

namespace wtr
{
	class RHIFrameExecutor : public RHIExecutor
	{
	public :
		using ListPool = wtr::DynamicArray<Memory::RefPtr<RHICommandList>>;

		RHIFrameExecutor(Memory::RefPtr<RHISystem> system);
		virtual ~RHIFrameExecutor();

	public :
		Memory::RefPtr<RHICommandList> Acquire() override;
		void Submit(Memory::RefPtr<RHICommandList> cmdList) override;
		void Execute() override;

		bool Init(const size_t frameCount);

	private :
		size_t GetNext(const size_t index);
		size_t GetPrev(const size_t index);

	private :
		size_t m_frameCount;

		size_t m_recordIndex;
		size_t m_beginIndex;
		std::atomic<size_t> m_endIndex;

		ListPool m_listPool;
	};
};

#endif // __WTR_RHIFRAMEEXECUTOR_H__