#ifndef __WTR_RHIEXECUTOR_H__
#define __WTR_RHIEXECUTOR_H__

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class RHICommandList;
};

namespace wtr
{
	class RHIExecutor
	{
	public :
		RHIExecutor();
		virtual ~RHIExecutor() = default;

	public :
		virtual Memory::RefPtr<RHICommandList> Acquire() = 0;
		virtual void Submit(Memory::RefPtr<RHICommandList> cmdList) = 0;
		virtual void Execute() = 0;

	};
};

#endif // __WTR_RHDEXECUTOR_H__