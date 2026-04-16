#include <Renderer/RenderScene.h>

#include <Renderer/Proxy/PrimitiveProxy.h>
#include <Renderer/Proxy/LightProxy.h>
#include <Renderer/MeshBatch.h>
#include <RHI/RHIResources.h>
#include <RHI/RHICommandList.h>
#include <Asset/AssetTypes.h>

#include <Memory/include/Core.h>

#include <Log/include/Log.h>

namespace wtr
{
	RenderScene::RenderScene()
		: m_primitives()
		, m_lights()
		, m_meshBatches()
		, m_pendingPrimitives()
	{}

	RenderScene::~RenderScene()
	{
		Clear();
	}

	void RenderScene::RemoveAll(Memory::RefPtr<RHICommandList> cmdList)
	{
		m_primitives.Clear();
		m_lights.Clear();

		m_pendingPrimitives.Clear();

		for (const auto& [batchKey, meshBatch] : m_meshBatches)
		{
			if (meshBatch)
			{
				meshBatch->Unload(cmdList);
			}
		}

		m_meshBatches.Clear();
	}

	void RenderScene::Clear()
	{
		m_primitives.Clear();
		m_lights.Clear();
		m_meshBatches.Clear();
		m_pendingPrimitives.Clear();
	}

	void RenderScene::Flush(Memory::RefPtr<RHICommandList> cmdList)
	{
		FlushPending(cmdList);
		FlushUpdated(cmdList);
		FlushBatch(cmdList);
	}

	void RenderScene::FlushPending(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		auto itr = m_pendingPrimitives.begin();
		while (itr != m_pendingPrimitives.end())
		{
			auto& primitive = *itr;
			if (!primitive || !primitive->GetMesh())
			{
				LOGERROR() << "[RenderScene] Failed to add the primitive proxy, cause the primitive proxy is invalid, ID : " << primitive->GetID().ToString();
				itr = m_pendingPrimitives.Erase(itr);
				continue;
			}

			auto mesh = primitive->GetMesh();
			if (mesh->GetState() >= eAssetState::eLoaded)
			{
				AddBatch(primitive, cmdList);
				m_primitives[primitive->GetID()] = primitive;
				itr = m_pendingPrimitives.Erase(itr);
				continue;
			}
			else
			{
				itr++;
			}
		}
	}

	void RenderScene::FlushUpdated(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		auto itr = m_updatedPrimitives.begin();
		while (itr != m_updatedPrimitives.end())
		{
			auto& primitive = *itr;
			if (!primitive || !primitive->GetMesh())
			{
				LOGERROR() << "[RenderScene] Failed to update the primitive proxy, cause the primitive proxy is invalid, ID : " << primitive->GetID().ToString();
				itr = m_updatedPrimitives.Erase(itr);
				continue;
			}

			auto mesh = primitive->GetMesh();
			if (mesh->GetState() >= eAssetState::eLoaded)
			{
				UpdateBatch(primitive, cmdList);
				itr = m_updatedPrimitives.Erase(itr);
				continue;
			}
			else
			{
				itr++;
			}
		}
	}

	void RenderScene::FlushBatch(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		for (auto& meshBatch : m_addedBatches)
		{
			if (meshBatch)
			{
				meshBatch->Upload(cmdList);
			}
		}

		for (auto& meshBatch : m_updatedBatches)
		{
			if (meshBatch)
			{
				meshBatch->Sync(cmdList);
			}
		}

		m_addedBatches.Clear();
		m_updatedBatches.Clear();
	}

	void RenderScene::UpdateProxy(const UpdateProxyInfo& updateInfo, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		auto itrPrimitive = m_primitives.Find(updateInfo.id);
		if (itrPrimitive != m_primitives.End())
		{
			auto& primitiveProxy = itrPrimitive->second;
			primitiveProxy->UpdatePosition(updateInfo.position);
			primitiveProxy->UpdateRotation(updateInfo.rotation);
			primitiveProxy->UpdateScale(updateInfo.scale);

			m_updatedPrimitives.Insert(primitiveProxy);
		}

		auto itrLight = m_lights.Find(updateInfo.id);
		if (itrLight != m_lights.End())
		{
			auto& lightProxy = itrLight->second;
			lightProxy->UpdatePosition(updateInfo.position);
			lightProxy->UpdateRotation(updateInfo.rotation);
			lightProxy->UpdateScale(updateInfo.scale);
		}
	}

	void RenderScene::AddPrimitive(Memory::RefPtr<PrimitiveProxy> primitive)
	{
		if (!primitive)
		{
			return;
		}

		m_pendingPrimitives.Insert(primitive);

		LOGINFO() << "[RenderScene] Add the primitive proxy, ID : " << primitive->GetID().ToString();
	}

	void RenderScene::RemovePrimitive(const ECS::UUID& id, Memory::RefPtr<RHICommandList> cmdList)
	{
		auto itr = m_primitives.Find(id);
		if (itr != m_primitives.End())
		{
			auto& primitiveProxy = itr->second;

			RemoveBatch(primitiveProxy, cmdList);

			LOGINFO() << "[RenderScene] Remove the primitive proxy, ID : " << id.ToString();
		}
	}

	void RenderScene::AddLight(Memory::RefPtr<LightProxy> light)
	{
		if (!light)
		{
			return;
		}

		m_lights[light->GetID()] = light;

		LOGINFO() << "[RenderScene] Add the light proxy, ID : " << light->GetID().ToString();
	}

	void RenderScene::RemoveLight(const ECS::UUID& id)
	{
		auto itr = m_lights.Find(id);
		if (itr != m_lights.End())
		{
			auto& lightProxy = itr->second;
			m_lights.Erase(itr);

			LOGINFO() << "[RenderScene] Remove the light proxy, ID : " << id.ToString();
		}
	}

	Memory::RefPtr<MeshBatch> RenderScene::GetMeshBatch(const MeshBatchKey& key)
	{
		auto itr = m_meshBatches.Find(key);
		if (itr != m_meshBatches.End())
		{
			return itr->second;
		}
		else
		{
			return {};
		}
	}

	const RenderScene::MeshBatchContainer& RenderScene::GetMeshBatches() const
	{
		return m_meshBatches;
	}

	void RenderScene::AddBatch(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!primitive || !cmdList)
		{
			return;
		}
		
		auto meshAsset = primitive->GetMesh();
		if (!meshAsset)
		{
			LOGERROR() << "[RenderScene] Failed to add the batch, cause the mesh asset is invalid, ID : " << primitive->GetID().ToString();
			return;
		}

		auto overrideMaterial = primitive->GetOverrideMaterial();

		const size_t endIndex = meshAsset->sections.Size();
		for (size_t index = 0; index < endIndex; index++)
		{
			const MeshBatchKey batchKey = GetMeshBatchKey(primitive, index);
			Memory::RefPtr<MeshBatch> meshBatch = GetMeshBatch(batchKey);
			if (meshBatch)
			{
				LOGINFO() << "[RenderScene] Add the mesh batch's transform info, the mesh batch ID : " << batchKey.ToString() << ", the primitive ID : " << primitive->GetID().ToString();

				meshBatch->AddTransform(primitive->GetID(), primitive->GetTransform());

				m_updatedBatches.Insert(meshBatch);

				continue;
			}
			
			Memory::RefPtr<const MaterialAsset> materialAsset;
			if (!overrideMaterial)
			{
				auto itr = meshAsset->materials.Find(meshAsset->sections[index].materialName);
				if (itr != meshAsset->materials.End())
				{
					materialAsset = itr->second;
				}
			}

			meshBatch = Memory::MakeRef<MeshBatch>();
			if (!meshBatch)
			{
				LOGERROR() << "[RenderScene] Failed to add the batch, cause failed to create the mesh batch, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;
				continue;
			}

			meshBatch->SetMesh(meshAsset, index);
			meshBatch->SetMaterial(materialAsset);
			meshBatch->AddTransform(primitive->GetID(), primitive->GetTransform());

			LOGINFO() << "[RenderScene] Add the batch, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;

			m_addedBatches.Insert(meshBatch);
			m_meshBatches.Insert(std::make_pair(batchKey, meshBatch));
		}
	}
	
	void RenderScene::UpdateBatch(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!primitive || !cmdList)
		{
			return;
		}

		auto meshAsset = primitive->GetMesh();
		if (!meshAsset || meshAsset->sections.Empty())
		{
			LOGWARN() << "[RenderScene] Failed to add the batch, cause the mesh asset is invalid, ID : " << primitive->GetID().ToString();
			return;
		}

		const size_t endIndex = meshAsset->sections.Size();
		for (size_t index = 0; index < endIndex; index++)
		{
			const MeshBatchKey batchKey = GetMeshBatchKey(primitive, index);

			Memory::RefPtr<MeshBatch> meshBatch = GetMeshBatch(batchKey);
			if (!meshBatch)
			{
				continue;
			}

			meshBatch->UpdateTransform(primitive->GetID(), primitive->GetTransform());

			m_updatedBatches.Insert(meshBatch);
		}
	}

	void RenderScene::RemoveBatch(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!primitive || !cmdList)
		{
			return;
		}
		
		auto meshAsset = primitive->GetMesh();
		if (!meshAsset || meshAsset->sections.Empty())
		{
			LOGWARN() << "[RenderScene] Failed to remove the batch, cause the mesh asset is invalid, ID : " << primitive->GetID().ToString();
			return;
		}

		const size_t endIndex = meshAsset->sections.Size();
		for (size_t index = 0; index < endIndex; index++)
		{
			const MeshBatchKey batchKey = GetMeshBatchKey(primitive, index);

			Memory::RefPtr<MeshBatch> meshBatch = GetMeshBatch(batchKey);
			if (!meshBatch)
			{
				continue;
			}

			meshBatch->RemoveTransform(primitive->GetID());

			if (meshBatch->GetInstanceCount() > 0)
			{
				meshBatch->Sync(cmdList);
			}
			else
			{
				meshBatch->Unload(cmdList);
				m_meshBatches.Erase(batchKey);

				LOGINFO() << "[RenderScene] Remove the batch, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;
			}
		}
	}

	const MeshBatchKey RenderScene::GetMeshBatchKey(Memory::RefPtr<PrimitiveProxy> primitive, const size_t sectionIndex)
	{
		if (!primitive)
		{
			return {};
		}

		auto meshAsset = primitive->GetMesh();
		if (!meshAsset)
		{
			return {};
		}

		if (meshAsset->sections.Empty() || sectionIndex >= meshAsset->sections.Size())
		{
			return {};
		}

		const MeshSection& section = meshAsset->sections[sectionIndex];

		Memory::RefPtr<const MaterialAsset> materialAsset = primitive->GetOverrideMaterial();
		if (!materialAsset)
		{
			auto itr = meshAsset->materials.Find(section.materialName);
			if (itr != meshAsset->materials.End())
			{
				materialAsset = itr->second;
			}
		}

		MeshBatchKey batchKey;
		batchKey.meshId = meshAsset->id;
		batchKey.materialId = materialAsset ? materialAsset->id : ECS::UUID::Null();
		batchKey.meshSection = sectionIndex;

		return batchKey;
	}
}