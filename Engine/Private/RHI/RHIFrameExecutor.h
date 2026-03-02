#ifndef __WTR_RHIFRAMEEXECUTOR_H__
#define __WTR_RHIFRAMEEXECUTOR_H__

#include <RHI/RHIExecutor.h>

#include <Container/include/DynamicArray.h>

#include <mutex>
#include <condition_variable>

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

		std::atomic<size_t> m_recordIndex;
		std::atomic<size_t> m_beginIndex;
		std::atomic<size_t> m_endIndex;

		std::mutex m_mutexWriting;
		std::condition_variable m_cvWriting;

		ListPool m_listPool;
	};
};

#endif // __WTR_RHIFRAMEEXECUTOR_H__