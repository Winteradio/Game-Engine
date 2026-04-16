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

        /**
         * @brief   Index tracking the next slot to be recorded by Acquire().
         * @details Owned exclusively by the Render Thread.
         *          Advanced by Acquire() before recording, and validated by Submit()
         *          to ensure the submitted command list matches the last acquired slot.
         *          Non-atomic by design — only the Render Thread calls Acquire() and Submit().
         */
        size_t m_recordIndex;

        /**
         * @brief   Index marking the start of the pending execution range.
         * @details Owned exclusively by the RHI Thread.
         *          Updated by Execute() after all pending command lists are processed.
         *          Non-atomic by design — only the RHI Thread calls Execute().
         */
        size_t m_beginIndex;

        /**
         * @brief   Index marking the end of the pending execution range.
         * @details Written by Submit() on the Render Thread, read by Execute() on the RHI Thread.
         *          Declared as atomic to ensure safe cross-thread visibility without a mutex.
         *          Uses acquire/release semantics to guarantee command list writes are visible
         *          to the RHI Thread before it reads this index.
         */
        std::atomic<size_t> m_endIndex;

		ListPool m_listPool;
	};
};

#endif // __WTR_RHIFRAMEEXECUTOR_H__