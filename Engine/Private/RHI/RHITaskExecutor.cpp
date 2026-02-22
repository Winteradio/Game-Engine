#include <RHI/RHITaskExecutor.h>

#include <RHI/RHITaskExecutor.h>

#include <Memory/include/Core.h>

#include <RHI/RHISystem.h>
#include <RHI/RHICommandList.h>

#include <thread>
#include <Log/include/Log.h>

namespace wtr
{
	RHITaskExecutor::RHITaskExecutor()
	{}

	RHITaskExecutor::~RHITaskExecutor()
	{}

	Memory::RefPtr<RHICommandList> RHITaskExecutor::Acquire()
	{
		Memory::RefPtr<RHICommandList> cmdList;
		return cmdList;
	}

	void RHITaskExecutor::Submit(Memory::RefPtr<RHICommandList> cmdList)
	{
	}

	void RHITaskExecutor::Execute(Memory::RefPtr<RHISystem> system)
	{
	}
}