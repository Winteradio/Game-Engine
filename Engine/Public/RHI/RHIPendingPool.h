#ifndef __WTR_RHIPENDINGPOOL_H__
#define __WTR_RHIPENDINGPOOL_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/HashSet.h>

namespace wtr
{
	class RHIResource;
	class RHIContext;
};

namespace wtr
{
	class RHIPendingPool
	{
	public :
		RHIPendingPool() = default;
		~RHIPendingPool() = default;

	public :
		void Add(Memory::RefPtr<RHIResource> resource);
		void Flush(RHIContext& context);
		bool Empty() const;

	private :
		wtr::HashSet<Memory::RefPtr<RHIResource>> m_resources;
	};
};

#endif // __WTR_RHIPENDINGPOOL_H__
