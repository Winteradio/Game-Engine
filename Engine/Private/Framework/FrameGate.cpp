#include <Framework/FrameGate.h>

namespace wtr
{
	FrameGate::FrameGate(const size_t frameLimit)
		: m_frameLimit(frameLimit)
		, m_frameDiff(0)
	{}

	FrameGate::~FrameGate()
	{}

	void FrameGate::Submit()
	{
		const int32_t frameDiff = m_frameDiff.load(std::memory_order_acquire);
		if (frameDiff >= static_cast<int32_t>(m_frameLimit))
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			m_cvProducer.wait(lock, [this]()
				{
					const int32_t frameDiff = m_frameDiff.load(std::memory_order_acquire);
					return frameDiff < static_cast<int32_t>(m_frameLimit);
				}
			);
		}

		m_frameDiff.fetch_add(1, std::memory_order_release);
		m_cvConsumer.notify_one();
	}

	void FrameGate::Acquire()
	{
		const int32_t frameDiff = m_frameDiff.load(std::memory_order_acquire);
		if (frameDiff <= 0)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cvConsumer.wait(lock, [this]()
				{
					const int32_t frameDiff = m_frameDiff.load(std::memory_order_acquire);
					return frameDiff > 0;
				}
			);
		}

		m_frameDiff.fetch_sub(1, std::memory_order_release);
		m_cvProducer.notify_one();
	}

	void FrameGate::NotifyAll()
	{
		m_cvConsumer.notify_all();
		m_cvProducer.notify_all();
	}
}