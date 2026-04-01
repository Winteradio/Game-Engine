#include <Framework/TaskWorker.h>

#include <Log/include/Log.h>

namespace wtr
{
	TaskWorker::TaskWorker()
		: Worker()
		, m_mutex()
		, m_cv()
		, m_task()
	{}

	TaskWorker::~TaskWorker()
	{}

	void TaskWorker::Wait()
	{
		m_isWaited = true;
	}

	void TaskWorker::Notify()
	{
		m_isWaited = false;

		m_cv.notify_all();
	}

	void TaskWorker::Set(Memory::RefPtr<Task> task)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_task = task ? task : nullptr;

		Notify();
	}

	bool TaskWorker::IsWaited() const
	{
		return m_isWaited;
	}

	bool TaskWorker::IsJoinable() const
	{
		return m_thread.joinable();
	}

	void TaskWorker::onStart()
	{
		LOGINFO() << "[Worker] The task worker started" << " Thread ID: " << std::hash<std::thread::id>()(m_thread.get_id());
	}

	void TaskWorker::onUpdate()
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			m_cv.wait(lock, [this]()
			{
				return !this->m_isRunning || (this->m_task != nullptr && !this->m_isWaited);
			}
			);
		}

		if (m_task)
		{
			m_task->operator()();
			m_task.Reset();

			Wait();
		}
	}

	void TaskWorker::onDestroy()
	{
		if (m_task)
		{
			m_task.Reset();
		}

		Notify();

		LOGINFO() << "[Worker] The task worker destroyed" << " Thread ID: " << std::hash<std::thread::id>()(m_thread.get_id());
	}
};