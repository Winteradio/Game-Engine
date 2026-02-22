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
		RHIExecutor() = default;
		virtual ~RHIExecutor() = default;

	public :
		virtual Memory::RefPtr<RHICommandList> Acquire() = 0;
		virtual void Submit(Memory::RefPtr<RHICommandList> cmdList) = 0;
		virtual void Execute(Memory::RefPtr<RHISystem> system) = 0;
	};
};

#endif // __WTR_RHDEXECUTOR_H__