#ifndef __WTR_GLOBALPIPELINE_H__
#define __WTR_GLOBALPIPELINE_H__

#include <Container/include/HashMap.h>
#include <Container/include/HashSet.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Renderer/RenderResource.h>

#include <cstdint>

namespace wtr
{
	struct RHIPipeLineCreateDesc;

	class RHIPipeLine;
	class RHICommandList;
};

namespace wtr
{
	namespace GlobalPipeLine
	{
		Memory::RefPtr<RHIPipeLine> GetPipeLine(Memory::RefPtr<RHICommandList> cmdList, const RHIPipeLineCreateDesc desc);

		void RemoveAll(Memory::RefPtr<RHICommandList> cmdList);
	};
};

#endif // __WTR_GLOBALPIPELINE_H__