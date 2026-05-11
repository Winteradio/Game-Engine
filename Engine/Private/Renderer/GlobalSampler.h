#ifndef __WTR_GLOBALSAMPLER_H__
#define __WTR_GLOBALSAMPLER_H__

#include <Container/include/HashMap.h>
#include <Container/include/HashSet.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Renderer/RenderResource.h>

#include <cstdint>

namespace wtr
{
	struct RHISamplerCreateDesc;

	class RHISampler;
	class RHICommandList;
};

namespace wtr
{
	namespace GlobalSampler
	{
		Memory::RefPtr<RHISampler> GetSampler(Memory::RefPtr<RHICommandList> cmdList, const RHISamplerCreateDesc desc);

		void RemoveAll(Memory::RefPtr<RHICommandList> cmdList);
	};
};

#endif // __WTR_GLOBALSAMPLER_H__