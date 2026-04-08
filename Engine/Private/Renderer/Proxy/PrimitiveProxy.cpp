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

	void PrimitiveProxy::SetMesh(Memory::RefPtr<MeshAsset> refMesh)
	{
		if (refMesh)
		{
			m_refMesh = refMesh;
		}
	}

	void PrimitiveProxy::SetOverrideMaterial(Memory::RefPtr<MaterialAsset> refMaterial)
	{
		if (refMaterial)
		{
			m_refOverrideMaterial = refMaterial;
		}
	}

	Memory::RefPtr<MeshAsset> PrimitiveProxy::GetMesh()
	{
		return m_refMesh;
	}

	Memory::RefPtr<const MeshAsset> PrimitiveProxy::GetMesh() const
	{
		return m_refMesh;
	}

	Memory::RefPtr<MaterialAsset> PrimitiveProxy::GetOverrideMaterial()
	{
		return m_refOverrideMaterial;
	}

	Memory::RefPtr<const MaterialAsset> PrimitiveProxy::GetOverrideMaterial() const
	{
		return m_refOverrideMaterial;
	}
}