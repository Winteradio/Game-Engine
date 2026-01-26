#ifndef __WTR_MESHPROXY_H__
#define __WTR_MESHPROXY_H__

#include <Renderer/Proxy/RenderProxy.h>

namespace wtr
{
	class MeshProxy : public RenderProxy
	{
	public :
		MeshProxy();
		virtual ~MeshProxy();

	public :
		virtual void CreateRHI(RHICommandList& commandList) override;
		virtual void UpdateRHI(RHICommandList& commandList) override;
		virtual void ReleaseRHI(RHICommandList& commandList) override;
	};
}

#endif // __WTR_MESHPROXY_H__