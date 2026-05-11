#include <Renderer/MeshBatch.h>

#include <Asset/AssetTypes.h>
#include <RHI/RHIResources.h>
#include <RHI/RHICommandList.h>

#include <Renderer/RenderTypes.h>
#include <Renderer/MeshDrawCommand.h>
#include <Renderer/Proxy/MaterialProxy.h>

#include <Log/include/Log.h>
#include <Memory/include/Core.h>

namespace wtr
{
	MeshBatch::MeshBatch()
		: m_proxyIndices()
		, m_proxyInfos()
		, m_drawCommand()
		, m_refMesh()
		, m_refMaterial()
		, m_sectionIndex(0)
		, m_dirty(eRenderDirty::eNone)
	{
	}

	void MeshBatch::Upload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_refMesh || !m_refMaterial)
		{
			return;
		}

		m_drawCommand = !m_drawCommand ? Memory::MakeRef<MeshDrawCommand>() : m_drawCommand;
		if (!m_drawCommand)
		{
			LOGINFO() << "[MeshBatch] Failed to create the mesh command";
			return;
		}

		wtr::DynamicArray<RHIBufferCreateDesc::DataRange> dataRanges;
		dataRanges.Reserve(m_proxyInfos.Size());

		size_t totalCount = 0;
		for (auto& proxyInfo : m_proxyInfos)
		{
			if (!proxyInfo.transform)
			{
				proxyInfo.dirty = false;
				continue;
			}

			const uint32_t offset = dataRanges.Empty() ? 0 : dataRanges.Back().offset + static_cast<uint32_t>(dataRanges.Back().data->GetSize());
			dataRanges.PushBack({ offset, proxyInfo.transform});

			totalCount += proxyInfo.transform->GetCount();

			proxyInfo.dirty = false;
		}

		auto& transform = m_drawCommand->transform;
		if (!transform)
		{
			constexpr uint32_t numComponents = 16; // Assuming the transform is a 4x4 matrix
			const eDataType componentType = eDataType::eFloat;
			const eBufferType bufferType = eBufferType::eStorage;
			const eDataAccess accessType = eDataAccess::eDynamic;

			RHIBufferCreateDesc bufferDesc;
			bufferDesc.bufferType = bufferType;
			bufferDesc.accessType = accessType;
			bufferDesc.componentType = componentType;
			bufferDesc.numComponents = numComponents;
			bufferDesc.count = totalCount;
			bufferDesc.size = totalCount * numComponents * GetDataTypeSize(componentType);
			bufferDesc.stride = numComponents * GetDataTypeSize(componentType);
			bufferDesc.dataRanges = std::move(dataRanges);

			transform  = cmdList->CreateBuffer(bufferDesc);
			if (!transform)
			{
				LOGINFO() << "[MeshBatch] Failed to create the transform buffer";
				
				Unload(cmdList);
				return;
			}
		}

		auto& visible = m_drawCommand->visible;
		if (!visible)
		{
			constexpr uint32_t numComponents = 1;
			const eDataType componentType = eDataType::eByte;
			const eBufferType bufferType = eBufferType::eStorage;
			const eDataAccess accessType = eDataAccess::eDynamic;

			Memory::RefPtr<ArrayData<uint8_t>> visibilityData = Memory::MakeRef<ArrayData<uint8_t>>();
			visibilityData->data.Assign(totalCount, 1);

			RHIBufferCreateDesc bufferDesc;
			bufferDesc.bufferType = bufferType;
			bufferDesc.accessType = accessType;
			bufferDesc.componentType = componentType;
			bufferDesc.numComponents = numComponents;
			bufferDesc.count = totalCount;
			bufferDesc.size = totalCount * numComponents * GetDataTypeSize(componentType);
			bufferDesc.stride = numComponents * GetDataTypeSize(componentType);
			bufferDesc.dataRanges.PushBack({ 0, visibilityData });

			visible = cmdList->CreateBuffer(bufferDesc);
			if (!visible)
			{
				LOGINFO() << "[MeshBatch] Failed to create the visibility buffer";

				Unload(cmdList);
				return;
			}
		}

		auto& indirect = m_drawCommand->indirect;
		if (!indirect)
		{
			constexpr uint32_t numComponents = 1;
			const eDataType componentType = eDataType::eNone;
			const eBufferType bufferType = eBufferType::eIndirect;
			const eDataAccess accessType = eDataAccess::eDynamic;

			Memory::RefPtr<ScalarData<IndirectDrawCommand>> indirectCommand = Memory::MakeRef<ScalarData<IndirectDrawCommand>>();

			RHIBufferCreateDesc bufferDesc;
			bufferDesc.bufferType = bufferType;
			bufferDesc.accessType = accessType;
			bufferDesc.componentType = componentType;
			bufferDesc.numComponents = numComponents;
			bufferDesc.count = 1;
			bufferDesc.size = sizeof(IndirectDrawCommand);
			bufferDesc.stride = sizeof(IndirectDrawCommand);
			bufferDesc.dataRanges.PushBack({ 0, indirectCommand });

			indirect = cmdList->CreateBuffer(bufferDesc);
			if (!indirect)
			{
				LOGINFO() << "[MeshBatch] Failed to create the indirect draw command";

				Unload(cmdList);
				return;
			}
		}

		auto& vertexLayout = m_drawCommand->vertexLayout;
		if (!vertexLayout)
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
			
			vertexLayout = cmdList->CreateVertexLayout(layoutDesc);
			if (!vertexLayout)
			{
				LOGINFO() << "[MeshBatch] Failed to create the vertex layout";

				Unload(cmdList);
				return;
			}
		}

		if (m_sectionIndex >= m_refMesh->sections.Size())
		{
			LOGINFO() << "[MeshBatch] Invalid mesh section index";

			Unload(cmdList);
			return;
		}

		m_drawCommand->minVertexIndex = m_refMesh->sections[m_sectionIndex].minVertexIndex;
		m_drawCommand->maxVertexIndex = m_refMesh->sections[m_sectionIndex].maxVertexIndex;
		m_drawCommand->instanceCount = static_cast<uint32_t>(totalCount);
		m_drawCommand->indexOffset = m_refMesh->sections[m_sectionIndex].indexOffset;
		m_drawCommand->indexCount = m_refMesh->sections[m_sectionIndex].indexCount;
		m_drawCommand->indexType = m_refMesh->index ? m_refMesh->index->GetComponentType() : eDataType::eNone;

		m_drawCommand->indirectDraw = true;
		m_drawCommand->drawMode = m_refMesh->drawMode;

		m_drawCommand->material = m_refMaterial;

		ClearDirty();
	}

	void MeshBatch::Unload(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_drawCommand)
		{
			return;
		}

		if (m_drawCommand->transform)
		{
			cmdList->RemoveBuffer(m_drawCommand->transform);
			m_drawCommand->transform.Reset();
		}

		if (m_drawCommand->indirect)
		{
			cmdList->RemoveBuffer(m_drawCommand->indirect);
			m_drawCommand->indirect.Reset();
		}

		if (m_drawCommand->vertexLayout)
		{
			cmdList->RemoveVertexLayout(m_drawCommand->vertexLayout);
			m_drawCommand->vertexLayout.Reset();
		}

		ClearDirty();
	}

	void MeshBatch::Sync(Memory::RefPtr<RHICommandList> cmdList)
	{
		const bool transformDirty = (m_dirty & eRenderDirty::eTransform) == eRenderDirty::eTransform;
		const bool meshDirty = (m_dirty & eRenderDirty::eMesh) == eRenderDirty::eMesh;
		const bool materialDirty = (m_dirty & eRenderDirty::eMaterial) == eRenderDirty::eMaterial;

		if (transformDirty)
		{
			SyncTransforms(cmdList);
		}

		if (meshDirty)
		{
			SyncMesh(cmdList);
		}

		if (materialDirty)
		{
			SyncMaterial();
		}

		ClearDirty();
	}

	void MeshBatch::SyncTransforms(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || m_proxyInfos.Empty())
		{
			return;
		}

		if (!m_drawCommand || !m_drawCommand->transform || !m_drawCommand->visible)
		{
			return;
		}

		wtr::DynamicArray<RHIBufferCreateDesc::DataRange> dataRanges;
		dataRanges.Reserve(m_proxyInfos.Size());
		
		size_t totalCount = 0;
		for (auto& proxyInfo : m_proxyInfos)
		{
			if (!proxyInfo.transform)
			{
				continue;
			}

			totalCount += proxyInfo.transform->GetCount();
		}

		size_t offset = 0;
		const bool resize = (totalCount != m_drawCommand->transform->GetCount());
		for (auto& proxyInfo : m_proxyInfos)
		{
			if (!proxyInfo.transform)
			{
				proxyInfo.dirty = false;
				continue;
			}

			if (proxyInfo.dirty || resize)
			{
				dataRanges.PushBack({ static_cast<uint32_t>(offset), proxyInfo.transform });
			}

			offset += proxyInfo.transform->GetSize();

			proxyInfo.dirty = false;
		}

		if (resize)
		{
			// Transform Buffer Resizes
			{
				RHIBufferDesc prevDesc = m_drawCommand->transform->GetDesc();

				RHIBufferCreateDesc bufferDesc;
				bufferDesc.bufferType = prevDesc.bufferType;
				bufferDesc.accessType = prevDesc.accessType;
				bufferDesc.componentType = prevDesc.componentType;
				bufferDesc.numComponents = prevDesc.numComponents;
				bufferDesc.count = totalCount;
				bufferDesc.size = totalCount * prevDesc.numComponents * GetDataTypeSize(prevDesc.componentType);
				bufferDesc.stride = prevDesc.stride;
				bufferDesc.dataRanges = std::move(dataRanges);

				cmdList->ResizeBuffer(bufferDesc, m_drawCommand->transform);
			}

			// Visible Buffer Resizes
			{
				Memory::RefPtr<ArrayData<uint8_t>> visibilityData = Memory::MakeRef<ArrayData<uint8_t>>();
				visibilityData->data.Assign(totalCount, 1);

				RHIBufferDesc prevDesc = m_drawCommand->visible->GetDesc();

				RHIBufferCreateDesc bufferDesc;
				bufferDesc.bufferType = prevDesc.bufferType;
				bufferDesc.accessType = prevDesc.accessType;
				bufferDesc.componentType = prevDesc.componentType;
				bufferDesc.numComponents = prevDesc.numComponents;
				bufferDesc.count = totalCount;
				bufferDesc.size = totalCount * prevDesc.numComponents * GetDataTypeSize(prevDesc.componentType);
				bufferDesc.stride = prevDesc.stride;
				bufferDesc.dataRanges.PushBack({ 0, visibilityData });

				cmdList->ResizeBuffer(bufferDesc, m_drawCommand->visible);
			}
		}
		else
		{
			// Transform Buffer Updates
			RHIBufferDesc prevDesc = m_drawCommand->transform->GetDesc();

			RHIBufferUpdateDesc bufferDesc;
			bufferDesc.bufferType = prevDesc.bufferType;
			bufferDesc.accessType = prevDesc.accessType;
			bufferDesc.componentType = prevDesc.componentType;
			bufferDesc.numComponents = prevDesc.numComponents;
			bufferDesc.count = totalCount;
			bufferDesc.size = totalCount * prevDesc.numComponents * GetDataTypeSize(prevDesc.componentType);
			bufferDesc.stride = prevDesc.stride;
			bufferDesc.dataRanges = std::move(dataRanges);
			bufferDesc.mapAccess = eMapAccess::eInvalidateBuffer;

			cmdList->UpdateBuffer(bufferDesc, m_drawCommand->transform);
		}
	}

	void MeshBatch::SyncMesh(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || !m_refMesh)
		{
			return;
		}

		if (!m_drawCommand || !m_drawCommand->vertexLayout)
		{
			return;
		}

		RHIVertexLayoutDesc prevDesc = m_drawCommand->vertexLayout->GetDesc();

		bool layoutChanged = false;

		for (const auto& [vertexKey, buffer] : m_refMesh->buffers)
		{
			const auto itr = prevDesc.vertexStreams.Find(vertexKey);
			if (itr == prevDesc.vertexStreams.End())
			{
				layoutChanged = true;

				break;
			}

			const auto& bufferStream = itr->second;
			if (!buffer || !bufferStream.buffer)
			{
				continue;
			}

			const eDataType componentType = buffer->GetComponentType();
			const uint32_t numComponents = buffer->GetNumComponents();
			const uint32_t vertexLocation = GetVertexLocation(vertexKey);
			const bool integer = IsIntegerDataType(componentType);

			if (componentType != bufferStream.attribute.componentType ||
				numComponents != bufferStream.attribute.numComponents ||
				vertexLocation != bufferStream.attribute.location ||
				integer != bufferStream.attribute.integer ||
				&*buffer != &*bufferStream.buffer)
			{
				layoutChanged = true;
				break;
			}
		}

		if (layoutChanged)
		{
			RHIVertexLayoutCreateDesc layoutDesc;

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
				attribute.offset = m_refMesh->sections[m_sectionIndex].minVertexIndex * vertexLocation;
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

			cmdList->UpdateVertexLayout(layoutDesc, m_drawCommand->vertexLayout);
		}
	}

	void MeshBatch::SyncMaterial()
	{
		if (!m_drawCommand || !m_refMaterial)
		{
			return;
		}

		m_drawCommand->material = m_refMaterial;
	}

	void MeshBatch::SetDirty(const eRenderDirty dirty)
	{
		m_dirty = m_dirty | dirty;
	}

	void MeshBatch::ClearDirty()
	{
		m_dirty = eRenderDirty::eNone;
	}

	eResourceState MeshBatch::GetResourceState() const
	{
		eResourceState allState = eResourceState::eAll;
		if (!m_drawCommand || !m_drawCommand->transform || !m_drawCommand->vertexLayout)
		{
			return eResourceState::eNone;
		}
		else
		{
			if (m_drawCommand->transform)
			{
				allState = allState & m_drawCommand->transform->GetState();
			}

			if (m_drawCommand->vertexLayout)
			{
				allState = allState & m_drawCommand->vertexLayout->GetState();
			}
		}

		if (m_drawCommand->indirectDraw)
		{
			if (m_drawCommand->indirect)
			{
				allState = allState & m_drawCommand->indirect->GetState();
			}
		}

		if (!m_refMesh && !m_refMaterial)
		{
			return eResourceState::eNone;
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
		m_proxyIndices.Clear();
		m_proxyInfos.Clear();

		m_drawCommand.Reset();
		m_refMesh.Reset();
		m_refMaterial.Reset();

		m_sectionIndex = 0;
		m_dirty = eRenderDirty::eNone;
	}

	void MeshBatch::SetMesh(Memory::RefPtr<const MeshAsset> refMesh, const size_t meshSection)
	{
		if (!refMesh || refMesh->sections.Size() <= meshSection || m_refMesh)
		{
			return;
		}

		m_refMesh = refMesh;
		m_sectionIndex = meshSection;

		SetDirty(eRenderDirty::eMesh);
	}

	void MeshBatch::UpdateMesh()
	{
		SetDirty(eRenderDirty::eMesh);
	}

	void MeshBatch::SetMaterial(Memory::RefPtr<const MaterialProxy> refMaterial)
	{
		if (!refMaterial || m_refMaterial)
		{
			return;
		}

		m_refMaterial = refMaterial;
		SetDirty(eRenderDirty::eMaterial);
	}

	void MeshBatch::AddTransform(const ECS::UUID& id, Memory::RefPtr<const RawData> transform)
	{
		if (!transform)
		{
			LOGERROR() << "[MeshBatch] Failed to add transform. Invalid transform data.";
			return;
		}

		auto itr = m_proxyIndices.Find(id);
		if (itr != m_proxyIndices.End())
		{
			return;
		}

		ProxyInfo info;
		info.dirty = true;
		info.id = id;
		info.transform = transform;

		m_proxyIndices[id] = m_proxyInfos.Size();
		m_proxyInfos.EmplaceBack(std::move(info));

		SetDirty(eRenderDirty::eTransform);
	}

	void MeshBatch::UpdateTransform(const ECS::UUID& id)
	{
		auto itr = m_proxyIndices.Find(id);
		if (itr == m_proxyIndices.End())
		{
			return;
		}

		const size_t index = itr->second;
		auto& proxyInfo = m_proxyInfos[index];
		proxyInfo.dirty = true;

		SetDirty(eRenderDirty::eTransform);
	}

	bool MeshBatch::HasTransform(const ECS::UUID& id) const
	{
		return m_proxyIndices.Find(id) != m_proxyIndices.End();
	}

	void MeshBatch::RemoveTransform(const ECS::UUID& id)
	{
		auto itr = m_proxyIndices.Find(id);
		if (itr == m_proxyIndices.End())
		{
			return;
		}

		const size_t index = itr->second;
		auto& proxyInfo = m_proxyInfos[index];
		auto& lastProxyInfo = m_proxyInfos.Back();
		lastProxyInfo.dirty = true;

		m_proxyIndices[lastProxyInfo.id] = index;
		m_proxyIndices.Erase(proxyInfo.id);

		std::swap(proxyInfo, lastProxyInfo);
		m_proxyInfos.PopBack();

		SetDirty(eRenderDirty::eTransform);
	}

	Memory::RefPtr<const MeshDrawCommand> MeshBatch::GetDrawCommand() const
	{
		return m_drawCommand;
	}

	const MeshBatchKey MeshBatch::GetKey() const
	{
		MeshBatchKey key;
		key.meshId = m_refMesh ? m_refMesh->id : ECS::UUID::Null();
		key.materialId = m_refMaterial ? m_refMaterial->GetID() : ECS::UUID::Null();
		key.meshSection = m_sectionIndex;

		return key;
	}

	bool MeshBatch::IsEmpty() const
	{
		return m_proxyInfos.Empty() || m_proxyIndices.Empty();
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