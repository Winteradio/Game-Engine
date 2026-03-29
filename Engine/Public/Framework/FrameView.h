#ifndef __WTR_FRAMEVIEW_H__
#define __WTR_FRAMEVIEW_H__

#include <Framework/Math/MathTypes.h>

namespace wtr
{
	struct FrameView
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
			fvec3 position = fvec3(0.f);
			fvec3 rotation = fvec3(0.f);

			float fov = 45.f;
			float nearPlane = 1.f;
			float farPlane = 1000.f;
		};

		Viewport viewport;
		Camera camera;
	};
};

#endif // __WTR_FRAMEVIEW_H__