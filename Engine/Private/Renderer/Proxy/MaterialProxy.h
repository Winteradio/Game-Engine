#ifndef __WTR_MATERIALPROXY_H__
#define __WTR_MATERIALPROXY_H__

#include <Renderer/Proxy/ShaderProxy.h>

#include <Renderer/MaterialDesc.h>
#include <Framework/Math/MathTypes.h>

#include <ECS/include/Object/Object.h>
#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	enum class eTextureSlot : uint8_t;
	enum class eVectorSlot : uint8_t;
	enum class eScalarSlot : uint8_t;

	class ShaderSet;
	class TextureAsset;
	class MaterialAsset;
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

		void SetMaterialAsset(Memory::RefPtr<const MaterialAsset> material);

		const MaterialDesc& GetMaterialDesc() const;
		Memory::RefPtr<const TextureAsset> GetTexture(const eTextureSlot slot) const;
		const fvec3 GetVector(const eVectorSlot slot) const;
		const float GetScalar(const eScalarSlot slot) const;

	private :
		MaterialDesc m_materialDesc;
		Memory::RefPtr<const MaterialAsset> m_materialAsset;
	};
}

#endif // __WTR_MATERIALPROXY_H__