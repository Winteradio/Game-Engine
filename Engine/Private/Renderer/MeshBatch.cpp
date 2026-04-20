#include <Renderer/MeshBatch.h>

#include <Asset/AssetTypes.h>
#include <Memory/include/Core.h>
#include <RHI/RHIResources.h>
#include <RHI/RHICommandList.h>

namespace wtr
{
	MeshBatch::MeshBatch()
		: m_transformInfos()
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

			const uint32_t count = static_cast<uint32_t>(m_transformInfos.Size());

			// TODO : Consider the type inference using the template parameter of the container of the transform data
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

			fmat4* transformData = reinterpret_cast<fmat4*>(cmdList->Alloc<fmat4>(bufferDesc.size));
			if (!transformData)
			{
				return;
			}

			size_t index = 0;
			auto itr = m_transformInfos.begin();
			while (itr != m_transformInfos.end())
			{
				transformData[index] = itr->second.transform;
				itr->second.dirty = false;

				++index;
				++itr;
			}

			bufferDesc.data = static_cast<const void*>(transformData);

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

			m_refDrawCommand->instanceCount = static_cast<uint32_t>(m_transformInfos.Size());
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
			m_transformInfos.Clear();
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

		if (!m_transformBuffer || !m_refDrawCommand)
		{
			return;
		}

		RHIBufferUpdateDesc updateDesc;
		updateDesc.bufferType = m_transformBuffer->GetBufferType();
		updateDesc.accessType = m_transformBuffer->GetAccessType();
		updateDesc.componentType = m_transformBuffer->GetComponentType();
		updateDesc.numComponents = m_transformBuffer->GetNumComponents();
		updateDesc.stride = m_transformBuffer->GetStride();

		updateDesc.count = static_cast<uint32_t>(m_transformInfos.Size());
		updateDesc.size = static_cast<uint32_t>(m_transformInfos.Size()) * m_transformBuffer->GetStride();

		fmat4* transformData = reinterpret_cast<fmat4*>(cmdList->Alloc<fmat4>(updateDesc.size));
		if (!transformData)
		{
			return;
		}

		size_t minDirtyIndex = std::numeric_limits<size_t>::max();
		size_t maxDirtyIndex = 0;

		size_t index = 0;
		auto itr = m_transformInfos.begin();
		while (itr != m_transformInfos.end())
		{
			minDirtyIndex = std::min(minDirtyIndex, index);
			maxDirtyIndex = std::max(maxDirtyIndex, index);

			transformData[index] = itr->second.transform;
			itr->second.dirty = false;

			++index;
			++itr;
		}

		// TODO : Partial Update
		updateDesc.data = static_cast<const void*>(transformData + minDirtyIndex);
		updateDesc.dataOffset = minDirtyIndex * m_transformBuffer->GetStride();
		updateDesc.dataSize = (maxDirtyIndex - minDirtyIndex + 1) * m_transformBuffer->GetStride();

		const bool needResize = m_transformBuffer->GetCount() < m_transformInfos.Size();
		if (needResize)
		{
			cmdList->ResizeBuffer(updateDesc, m_transformBuffer);

			m_refDrawCommand->instanceCount = m_transformInfos.Size();
		}
		else
		{
			cmdList->UpdateBuffer(updateDesc, m_transformBuffer);
		}
	}

	eResourceState MeshBatch::GetResourceState() const
	{
		if (!m_transformBuffer || !m_vertexLayout)
		{
			return eResourceState::eNone;
		}

		if (!m_refMesh && !m_refMaterial)
		{
			return eResourceState::eNone;
		}

		eResourceState allState = eResourceState::eAll;
		if (m_transformBuffer)
		{
			allState = allState & m_transformBuffer->GetState();
		}

		if (m_vertexLayout)
		{
			allState = allState & m_vertexLayout->GetState();
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
		m_transformInfos.Clear();
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
		auto itr = m_transformInfos.Find(id);
		if (itr == m_transformInfos.end())
		{
			m_transformInfos[id] = { transform, true };
		}
	}

	void MeshBatch::UpdateTransform(const ECS::UUID& id, const fmat4& transform)
	{
		auto itr = m_transformInfos.Find(id);
		if (itr == m_transformInfos.end())
		{
			return;
		}

		if (itr->second.transform == transform)
		{
			return;
		}

		itr->second.transform = transform;
		itr->second.dirty = true;
	}

	void MeshBatch::RemoveTransform(const ECS::UUID& id)
	{
		auto itr = m_transformInfos.Find(id);
		if (itr == m_transformInfos.end())
		{
			return;
		}

		m_transformInfos.Erase(itr);
	}

	const size_t MeshBatch::GetInstanceCount() const
	{
		return m_transformInfos.Size();
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

	bool MeshBatchKey::operator==(const MeshBatchKey& other) const
	{
		return meshId == other.meshId && materialId == other.materialId && meshSection == other.meshSection;
	}

	const std::string MeshBatchKey::ToString() const
	{
		std::string data;
		data += "Mesh ID : " + meshId.ToString() + " / Material ID : " + materialId.ToString() + " / Section : " + std::to_string(meshSection) + " ";
		return data;
	}

	size_t MeshBatchHasher::operator()(const MeshBatchKey& key) const
	{
		size_t seed = 0;
		auto combine = [&seed](const auto& value)
		{
			std::hash<std::decay_t<decltype(value)>> hasher;
			seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		};
	
		combine(key.meshId);
		combine(key.materialId);
		combine(key.meshSection);
		return seed;
	}

	size_t MeshBatchHasher::operator()(const Memory::RefPtr<MeshBatch>& refMeshBatch) const
	{
		if (!refMeshBatch)
		{
			return 0;
		}

		return operator()(refMeshBatch->GetKey());
	}
}