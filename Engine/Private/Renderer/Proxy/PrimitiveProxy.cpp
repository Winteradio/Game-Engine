#include <Renderer/Proxy/PrimitiveProxy.h>

#include <RHI/RHICommandList.h>
#include <Asset/AssetTypes.h>

namespace wtr
{
	PrimitiveProxy::PrimitiveProxy()
		: SceneProxy()
		, m_refMesh(nullptr)
		, m_refOverrideMaterial(nullptr)
	{
	}

	PrimitiveProxy::~PrimitiveProxy()
	{
	}

	void PrimitiveProxy::SetMesh(Memory::RefPtr<const MeshAsset> refMesh)
	{
		if (refMesh)
		{
			m_refMesh = refMesh;
		}
	}

	void PrimitiveProxy::SetOverrideMaterial(Memory::RefPtr<const MaterialAsset> refMaterial)
	{
		if (refMaterial)
		{
			m_refOverrideMaterial = refMaterial;
		}
	}

	Memory::RefPtr<const MeshAsset> PrimitiveProxy::GetMesh() const
	{
		return m_refMesh;
	}

	Memory::RefPtr<const MaterialAsset> PrimitiveProxy::GetOverrideMaterial() const
	{
		return m_refOverrideMaterial;
	}
}