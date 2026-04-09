#ifndef __WTR_TASKWORKER_H__
#define __WTR_TASKWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Worker.h>
#include <Framework/Task.h>

#include <mutex>
#include <condition_variable>

namespace wtr
{
	class TaskWorker : public Worker
	{
	public :
		TaskWorker();
		virtual ~TaskWorker();

	public :
		void Wait();
		void Notify();
		
		void Set(Memory::RefPtr<DefaultTask> task);
		
		bool IsWaited() const;
		bool IsJoinable() const;

	protected :
		void onStart() override;
		void onUpdate() override;
		void onNotify() override;

	private :
		std::atomic<bool> m_isWaited;
		std::condition_variable m_cv;
		std::mutex m_mutex;
		Memory::RefPtr<DefaultTask> m_task;
	};
};

#endif // __WTR_TASKWORKER_H__