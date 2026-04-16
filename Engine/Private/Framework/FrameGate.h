#ifndef __WTR_FRAMEGATE_H__
#define __WTR_FRAMEGATE_H__

#include <condition_variable>
#include <atomic>
#include <cstdint>
#include <mutex>

namespace wtr
{
	class FrameHandler
	{
	public :
		FrameHandler() = default;
		virtual ~FrameHandler() = default;

	public :
		virtual void NotifyAll() = 0;
	};

	class FrameProducer : virtual public FrameHandler
	{
	public :
		FrameProducer() = default;
		virtual ~FrameProducer() = default;

	public :
		virtual void Submit() = 0;
	};

	class FrameConsumer : virtual public FrameHandler
	{
	public :
		FrameConsumer() = default;
		virtual ~FrameConsumer() = default;

	public :
		virtual void Acquire() = 0;
	};

	class FrameGate : public FrameProducer, public FrameConsumer
	{
	public :
		FrameGate(const size_t frameLimit);
		~FrameGate();

	public :
		void Submit() override;
		void Acquire() override;
		void NotifyAll() override;

	private :
		std::condition_variable m_cvProducer;
		std::condition_variable m_cvConsumer;

		std::mutex m_mutex;
		const size_t m_frameLimit;
		std::atomic<int32_t> m_frameDiff;
		std::atomic<bool> m_notified;
	};
}

#endif // __WTR_FRAMEGATE_H__