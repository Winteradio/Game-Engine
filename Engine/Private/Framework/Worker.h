#ifndef __WTR_WORKER_H__
#define __WTR_WORKER_H__

#include <atomic>
#include <thread>

namespace wtr
{
	class Worker
	{
	public :
		Worker();
		virtual ~Worker();

	public :
		void Start();
		void Stop();

	protected :
		virtual void onStart() = 0;
		virtual void onUpdate() = 0;
		virtual void onDestroy() = 0;

	private :
		void Run();

	private :
		std::atomic<bool> m_isRunning;
		std::thread m_thread;
	};
};

#endif // __WTR_WORKER_H__