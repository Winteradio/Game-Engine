#ifndef __WTR_COMPONENT_H__
#define __WTR_COMPONENT_H__

#include <ECS/include/Object/Data.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
	namespace Component
	{
		struct Transform : ECS::Component::Base
		{
			fvec3 position = fvec3(0.f);
			fvec3 rotation = fvec3(0.f);
			fvec3 scale = fvec3(1.f);
		};

		struct Color : ECS::Component::Base
		{
			float red = 1.f;
			float blue = 1.f;
			float green = 1.f;
			float alpha = 1.f;
		};

		struct Camera : ECS::Component::Base
		{
			float fov = 45.f;

			float near = 1.f;
			float far = 1000.f;

			float width = 1080.f;
			float height = 800.f;
		};

		struct Resource : ECS::Component::Base
		{
			ECS::UUID id;
		};

		struct Light : ECS::Component::Base
		{
			fvec3 direction = fvec3(0.f);
		};
	}
};

#endif // __WTR_COMPONENT_H__