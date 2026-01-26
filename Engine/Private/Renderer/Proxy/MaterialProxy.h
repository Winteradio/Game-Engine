#ifndef __WTR_MATERIALPROXY_H__
#define __WTR_MATERIALPROXY_H__

#include <Renderer/Proxy/RenderProxy.h>

namespace wtr
{
	class MaterialProxy : public RenderProxy
	{
	public:
		MaterialProxy();
		virtual ~MaterialProxy();

	public:
		virtual void CreateRHI(RHICommandList& commandList) override;
		virtual void UpdateRHI(RHICommandList& commandList) override;
		virtual void ReleaseRHI(RHICommandList& commandList) override;
	};
}

#endif // __WTR_MATERIALPROXY_H__