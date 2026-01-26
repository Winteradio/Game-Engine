#ifndef __WTR_RENDERPROXY_H__
#define __WTR_RENDERPROXY_H__

#include <cstdint>

namespace wtr
{
	class RHICommandBase;
	class RHICommandList;
};

namespace wtr
{
	enum class eProxyState : uint8_t
	{
		eNone = 0x00,
		eCreate = 0x01,
		eUpdate = 0x02,
		eRelease = 0x03
	};

	class RenderProxy
	{
	public :
		RenderProxy();

	public :
		void DoRHI(RHICommandList& commandList);

		virtual void CreateRHI(RHICommandList& commandList) = 0;
		virtual void UpdateRHI(RHICommandList& commandList) = 0;
		virtual void ReleaseRHI(RHICommandList& commandList) = 0;

		void SetState(const eProxyState eState);

	private :
		eProxyState m_eState;
	};
};

#endif // __WTR_RENDERPROXY_H__