#include <Renderer/Proxy/PrimitiveProxy.h>

#include <Renderer/MeshBatch.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIResources.h>
#include <Asset/AssetTypes.h>

#include <Memory/include/Core.h>

namespace wtr
{
	PrimitiveProxy::PrimitiveProxy(const ECS::UUID& id)
		: SceneProxy(id)
		, m_refMesh(nullptr)
		, m_refOverrideMaterial(nullptr)
	{
	}

	bool PrimitiveProxy::IsUploadable() const
	{
		if (!m_refMesh || m_refMesh->GetResourceState() != eResourceState::eReady)
		{
			return false;
		}

		if (m_refOverrideMaterial && m_refOverrideMaterial->GetResourceState() != eResourceState::eReady)
		{
			return false;
		}

		return true;
	}

	void PrimitiveProxy::SetMesh(Memory::RefPtr<const MeshAsset> refMesh)
	{
		if (refMesh && !m_refMesh)
		{
			m_refMesh = refMesh;

			this->OnUpdate();
		}
	}

	void PrimitiveProxy::SetOverrideMaterial(Memory::RefPtr<const MaterialAsset> refOverrideMaterial)
	{
		if (refOverrideMaterial && !m_refOverrideMaterial)
		{
			m_refOverrideMaterial = refOverrideMaterial;

			this->OnUpdate();
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

	SinglePrimitiveProxy::SinglePrimitiveProxy(const ECS::UUID& id)
		: PrimitiveProxy(id)
		, m_transformData(Memory::MakeRef<ScalarData<ftransform>>())
	{}

	Memory::RefPtr<const RawData> SinglePrimitiveProxy::GetRawData() const
	{
		return m_transformData;
	}

	bool SinglePrimitiveProxy::IsSyncable() const
	{
		return m_transformData;
	}

	StaticPrimitiveProxy::StaticPrimitiveProxy(const ECS::UUID& id)
		: SinglePrimitiveProxy(id)
	{
	}

	void StaticPrimitiveProxy::UpdateBatch(Memory::RefPtr<MeshBatch> meshBatch)
	{
		if (!meshBatch || !m_transformData)
		{
			return;
		}

		const bool transformDirty = (this->GetDirty() & eRenderDirty::eTransform) == eRenderDirty::eTransform;
		if (transformDirty)
		{
			m_transformData->data = GetTransform();

			meshBatch->UpdateTransform(this->GetID());
		}
	}

	DynamicPrimitiveProxy::DynamicPrimitiveProxy(const ECS::UUID& id)
		: SinglePrimitiveProxy(id)
	{
	}

	void DynamicPrimitiveProxy::UpdateBatch(Memory::RefPtr<MeshBatch> meshBatch)
	{
		if (!meshBatch || !m_transformData)
		{
			return;
		}

		const bool transformDirty = (this->GetDirty() & eRenderDirty::eTransform) == eRenderDirty::eTransform;
		if (transformDirty)
		{
			m_transformData->data = GetTransform();

			meshBatch->UpdateTransform(this->GetID());
		}

		const bool meshDirty = (this->GetDirty() & eRenderDirty::eMesh) == eRenderDirty::eMesh;
		if (meshDirty)
		{
			meshBatch->UpdateMesh();
		}
	}

	void DynamicPrimitiveProxy::UpdateMesh(Memory::RefPtr<const MeshAsset> refMesh)
	{
		if (refMesh)
		{
			m_refMesh = refMesh;

			const bool needDirty = this->GetDirty() != eRenderDirty::eMesh;
			if (needDirty)
			{
				this->SetDirty(eRenderDirty::eMesh);
				this->OnUpdate();
			}
		}
	}

	InstancedStaticPrimitiveProxy::InstancedStaticPrimitiveProxy(const ECS::UUID& id)
		: PrimitiveProxy(id)
		, m_transformData(Memory::MakeRef<ArrayData<ftransform>>())
	{
	}

	Memory::RefPtr<const RawData> InstancedStaticPrimitiveProxy::GetRawData() const
	{
		return m_transformData;
	}

	bool InstancedStaticPrimitiveProxy::IsSyncable() const
	{
		return m_transformData;
	}

	void InstancedStaticPrimitiveProxy::UpdateBatch(Memory::RefPtr<MeshBatch> meshBatch)
	{
		if (!meshBatch || !m_transformData)
		{
			return;
		}

		const bool transformDirty = (this->GetDirty() & eRenderDirty::eTransform) == eRenderDirty::eTransform;
		if (transformDirty)
		{
			meshBatch->UpdateTransform(this->GetID());
		}
	}

	void InstancedStaticPrimitiveProxy::SetInstanceCount(const size_t instanceCount)
	{
		if (!m_transformData)
		{
			return;
		}

		if (m_transformData->data.Size() != instanceCount)
		{
			m_transformData->data.Resize(instanceCount);

			const bool needDirty = this->GetDirty() != eRenderDirty::eTransform;
			if (needDirty)
			{
				this->SetDirty(eRenderDirty::eTransform);
				this->OnUpdate();
			}
		}
	}

	void InstancedStaticPrimitiveProxy::UpdateTransform(const size_t instanceIndex, const ftransform& transform)
	{
		if (!m_transformData || instanceIndex >= m_transformData->data.Size())
		{
			return;
		}

		m_transformData->data[instanceIndex] = transform;

		const bool needDirty = this->GetDirty() != eRenderDirty::eTransform;
		if (needDirty)
		{
			this->SetDirty(eRenderDirty::eTransform);
			this->OnUpdate();
		}
	}

	const size_t InstancedStaticPrimitiveProxy::GetInstanceCount() const
	{
		return m_transformData ? m_transformData->data.Size() : 0;
	}
}