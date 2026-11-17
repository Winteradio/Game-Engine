#include <Framework/Worker.h>

#include <Log/include/Log.h>

namespace wtr
{
	Worker::Worker()
		: m_isRunning{ false }
		, m_thread()
	{
	}

	Worker::~Worker()
	{
	}

	Worker::Worker(Worker&& other) noexcept
		: m_isRunning(other.m_isRunning.load(std::memory_order_relaxed))
		, m_thread(std::move(other.m_thread))
	{
		other.m_isRunning.store(false, std::memory_order_relaxed);
	}

	Worker& Worker::operator=(Worker&& other) noexcept
	{
		if (this != &other)
		{
			m_isRunning.store(other.m_isRunning.load(std::memory_order_relaxed), std::memory_order_relaxed);
			m_thread = std::move(other.m_thread);
			other.m_isRunning.store(false, std::memory_order_relaxed);
		}
		return *this;
	}

	void Worker::Start()
	{
		if (m_isRunning)
		{
			return;
		}

		if (m_isInline)
		{
			m_isRunning = onStart();
			return;
		}

		m_thread = std::thread(&Worker::Run, this);

		m_isRunning = true;
	}

	void Worker::Stop()
	{
		const bool wasRunning = m_isRunning;

		m_isRunning = false;

		onNotify();

		if (m_isInline)
		{
			if (wasRunning)
			{
				onDestroy();
			}

			return;
		}

		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}

	void Worker::SetInline(const bool isInline)
	{
		m_isInline = isInline;
	}

	void Worker::Tick()
	{
		if (m_isInline && m_isRunning)
		{
			onUpdate();
		}
	}

	void Worker::Run()
	{
		if (!onStart())
		{
			return;
		}

		while (m_isRunning)
		{
			onUpdate();
		}

		onDestroy();
	}

	bool Worker::onStart()
	{
		return true;
	}

	void Worker::onUpdate()
	{}

	void Worker::onDestroy()
	{}

	void Worker::onNotify()
	{}
}