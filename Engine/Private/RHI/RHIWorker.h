#ifndef __WTR_RHIWORKER_H__
#define __WTR_RHIWORKER_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Framework/Worker.h>

namespace wtr
{
	class RHISystem;
	class RHICommandExecutor;
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
		void SetExecutor(const Memory::RefPtr<RHICommandExecutor> executor);

	public :
		void onStart() override;
		void onUpdate() override;
		void onDestroy() override;

	private :
		Memory::RefPtr<RHISystem> m_refSystem;
		Memory::RefPtr<RHICommandExecutor> m_refExecutor;
	};
};

#endif // __WTR_RHIWORKER_H__