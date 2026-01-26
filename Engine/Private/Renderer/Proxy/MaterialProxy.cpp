#include <Renderer/Proxy/MaterialProxy.h>

#include <RHI/RHICommandList.h>

namespace wtr
{
	MaterialProxy::MaterialProxy()
	{}

	MaterialProxy::~MaterialProxy()
	{}

	void MaterialProxy::CreateRHI(RHICommandList& commandList)
	{}

	void MaterialProxy::UpdateRHI(RHICommandList& commandList)
	{}

	void MaterialProxy::ReleaseRHI(RHICommandList& commandList)
	{}
}