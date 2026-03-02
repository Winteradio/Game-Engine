#include <RHI/RHIExecutor.h>

namespace wtr
{
	RHIExecutor::RHIExecutor(Memory::RefPtr<RHISystem> system)
		: m_system(system)
	{}
};