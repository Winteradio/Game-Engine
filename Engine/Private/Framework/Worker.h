#ifndef __WTR_WORKER_H__
#define __WTR_WORKER_H__

#include <atomic>
#include <thread>

namespace wtr
{
	enum class eWorkerType : uint8_t
	{
		eProceduer,
		eConsumer,
		eTask,
	};

	class Worker
	{
	public :
		Worker();
		virtual ~Worker();

		Worker(const Worker&) = delete;
		Worker& operator=(const Worker&) = delete;

		Worker(Worker&& other) noexcept;
		Worker& operator=(Worker&& other) noexcept;

	public :
		void Start();
		void Stop();

	protected :
		virtual void onStart() = 0;
		virtual void onUpdate() = 0;
		virtual void onDestroy() = 0;

	private :
		void Run();

	protected :
		std::atomic<bool> m_isRunning;
		std::thread m_thread;
	};
};

#endif // __WTR_WORKER_H__