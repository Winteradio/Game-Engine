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
		if (!m_refDrawCommand || !cmdList)
		{
			return;
		}
		
		if (m_refMesh)
		{
			for (const auto& [vertexKey, rawBuffer] : m_refMesh->rawBuffers)
			{
				if (!rawBuffer || rawBuffer->data.Empty())
				{
					continue;
				}

				RHIBufferCreateDesc bufferDesc;
				bufferDesc.rawBuffer = rawBuffer;
				bufferDesc.bufferType = eBufferType::eVertex;
				bufferDesc.accessType = eDataAccess::eStatic;
				bufferDesc.size = static_cast<uint32_t>(rawBuffer->data.Size());
				bufferDesc.stride = rawBuffer->numComponents * GetDataTypeSize(rawBuffer->componentType);

				Memory::RefPtr<RHIBuffer> vertexBuffer = cmdList->CreateBuffer(bufferDesc);
				if (vertexBuffer)
				{
					m_refMesh->buffers[vertexKey] = vertexBuffer;
				}
			}

			if (m_refMesh->rawIndex && !m_refMesh->rawIndex->data.Empty())
			{
				RHIBufferCreateDesc bufferDesc;
				bufferDesc.rawBuffer = m_refMesh->rawIndex;
				bufferDesc.bufferType = eBufferType::eIndex;
				bufferDesc.accessType = eDataAccess::eStatic;
				bufferDesc.size = static_cast<uint32_t>(m_refMesh->rawIndex->data.Size());
				bufferDesc.stride = GetDataTypeSize(m_refMesh->rawIndex->componentType);

				Memory::RefPtr<RHIBuffer> indexBuffer = cmdList->CreateBuffer(bufferDesc);
				if (indexBuffer)
				{
					m_refMesh->index = indexBuffer;
				}
			}

			m_refDrawCommand->indexBuffer = m_refMesh->index;
			m_refDrawCommand->vertexBuffers = m_refMesh->buffers;
		}

		if (m_refMaterial)
		{
			for (const auto& [textureSlot, refTexture] : m_refMaterial->textures)
			{
				if (!refTexture)
				{
					continue;
				}

				RHITextureCreateDesc textureDesc;
				textureDesc.rawBuffer = refTexture->rawBuffer;
				textureDesc.width = refTexture->width;
				textureDesc.height = refTexture->height;
				textureDesc.depth = refTexture->depth;
				textureDesc.mipLevels = refTexture->mipLevels;
				textureDesc.sampleCount = refTexture->sampleCount;
				textureDesc.format = refTexture->pixelFormat;

				Memory::RefPtr<RHITexture> texture = cmdList->CreateTexture(textureDesc);
				if (texture)
				{
					m_refMaterial->textures[textureSlot]->texture = texture;
					m_refDrawCommand->textureSlots[textureSlot] = texture;
				}
			}
		}

		if (!m_transformBuffer && !m_transforms.Empty())
		{
			Memory::RefPtr<FormattedBuffer> rawBuffer = Memory::MakeRef<FormattedBuffer>();
			rawBuffer->data.Resize(m_transforms.Size() * sizeof(fmat4));
			std::copy(m_transforms.Data(), m_transforms.Data() + m_transforms.Size(), reinterpret_cast<fmat4*>(rawBuffer->data.Data()));

			RHIBufferCreateDesc bufferDesc;
			bufferDesc.rawBuffer = rawBuffer;
			bufferDesc.bufferType = eBufferType::eVertex;
			bufferDesc.accessType = eDataAccess::eDynamic;
			bufferDesc.size = static_cast<uint32_t>(rawBuffer->data.Size());
			bufferDesc.stride = rawBuffer->numComponents * GetDataTypeSize(rawBuffer->componentType);

			m_transformBuffer = cmdList->CreateBuffer(bufferDesc);
		}
	}

	void MeshBatch::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
	}

	void MeshBatch::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (m_refMesh)
		{

		}

		if (m_refMaterial)
		{

		}

		if (m_transformBuffer)
		{

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