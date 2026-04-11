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

	void MeshBatch::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (!m_transformBuffer)
		{
			constexpr uint32_t numComponents = 16;

			const uint32_t count = static_cast<uint32_t>(m_transforms.Size());
			const eDataType componentType = eDataType::eFloat;
			const eBufferType bufferType = eBufferType::eStorage;
			const eDataAccess accessType = eDataAccess::eDynamic;

			RHIBufferCreateDesc bufferDesc;
			bufferDesc.bufferType = bufferType;
			bufferDesc.accessType = accessType;
			bufferDesc.componentType = componentType;
			bufferDesc.numComponents = numComponents;
			bufferDesc.count = count;
			bufferDesc.size = count * numComponents * GetDataTypeSize(componentType);
			bufferDesc.stride = numComponents * GetDataTypeSize(componentType);
			bufferDesc.data = static_cast<const void*>(m_transforms.Data());

			m_transformBuffer = cmdList->CreateBuffer(bufferDesc);
		}

		if (!m_vertexLayout && m_refMesh)
		{
			const MeshSection& meshSection = m_refMesh->sections[m_sectionIndex];

			RHIVertexLayoutCreateDesc layoutDesc;
			layoutDesc.indexBuffer = m_refMesh->index;

			for (const auto& [vertexKey, buffer] : m_refMesh->buffers)
			{
				if (!buffer)
				{
					continue;
				}

				const eDataType componentType = buffer->GetComponentType();
				const uint32_t componentSize = GetDataTypeSize(componentType);
				const uint32_t numComponents = buffer->GetNumComponents();

				const uint32_t vertexLocation = GetVertexLocation(vertexKey);

				const bool integer = IsIntegerDataType(componentType);

				RHIVertexAttribute attribute;
				attribute.location = vertexLocation;
				attribute.offset = meshSection.minVertexIndex * vertexLocation;
				attribute.normalized = false; // Assuming non-normalized data
				attribute.integer = integer;
				attribute.divisor = 0; // Assuming per-vertex data
				attribute.numComponents = numComponents;
				attribute.componentType = componentType;
				attribute.semantic = vertexKey.semantic;
				attribute.semanticIndex = vertexKey.semanticIndex;

				layoutDesc.vertexStreams[vertexKey] = { buffer, attribute };
			}

			layoutDesc.indexBuffer = m_refMesh->index;

			m_vertexLayout = cmdList->CreateVertexLayout(layoutDesc);
		}

		if (m_transformBuffer && m_vertexLayout)
		{
			m_refDrawCommand = Memory::MakeRef<MeshDrawCommand>();
			if (!m_refDrawCommand)
			{
				return;
			}

			m_refDrawCommand->vertexLayout = m_vertexLayout;
			m_refDrawCommand->indexOffset = m_refMesh->sections[m_sectionIndex].indexOffset;
			m_refDrawCommand->indexCount = m_refMesh->sections[m_sectionIndex].indexCount;
			m_refDrawCommand->minVertexIndex = m_refMesh->sections[m_sectionIndex].minVertexIndex;
			m_refDrawCommand->maxVertexIndex = m_refMesh->sections[m_sectionIndex].maxVertexIndex;

			if (m_refMaterial)
			{
				for (const auto& [slot, textureAsset] : m_refMaterial->textures)
				{
					if (!textureAsset)
					{
						continue;
					}

					m_refDrawCommand->textureSlots[slot] = textureAsset->texture;
				}

				m_refDrawCommand->vectorValues = m_refMaterial->vectorValues;
				m_refDrawCommand->scalarValues = m_refMaterial->scalarValues;
			}

			m_refDrawCommand->instanceBuffer = m_transformBuffer;
		}
	}

	void MeshBatch::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_transformBuffer)
		{
			cmdList->RemoveBuffer(m_transformBuffer);

			m_transformBuffer.Reset();
			m_transformIds.Clear();
			m_transforms.Clear();
		}

		if (m_vertexLayout)
		{
			cmdList->RemoveVertexLayout(m_vertexLayout);

			m_vertexLayout.Reset();
		}
	}

	void MeshBatch::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_transformBuffer)
		{
			RHIBufferUpdateDesc updateDesc;
			updateDesc.bufferType = m_transformBuffer->GetBufferType();
			updateDesc.accessType = m_transformBuffer->GetAccessType();
			updateDesc.componentType = m_transformBuffer->GetComponentType();
			updateDesc.numComponents = m_transformBuffer->GetNumComponents();
			updateDesc.count = static_cast<uint32_t>(m_transforms.Size());
			updateDesc.size = updateDesc.count * updateDesc.numComponents * GetDataTypeSize(updateDesc.componentType);
			updateDesc.stride = updateDesc.numComponents * GetDataTypeSize(updateDesc.componentType);

			// TODO : Partial Update
			updateDesc.data = m_transforms.Data();
			updateDesc.dataOffset = 0;
			updateDesc.dataSize = updateDesc.size;

			const bool needResize = m_transformBuffer->GetCount() < m_transforms.Size();
			if (needResize)
			{
				cmdList->ResizeBuffer(updateDesc, m_transformBuffer);
			}
			else
			{
				cmdList->UpdateBuffer(updateDesc, m_transformBuffer);
			}
			
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

	void MeshBatch::SetMesh(Memory::RefPtr<const MeshAsset> refMesh, const size_t meshSection)
	{
		if (!refMesh || refMesh->sections.Size() <= meshSection)
		{
			return;
		}

		m_refMesh = refMesh;
		m_sectionIndex = meshSection;
	}

	void MeshBatch::SetMaterial(Memory::RefPtr<const MaterialAsset> refMaterial)
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

	const size_t MeshBatch::GetInstanceCount() const
	{
		return m_transformIds.Size();
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

	const std::string MeshBatch::ToString() const
	{
		std::string data;

		if (m_refMesh)
		{
			data += "Mesh : " + m_refMesh->name + " / Section : " + std::to_string(m_sectionIndex) + " ";
		}

		if (m_refMaterial)
		{
			data += "Material : " + m_refMaterial->name + " ";
		}

		return data;
	}

	void MeshBatch::UpdateTransformData()
	{
		m_transforms.Clear();
		m_transforms.Reserve(m_transformIds.Size());

		for (const auto& [id, transform] : m_transformIds)
		{
			m_transforms.PushBack(transform);
		}
	}
}