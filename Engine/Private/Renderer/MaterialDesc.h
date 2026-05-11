#ifndef __WTR_MATERIALDESC_H__
#define __WTR_MATERIALDESC_H__

#include <Renderer/RenderTypes.h>

namespace wtr
{
    struct MaterialDesc
    {
        // Determines the lighting model and GBuffer layout used for this material
        eShadingModel shadingModel = eShadingModel::eLit;

        // Determines which pass renders this material and how blending is applied
        // Default: Opaque (rendered in GeometryPass, no blending)
        eBlendMode blendMode = eBlendMode::eOpaque;

        // Enables two-sided rendering by disabling back-face culling
        // Default: false (back-face culled)
        bool isDoubleSided = false;

        // Selects the shading workflow: true = Metallic/Roughness (PBR), false = Phong (Ambient/Diffuse/Specular)
        // Default: false (Phong workflow, PBR support planned)
        bool isPBR = false;

        // Phong workflow textures (used when isPBR = false)
        bool hasAmbientMap = false;  // Ambient occlusion color map
        bool hasDiffuseMap = false;  // Base color/albedo map
        bool hasSpecularMap = false;  // Specular highlight map

        // Shared textures (used in both PBR and Phong workflows)
        bool hasEmissiveMap = false;  // Self-illumination map
        bool hasOpacityMap = false;  // Alpha mask map (required for Masked blend mode)
        bool hasBumpMap = false;  // Height map for bump offset normal calculation
        bool hasNormalMap = false;  // Tangent-space normal map

        // PBR workflow textures (used when isPBR = true)
        bool hasRoughnessMap = false;  // Surface micro-roughness map (0 = smooth, 1 = rough)
        bool hasMetallicMap = false;  // Metallic surface map (0 = dielectric, 1 = metal)
        bool hasAmbientOcclusionMap = false;  // Baked ambient occlusion map
        bool hasSheenMap = false;  // Sheen BRDF map for cloth and velvet materials

        bool operator==(const MaterialDesc& other) const
        {
            return shadingModel == other.shadingModel &&
                   blendMode == other.blendMode &&
                   isDoubleSided == other.isDoubleSided &&
                   isPBR == other.isPBR &&
                   hasAmbientMap == other.hasAmbientMap &&
                   hasDiffuseMap == other.hasDiffuseMap &&
                   hasSpecularMap == other.hasSpecularMap &&
                   hasEmissiveMap == other.hasEmissiveMap &&
                   hasOpacityMap == other.hasOpacityMap &&
                   hasBumpMap == other.hasBumpMap &&
                   hasNormalMap == other.hasNormalMap &&
                   hasRoughnessMap == other.hasRoughnessMap &&
                   hasMetallicMap == other.hasMetallicMap &&
                   hasAmbientOcclusionMap == other.hasAmbientOcclusionMap &&
                   hasSheenMap == other.hasSheenMap;
		}

        bool operator!=(const MaterialDesc& other) const
        {
            return !(*this == other);
		}
    };

	struct MaterialHasher
	{
		size_t operator()(const MaterialDesc& desc) const
		{
            size_t hash = 0;

            hash = hash | (static_cast<size_t>(desc.shadingModel) << 2);
			hash = hash | (static_cast<size_t>(desc.blendMode) << 2);
			hash = hash | (static_cast<size_t>(desc.isDoubleSided) << 1);
			hash = hash | (static_cast<size_t>(desc.isPBR) << 1);
			hash = hash | (static_cast<size_t>(desc.hasAmbientMap) << 1);
			hash = hash | (static_cast<size_t>(desc.hasDiffuseMap) << 1);
			hash = hash | (static_cast<size_t>(desc.hasSpecularMap) << 1);
			hash = hash | (static_cast<size_t>(desc.hasEmissiveMap) << 1);
			hash = hash | (static_cast<size_t>(desc.hasOpacityMap) << 1);
			hash = hash | (static_cast<size_t>(desc.hasBumpMap) << 1);
			hash = hash | (static_cast<size_t>(desc.hasNormalMap) << 1);
			hash = hash | (static_cast<size_t>(desc.hasRoughnessMap) << 1);
			hash = hash | (static_cast<size_t>(desc.hasMetallicMap) << 1);
			hash = hash | (static_cast<size_t>(desc.hasAmbientOcclusionMap) << 1);
			hash = hash | (static_cast<size_t>(desc.hasSheenMap) << 1);

			return hash;
		}
	};
}

#endif // __WTR_MATERIALDESC_H__