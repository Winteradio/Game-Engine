#pragma once

#include <Framework/Math/MathTypes.h>

namespace wtr
{
	struct RenderView
	{
		struct Viewport
		{
			uint16_t posX;
			uint16_t posY;
			uint16_t width;
			uint16_t height;
		};

		struct Camera
		{
			fvec3 position;
			fvec3 rotation;

			float fov;
			float nearPlane;
			float farPlane;
		};

		Viewport viewport;
		Camera camera;
	};
}