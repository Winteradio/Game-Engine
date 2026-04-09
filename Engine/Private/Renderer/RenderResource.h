#ifndef __WTR_RENDERRESOURCE_H__
#define __WTR_RENDERRESOURCE_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <cstdint>

namespace wtr
{
	enum class eResourceState : uint8_t;
	class RHICommandList;
};

namespace wtr
{
	class RenderResource
	{
	public:
		RenderResource() = default;
		virtual ~RenderResource() = default;

	public:
		virtual bool Init() = 0;
		virtual void Upload(Memory::RefPtr<RHICommandList> cmdList) {};
		virtual void Unload(Memory::RefPtr<RHICommandList> cmdList) {};
		virtual void Sync(Memory::RefPtr<RHICommandList> cmdList) {};
		virtual eResourceState GetResourceState() const = 0;
	};
};

#endif // __WTR_RENDERRESOURCE_H__