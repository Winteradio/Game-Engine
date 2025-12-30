#include <Framework/Worker.h>

namespace wtr
{
	Worker::Worker()
		: m_isRunning{false}
		, m_thread()
	{}

	Worker::~Worker()
	{}

	void Worker::Start()
	{
		if (m_isRunning)
		{
			return;
		}

		m_thread = std::thread(&Worker::Run, this);
		m_isRunning = true;
	}

	void Worker::Stop()
	{
		if (!m_isRunning)
		{
			return;
		}

		if (m_thread.joinable())
		{
			m_thread.join();
			m_isRunning = false;
		}
	}

	void Worker::Run()
	{
		onStart();

		while (m_isRunning)
		{
			onUpdate();
		}

		onDestroy();
	}
}