#ifndef __WTR_COMPONENT_H__
#define __WTR_COMPONENT_H__

#include <ECS/include/Object/Data.h>
#include <Framework/Math/MathTypes.h>
#include <Asset/AssetTypes.h>

namespace wtr
{
	struct TransformComponent : ECS::Component
	{
		fvec3 position = fvec3(0.f);
		fvec3 rotation = fvec3(0.f);
		fvec3 scale = fvec3(1.f);
	};

	struct ColorComponent : ECS::Component
	{
		float red = 1.f;
		float blue = 1.f;
		float green = 1.f;
		float alpha = 1.f;
	};

	struct CameraComponent : ECS::Component
	{
		float fov = 45.f;

		float near = 1.f;
		float far = 1000.f;

		float width = 1080.f;
		float height = 800.f;

		bool perspective = true;
	};

	struct LightComponent : ECS::Component
	{
		fvec3 direction = fvec3(0.f);
	};

	struct MeshComponent : ECS::Component
	{
		Memory::RefPtr<MeshAsset> mesh;
	};
};

#endif // __WTR_COMPONENT_H__