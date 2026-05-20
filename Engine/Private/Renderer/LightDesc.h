#ifndef __WTR_LIGHTDESC_H__
#define __WTR_LIGHTDESC_H__

#include <Renderer/RenderTypes.h>

namespace wtr
{
	struct LightDesc
	{
		// Determines the light volume geometry and attenuation model used in LightingPass
		eLightType lightType = eLightType::eNone;

		// Determines the shadow edge filtering algorithm applied in LightingPass
		// Default: None (no shadow calculation)
		eShadowType shadowType = eShadowType::eNone;

		// Splits shadow map into multiple cascades based on camera distance
		// Valid only for Directional lights
		// Default: false (single shadow map)
		bool useCascade = false;

		bool operator==(const LightDesc&) const;
	};

	struct LightHasher
	{
		size_t operator()(const LightDesc& desc) const
		{
			size_t hash = 0;
			hash = hash | (static_cast<size_t>(desc.lightType) << 2);
			hash = hash | (static_cast<size_t>(desc.shadowType) << 2);
			hash = hash | (static_cast<size_t>(desc.useCascade) << 1);
			return hash;
		}
	};
}

#endif // __WTR_LIGHTDESC_H__