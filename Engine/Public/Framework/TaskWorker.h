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
		static Memory::RefPtr<Task> Make(Logic&& logic, Callback&& callback, Args&&... args)
		{
			auto taskFunc = [taskLogic = std::forward<Logic>(logic),
				taskCallback = std::forward<Callback>(callback),
				argsTuple = std::make_tuple(std::forward<Args>(args)...)]() mutable
				{
					if constexpr (std::is_void_v<std::invoke_result_t<Logic, Args...>>) {

						std::apply(taskLogic, std::move(argsTuple));
						taskCallback();

					}
					else 
					{
						auto result = std::apply(taskLogic, std::move(argsTuple));
						taskCallback(result);
					}
				};

			return Memory::MakeRef<Task>(taskFunc);
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
		
		bool IsWaited() const;
		bool IsJoinable() const;

	protected :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		std::atomic<bool> m_isWaited;
		std::condition_variable m_cv;
		std::mutex m_mutex;
		Memory::RefPtr<Task> m_task;
	};
};

#endif // __WTR_TASKWORKER_H__