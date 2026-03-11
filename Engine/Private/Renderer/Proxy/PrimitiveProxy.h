#ifndef __WTR_PRIMTIVEPROXY_H__
#define __WTR_PRIMTIVEPROXY_H__

#include <Renderer/Proxy/RenderProxy.h>

namespace wtr
{
	class PrimitiveProxy : public RenderProxy
	{
	public:
		PrimitiveProxy();
		virtual ~PrimitiveProxy();

	public:
		virtual void CreateRHI(RHICommandList& commandList) override;
		virtual void UpdateRHI(RHICommandList& commandList) override;
		virtual void ReleaseRHI(RHICommandList& commandList) override;
	};
}

#endif // __WTR_PRIMTIVEPROXY_H__