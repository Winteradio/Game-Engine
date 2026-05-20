#ifndef __WTR_RENDERRESOURCE_H__
#define __WTR_RENDERRESOURCE_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <cstdint>

namespace wtr
{
	enum class eRenderPass : uint8_t;
	enum class eResourceState : uint8_t;

	class PipeLineState;
	class RHICommandList;
};

namespace wtr
{
	class RenderState
	{
	public:
		RenderState() = default;
		virtual ~RenderState() = default;

	public :
		virtual eResourceState GetResourceState() const = 0;
	};

	class RenderResource : public RenderState
	{
	public:
		RenderResource() = default;
		virtual ~RenderResource() = default;

	public:
		virtual void Upload(Memory::RefPtr<RHICommandList> cmdList) {};
		virtual void Unload(Memory::RefPtr<RHICommandList> cmdList) {};
		virtual void Sync(Memory::RefPtr<RHICommandList> cmdList) {};
	};
};

#endif // __WTR_RENDERRESOURCE_H__