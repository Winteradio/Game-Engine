#ifndef __WTR_RHIEXECUTOR_H__
#define __WTR_RHIEXECUTOR_H__

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class RHISystem;
	class RHICommandList;
};

namespace wtr
{
	class RHIExecutor
	{
	public :
		RHIExecutor(Memory::RefPtr<RHISystem> system);
		virtual ~RHIExecutor() = default;

	public :
		virtual Memory::RefPtr<RHICommandList> Acquire() = 0;
		virtual void Submit(Memory::RefPtr<RHICommandList> cmdList) = 0;
		virtual void Execute() = 0;

	protected :
		Memory::RefPtr<RHISystem> m_system;
	};
};

#endif // __WTR_RHDEXECUTOR_H__