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

		Memory::RefPtr<const RHIBuffer> GetVectorBuffer() const;
		Memory::RefPtr<const RHIBuffer> GetScalarBuffer() const;
		Memory::RefPtr<const TextureAsset> GetTexture(const eResourceSlot slot) const;
		Memory::RefPtr<const MaterialAsset> GetMaterial() const;

	private :
		MaterialDesc m_materialDesc;
		Memory::RefPtr<const MaterialAsset> m_materialAsset;
		Memory::RefPtr<RHIBuffer> m_vectorBuffer;
		Memory::RefPtr<RHIBuffer> m_scalarBuffer;
	};
}

#endif // __WTR_MATERIALPROXY_H__