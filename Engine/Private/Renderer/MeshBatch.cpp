#include <Renderer/MeshBatch.h>

#include <Asset/AssetTypes.h>
#include <Memory/include/Core.h>
#include <RHI/RHIResources.h>
#include <RHI/RHICommandList.h>

namespace wtr
{
	MeshBatch::MeshBatch()
		: m_transformIds()
		, m_transforms()
		, m_transformBuffer()
		, m_refDrawCommand()
		, m_refMesh()
		, m_refMaterial()
		, m_sectionIndex(0)
	{}

	bool MeshBatch::Init()
	{
		if (!m_refMesh || m_refMesh->rawBuffers.Empty() || m_refMesh->sections.Size() <= m_sectionIndex)
		{
			return false;
		}

		auto& meshSection = m_refMesh->sections[m_sectionIndex];

		m_refDrawCommand = Memory::MakeRef<MeshDrawCommand>();
		if (!m_refDrawCommand)
		{
			return false;
		}

		m_refDrawCommand->indexOffset = meshSection.indexOffset;
		m_refDrawCommand->indexCount = meshSection.indexCount;
		m_refDrawCommand->minVertexIndex = meshSection.minVertexIndex;
		m_refDrawCommand->maxVertexIndex = meshSection.maxVertexIndex;

		return true;
	}

	void MeshBatch::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!m_refDrawCommand || !m_refMesh || cmdList)
		{
			return;
		}
	}

	void MeshBatch::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!m_refDrawCommand || !m_transformBuffer || !m_refMesh)
		{
			return;
		}
	}

	void MeshBatch::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!m_refDrawCommand || !m_transformBuffer || !m_refMesh)
		{
			return;
		}
	}

	eResourceState MeshBatch::GetResourceState() const
	{
		if (!m_transformBuffer && !m_refMesh && !m_refMaterial)
		{
			return eResourceState::eNone;
		}

		eResourceState allState = eResourceState::eAll;
		if (m_transformBuffer)
		{
			allState = allState & m_transformBuffer->GetState();
		}

		if (m_refMesh)
		{
			allState = allState & m_refMesh->GetResourceState();
		}

		if (m_refMaterial)
		{
			allState = allState & m_refMaterial->GetResourceState();
		}

		return allState;
	}

	void MeshBatch::Clear()
	{
		m_transformIds.Clear();
		m_transforms.Clear();
		m_transformBuffer.Reset();

		m_refDrawCommand.Reset();
		m_refMesh.Reset();
		m_refMaterial.Reset();

		m_sectionIndex = 0;
	}

	void MeshBatch::SetMesh(Memory::RefPtr<MeshAsset> refMesh, const size_t meshSection)
	{
		if (!refMesh || refMesh->sections.Size() <= meshSection)
		{
			return;
		}

		m_refMesh = refMesh;
		m_sectionIndex = meshSection;
	}

	void MeshBatch::SetMaterial(Memory::RefPtr<MaterialAsset> refMaterial)
	{
		if (!refMaterial)
		{
			return;
		}

		m_refMaterial = refMaterial;
	}

	void MeshBatch::AddTransform(const ECS::UUID& id, const fmat4& transform)
	{
		auto itr = m_transformIds.Find(id);
		if (itr != m_transformIds.end())
		{
			UpdateTransform(id, transform);
		}
		else
		{
			m_transformIds[id] = transform;

			UpdateTransformData();
		}
	}

	void MeshBatch::UpdateTransform(const ECS::UUID& id, const fmat4& transform)
	{
		auto itr = m_transformIds.Find(id);
		if (itr == m_transformIds.end())
		{
			return;
		}

		if (itr->second == transform)
		{
			return;
		}

		itr->second = transform;

		UpdateTransformData();
	}

	void MeshBatch::RemoveTransform(const ECS::UUID& id)
	{
		auto itr = m_transformIds.Find(id);
		if (itr != m_transformIds.End())
		{
			m_transformIds.Erase(itr);

			UpdateTransformData();
		}
	}

	const wtr::DynamicArray<fmat4>& MeshBatch::GetTransforms() const
	{
		return m_transforms;
	}

	Memory::RefPtr<RHIBuffer> MeshBatch::GetTransformBuffer()
	{
		return m_transformBuffer;
	}

	Memory::RefPtr<MeshAsset> MeshBatch::GetMesh()
	{
		return m_refMesh;
	}

	Memory::RefPtr<MaterialAsset> MeshBatch::GetMaterial()
	{
		return m_refMaterial;
	}

	Memory::RefPtr<const MeshDrawCommand> MeshBatch::GetDrawCommand() const
	{
		return m_refDrawCommand;
	}

	const MeshBatchKey MeshBatch::GetKey() const
	{
		MeshBatchKey key;
		key.meshId = m_refMesh ? m_refMesh->id : ECS::UUID::Null();
		key.materialId = m_refMaterial ? m_refMaterial->id : ECS::UUID::Null();
		key.meshSection = m_sectionIndex;

		return key;
	}

	void MeshBatch::UpdateTransformData()
	{
		if (!m_refDrawCommand || !m_transformBuffer)
		{
			return;
		}

		m_transforms.Clear();
		m_transforms.Reserve(m_transformIds.Size());

		for (const auto& [id, transform] : m_transformIds)
		{
			m_transforms.PushBack(transform);
		}

		m_refDrawCommand->instanceCount = static_cast<uint32_t>(m_transforms.Size());
		m_transformBuffer->SetState(eResourceState::eLoaded);
	}
}