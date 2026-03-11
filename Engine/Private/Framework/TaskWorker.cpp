#include <Framework/TaskWorker.h>

namespace wtr
{
	Task::Task(std::function<void()> func)
		: func(func)
	{}

	void Task::operator()()
	{
		if (func)
		{
			func();
		}
	}

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
		std::unique_lock<std::mutex> lock(m_mutex);

		m_cv.wait(lock, [this]()
		{
			return this->IsRunning();
		});
	}

	void TaskWorker::Notify()
	{
		m_cv.notify_one();
	}

	void TaskWorker::Set(Memory::RefPtr<Task> task)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_task = task ? task : nullptr;

		Notify();
	}

	bool TaskWorker::IsRunning() const
	{
		return m_isRunning;
	}

	bool TaskWorker::IsJoinable() const
	{
		return m_thread.joinable();
	}

	void TaskWorker::onStart()
	{}

	void TaskWorker::onUpdate()
	{
		if (m_task)
		{
			m_task->operator()();
			m_task.Reset();
		}
	}

	void TaskWorker::onDestroy()
	{
		if (m_task)
		{
			m_task.Reset();
		}
	}
};