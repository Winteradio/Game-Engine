#include <Renderer/Proxy/RenderProxy.h>

namespace wtr
{
	RenderProxy::RenderProxy()
		: m_eState(eProxyState::eNone)
	{}

	void RenderProxy::DoRHI(RHICommandList& commandList)
	{
		if (eProxyState::eCreate == m_eState)
		{
			CreateRHI(commandList);
		}
		else if (eProxyState::eUpdate == m_eState)
		{
			UpdateRHI(commandList);
		}
		else if (eProxyState::eRelease == m_eState)
		{
			ReleaseRHI(commandList);
		}
		else
		{
			// nothing
		}
	}

	void RenderProxy::SetState(const eProxyState eState)
	{
		m_eState = eState;
	}
}