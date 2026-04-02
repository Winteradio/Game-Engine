#ifndef __WTR_PRIMTIVEPROXY_H__
#define __WTR_PRIMTIVEPROXY_H__

#include <Renderer/Proxy/SceneProxy.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
	class PrimitiveProxy : public SceneProxy
	{
		GENERATE(PrimitiveProxy);
	public :
		PrimitiveProxy();
		virtual ~PrimitiveProxy();

	public :
	};
}

#endif // __WTR_PRIMTIVEPROXY_H__