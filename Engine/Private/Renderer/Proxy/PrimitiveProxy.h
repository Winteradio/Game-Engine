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
		void SetMesh(Memory::RefPtr<MeshAsset> refMesh);
		void SetMaterial(Memory::RefPtr<MaterialAsset> refMaterial);

		Memory::RefPtr<MeshAsset> GetMesh();
		Memory::RefPtr<const MeshAsset> GetMesh() const;

		Memory::RefPtr<MaterialAsset> GetMaterial();
		Memory::RefPtr<const MaterialAsset> GetMaterial() const;

	private :
		Memory::RefPtr<MeshAsset> m_refMesh;
		Memory::RefPtr<MaterialAsset> m_refOverrideMaterial;
	};
}

#endif // __WTR_PRIMTIVEPROXY_H__