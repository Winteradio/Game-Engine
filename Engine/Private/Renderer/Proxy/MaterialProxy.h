#ifndef __WTR_MATERIALPROXY_H__
#define __WTR_MATERIALPROXY_H__

#include <Renderer/Proxy/ShaderProxy.h>

#include <Renderer/MaterialDesc.h>
#include <Framework/Math/MathTypes.h>

#include <ECS/include/Object/Object.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	enum class eResourceSlot : uint8_t;
	enum class eVectorSlot : uint8_t;
	enum class eScalarSlot : uint8_t;

	class ShaderSet;
	class TextureAsset;
	class MaterialAsset;
	class RHIBuffer;
};

namespace wtr
{
	class MaterialProxy : public ECS::Object, public ShaderProxy
	{
	public :
		MaterialProxy(const ECS::UUID& id);
		virtual ~MaterialProxy() = default;

	public :
		eResourceState GetResourceState() const override;

		void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
		void Unload(Memory::RefPtr<RHICommandList> cmdList) override;
		void Sync(Memory::RefPtr<RHICommandList> cmdList) override;

	public :
		void SetMaterialAsset(Memory::RefPtr<const MaterialAsset> material);
		void UpdateMaterialDesc();
		const MaterialDesc& GetMaterialDesc() const;

		Memory::RefPtr<const TextureAsset> GetTexture(const eResourceSlot slot) const;
		Memory::RefPtr<const MaterialAsset> GetMaterial() const;
		Memory::RefPtr<const RHIBuffer> GetMaterialBuffer() const;

	private :
		struct PhongParams
		{
			alignas(16) fvec3 diffuse = fvec3(1.f);
			alignas(16) fvec3 specular = fvec3(1.f);
			alignas(16) fvec3 emissive = fvec3(0.f);
			alignas(4) float shininess = 32.f;
			alignas(4) float opacity = 1.f;
		};

		struct PBRParams
		{
			alignas(16) fvec3 baseColor = fvec3(1.f);
			alignas(4)  float roughness = 0.5f;
			alignas(4)  float metallic = 0.f;
			alignas(4)  float ambientOcclusion = 1.f;
			alignas(4) float opacity = 1.f;
		};

		MaterialDesc m_materialDesc;
		Memory::RefPtr<const MaterialAsset> m_materialAsset;
		Memory::RefPtr<RHIBuffer> m_materialBuffer;
	};
}

#endif // __WTR_MATERIALPROXY_H__