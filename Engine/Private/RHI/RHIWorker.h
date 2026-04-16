#ifndef __WTR_RHIWORKER_H__
#define __WTR_RHIWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Worker.h>

namespace wtr
{
	class RHISystem;
	class RHIExecutor;

	class FrameConsumer;
};

namespace wtr
{
	class RHIWorker : public Worker
	{
	public :
		RHIWorker();
		~RHIWorker();

	public :
		void SetSystem(const Memory::RefPtr<RHISystem> rhiSystem);
		void SetFrameExecutor(const Memory::RefPtr<RHIExecutor> executor);
		void SetTaskExecutor(const Memory::RefPtr<RHIExecutor> executor);
		void SetConsumer(const Memory::RefPtr<FrameConsumer> consumer);

	public :
		bool onStart() override;
		void onUpdate() override;
		void onNotify() override;

	private :
		Memory::RefPtr<RHISystem> m_refSystem;
		Memory::RefPtr<RHIExecutor> m_refFrameExecutor;
		Memory::RefPtr<RHIExecutor> m_refTaskExecutor;
		Memory::RefPtr<FrameConsumer> m_refConsumer;
	};
};

#endif // __WTR_RHIWORKER_H__