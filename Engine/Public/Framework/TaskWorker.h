#ifndef __WTR_TASKWORKER_H__
#define __WTR_TASKWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Worker.h>

#include <mutex>
#include <condition_variable>
#include <functional>

namespace wtr
{
	struct Task
	{
		std::function<void()> func;

		Task(std::function<void()> func);
		void operator()();

		template<typename Logic, typename Callback, typename... Args>
		static Task Make(Logic&& logic, Callback&& callback, Args&&... args)
		{
			auto taskFunc = [taskLogic = std::forward<Logic>(logic), 
				taskCallback = std::forward<Callback>(callback), 
				...taskArgs = std::forward<Args>(args)...]
				() mutable
			{
				auto result = taskLogic(taskArgs...);
				taskCallback(result);
			};

			return Task(taskFunc);
		};
	};

	class TaskWorker : public Worker
	{
	public :
		TaskWorker();
		virtual ~TaskWorker();

	public :
		void Wait();
		void Notify();
		
		void Set(Memory::RefPtr<Task> task);
		
		bool IsRunning() const;
		bool IsJoinable() const;

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		std::condition_variable m_cv;
		std::mutex m_mutex;
		Memory::RefPtr<Task> m_task;
	};
};

#endif // __WTR_TASKWORKER_H__