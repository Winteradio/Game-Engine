#ifndef __WTR_RHICOMMANDEXECUTOR_H__
#define __WTR_RHICOMMANDEXECUTOR_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <RHI/RHICommandList.h>

namespace wtr
{
	class RHISystem;
};

namespace wtr
{
	class RHICommandExecutor
	{
	public :
		RHICommandExecutor();
		~RHICommandExecutor();

	public :
		RHICommandList& Acquire();
		void Release(RHICommandList&& commandList);
		void Execute(Memory::RefPtr<RHISystem> system);

	private :
	};
};

#endif // __WTR_RHICOMMANDEXECUTOR_H__