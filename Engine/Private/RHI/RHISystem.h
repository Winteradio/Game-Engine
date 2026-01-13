#ifndef __WTR_RHISYSTEM_H__
#define __WTR_RHISYSTEM_H__

#include <RHI/RHICommandList.h>

namespace wtr
{
	class RHISystem
	{
	public :
		RHISystem();
		virtual ~RHISystem();

	public :
		virtual bool Init(void* nativeHandle) = 0;

	protected :
		RHICommandList m_commandList;
	};
};

#endif // __WTR_RHISYSTEM_H__