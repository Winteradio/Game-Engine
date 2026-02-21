#include <RHI/RHICommandExecutor.h>

namespace wtr
{
	RHICommandExecutor::RHICommandExecutor()
	{}

	RHICommandExecutor::~RHICommandExecutor()
	{}

	RHICommandList& RHICommandExecutor::Acquire()
	{
		static RHICommandList null;
		return null;
	}

	void RHICommandExecutor::Release(RHICommandList&& commandList)
	{
		//m_commandList = std::move(commandList);
	}

	void RHICommandExecutor::Execute(Memory::RefPtr<RHISystem> system)
	{
		//m_commandList.ExecuteAll(system);
	}
}