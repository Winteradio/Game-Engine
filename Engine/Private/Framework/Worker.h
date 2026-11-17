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

		// Inline: run on the caller's thread, driven by Tick(). Set it before Start().
		void SetInline(const bool isInline);
		void Tick();

	protected :
		virtual bool onStart();
		virtual void onUpdate();
		virtual void onDestroy();
		virtual void onNotify();

	private :
		void Run();

	protected :
		std::atomic<bool> m_isRunning;
		bool m_isInline = false;
		std::thread m_thread;
	};
};

#endif // __WTR_WORKER_H__