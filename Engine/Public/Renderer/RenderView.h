#ifndef __WTR_RENDERVIEW_H__
#define __WTR_RENDERVIEW_H__

#include <Framework/Math/MathTypes.h>

namespace wtr
{
	struct RenderView
	{
		struct Viewport
		{
			uint16_t posX = 0;
			uint16_t posY = 0;
			uint16_t width = 0;
			uint16_t height = 0;
		};

		struct Camera
		{
			fmat4 viewMatrix = fmat4(1.f);
			fmat4 projMatrix = fmat4(1.f);

			fvec3 position = fvec3(0.f);
		};

		Viewport viewport;
		Camera camera;
	};
};

#endif // __WTR_RENDERVIEW_H__