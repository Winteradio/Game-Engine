#ifndef __WTR_PRIMTIVEPROXY_H__
#define __WTR_PRIMTIVEPROXY_H__

#include <Renderer/Proxy/SceneProxy.h>
#include <Framework/Math/MathTypes.h>

#include <Memory/include/Pointer/RefPtr.h>

namespace wtr
{
	class MeshAsset;
	class MaterialAsset;
};

namespace wtr
{
	class PrimitiveProxy : public SceneProxy
	{
		GENERATE(PrimitiveProxy);
	public :
		PrimitiveProxy();
		virtual ~PrimitiveProxy();

	public :
		void SetMesh(Memory::RefPtr<const MeshAsset> refMesh);
		void SetOverrideMaterial(Memory::RefPtr<const MaterialAsset> refMaterial);

		Memory::RefPtr<const MeshAsset> GetMesh() const;
		Memory::RefPtr<const MaterialAsset> GetOverrideMaterial() const;

	private :
		Memory::RefPtr<const MeshAsset> m_refMesh;
		Memory::RefPtr<const MaterialAsset> m_refOverrideMaterial;
	};

	struct PrimitiveProxyHasher
	{
		size_t operator()(const PrimitiveProxy& proxy) const
		{
			return std::hash<ECS::UUID>()(proxy.GetID());
		}

		size_t operator()(const Memory::RefPtr<PrimitiveProxy>& proxy) const
		{
			return std::hash<ECS::UUID>()(proxy ? proxy->GetID() : ECS::UUID::Null());
		}
	};
}

#endif // __WTR_PRIMTIVEPROXY_H__