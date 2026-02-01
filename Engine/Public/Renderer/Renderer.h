#ifndef __WTR_RENDERER_H__
#define __WTR_RENDERER_H__

#include <Renderer/RenderTypes.h>

namespace wtr
{
	class FrameView;
};

namespace wtr
{
	class Renderer
	{
	public :
		Renderer();
		~Renderer();

	public :
		bool Init(const RenderDesc& desc, void* nativeHandle);
		void Render(FrameView& frame);
	};
};

#endif // __WTR_RENDERER_H__