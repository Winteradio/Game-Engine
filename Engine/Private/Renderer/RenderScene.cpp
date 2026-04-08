#include <Renderer/RenderScene.h>

#include <Renderer/Proxy/PrimitiveProxy.h>
#include <Renderer/Proxy/LightProxy.h>
#include <Renderer/MeshBatch.h>
#include <RHI/RHIResources.h>
#include <Asset/AssetTypes.h>

#include <Memory/include/Core.h>

#include <Log/include/Log.h>

namespace wtr
{
	RenderScene::RenderScene()
		: m_primitives()
		, m_lights()
		, m_meshBatches()
		, m_addable()
		, m_removable()
		, m_updatable()
	{}

	RenderScene::~RenderScene()
	{}

	void RenderScene::RemoveAll()
	{
		m_primitives.Clear();
		m_lights.Clear();

		for (auto meshBatch : m_addable)
		{
			m_removable.Insert(meshBatch);
		}

		for (auto& [batchKey, meshBatch] : m_meshBatches)
		{
			m_removable.Insert(meshBatch);
		}

		m_addable.Clear();
		m_updatable.Clear();
	}

	void RenderScene::Clear()
	{
		m_primitives.Clear();
		m_lights.Clear();
		m_meshBatches.Clear();
		m_addable.Clear();
		m_updatable.Clear();
		m_removable.Clear();
	}

	void RenderScene::FlushPending()
	{
		FlushPrimitive();
		FlushAddable();
		FlushUpdatable();
		FlushRemovable();
	}

	void RenderScene::UpdateProxy(const ECS::UUID& id, const fvec3 position, const fvec3 rotation, const fvec3 scale)
	{
		auto itrPrimitive = m_primitives.Find(id);
		if (itrPrimitive != m_primitives.End())
		{
			auto& primitiveProxy = itrPrimitive->second;
			primitiveProxy->UpdatePosition(position);
			primitiveProxy->UpdateRotation(rotation);
			primitiveProxy->UpdateScale(scale);

			UpdateBatch(primitiveProxy);

			return;
		}

		auto itrLight = m_lights.Find(id);
		if (itrLight != m_lights.End())
		{
			auto& lightProxy = itrLight->second;

			lightProxy->UpdatePosition(position);
			lightProxy->UpdateRotation(rotation);
			lightProxy->UpdateScale(scale);
		}
	}

	void RenderScene::AddPrimitive(Memory::RefPtr<PrimitiveProxy> primitive)
	{
		if (!primitive)
		{
			return;
		}

		m_pendingPrimitives.PushBack(primitive);

		LOGINFO() << "[RenderScene] Add the primitive proxy, ID : " << primitive->GetID().ToString();
	}

	void RenderScene::RemovePrimitive(const ECS::UUID& id)
	{
		auto itr = m_primitives.Find(id);
		if (itr != m_primitives.End())
		{
			auto& primitiveProxy = itr->second;

			RemoveBatch(primitiveProxy);

			LOGINFO() << "[RenderScene] Remove the primitive proxy, ID : " << id.ToString();
		}
	}

	Memory::RefPtr<PrimitiveProxy> RenderScene::GetPrimitive(const ECS::UUID& id)
	{
		auto itr = m_primitives.Find(id);
		if (itr != m_primitives.End())
		{
			return itr->second;
		}
		else
		{
			return {};
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

	Memory::RefPtr<LightProxy> RenderScene::GetLight(const ECS::UUID& id)
	{
		auto itr = m_lights.Find(id);
		if (itr != m_lights.End())
		{
			return itr->second;
		}
		else
		{
			return {};
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

	RenderScene::PendingBatch& RenderScene::GetAddable()
	{
		return m_addable;
	}

	RenderScene::PendingBatch& RenderScene::GetRemovable()
	{
		return m_removable;
	}

	RenderScene::PendingBatch& RenderScene::GetUpdatable()
	{
		return m_updatable;
	}

	void RenderScene::FlushPrimitive()
	{
		auto itr = m_pendingPrimitives.begin();
		while (itr != m_pendingPrimitives.end())
		{
			auto& primitive = *itr;
			if (!primitive || !primitive->GetMesh())
			{
				LOGINFO() << "[RENDER SCENE] Failed to add the primitive proxy, cause the primitive proxy is invalid, ID : " << primitive->GetID().ToString();
				itr = m_pendingPrimitives.Erase(itr);
				continue;
			}

			auto mesh = primitive->GetMesh();
			if (mesh->GetResourceState() >= eResourceState::eLoaded)
			{
				AddBatch(primitive);

				itr = m_pendingPrimitives.Erase(itr);
				continue;
			}
			else
			{
				itr++;
			}
		}
	}

	void RenderScene::FlushAddable()
	{
		auto itr = m_addable.begin();
		while (itr != m_addable.end())
		{
			auto& meshBatch = *itr;
			if (!meshBatch)
			{
				LOGINFO() << "[RENDER SCENE] Failed to add the mesh batch, cause the mesh batch is invalid";

				itr = m_addable.Erase(itr);
				continue;
			}

			if (meshBatch->GetResourceState() == eResourceState::eReady)
			{
				LOGINFO() << "[RENDER SCENE] Add the mesh batch, cause the mesh batch is in ready state";
				m_meshBatches[meshBatch->GetKey()] = meshBatch;
				itr = m_addable.Erase(itr);
				continue;
			}
			else if (meshBatch->GetResourceState() == eResourceState::eError)
			{
				LOGINFO() << "[RENDER SCENE] Failed to add the mesh batch, cause the mesh batch is in error state";
				itr = m_addable.Erase(itr);
				continue;
			}
			else
			{
				itr++;
			}
		}
	}

	void RenderScene::FlushUpdatable()
	{
		auto itr = m_updatable.begin();
		while (itr != m_updatable.end())
		{
			auto& meshBatch = *itr;
			if (!meshBatch)
			{
				LOGINFO() << "[RENDER SCENE] Failed to update the mesh batch, cause the mesh batch is invalid";
				itr = m_updatable.Erase(itr);
				continue;
			}

			if (meshBatch->GetResourceState() == eResourceState::eReady)
			{
				LOGINFO() << "[RENDER SCENE] Update the mesh batch, cause the mesh batch is in ready state";
				itr = m_updatable.Erase(itr);
				continue;
			}
			else if (meshBatch->GetResourceState() == eResourceState::eError)
			{
				LOGINFO() << "[RENDER SCENE] Failed to update the mesh batch, cause the mesh batch is in error state";
				itr = m_updatable.Erase(itr);
				continue;
			}
			else
			{
				itr++;
			}
		}
	}

	void RenderScene::FlushRemovable()
	{
		auto itr = m_removable.begin();
		while (itr != m_removable.end())
		{
			auto& meshBatch = *itr;
			if (!meshBatch)
			{
				LOGINFO() << "[RENDER SCENE] Failed to remove the mesh batch, cause the mesh batch is invalid";

				itr = m_removable.Erase(itr);
				continue;
			}

			if (meshBatch->GetResourceState() == eResourceState::eNone)
			{
				LOGINFO() << "[RENDER SCENE] Remove the mesh batch, the mesh batch is in none state";

				itr = m_removable.Erase(itr);
				m_meshBatches.Erase(meshBatch->GetKey());

				continue;
			}
			else if (meshBatch->GetResourceState() == eResourceState::eError)
			{
				LOGINFO() << "[RENDER SCENE] Failed to remove the mesh batch, cause the mesh batch is in error state";

				itr = m_removable.Erase(itr);
				m_meshBatches.Erase(meshBatch->GetKey());

				continue;
			}
			else
			{
				itr++;
			}
		}
	}

	void RenderScene::AddBatch(Memory::RefPtr<PrimitiveProxy> primitive)
	{
		if (!primitive)
		{
			return;
		}
		
		auto meshAsset = primitive->GetMesh();
		if (!meshAsset || meshAsset->sections.Empty())
		{
			LOGINFO() << "[RenderScene] Failed to add the batch, cause the mesh asset is invalid, ID : " << primitive->GetID().ToString();
			return;
		}

		const size_t endIndex = meshAsset->sections.Size();
		for (size_t index = 0; index < endIndex; index++)
		{
			auto& section = meshAsset->sections[index];
			if (section.minVertexIndex >= section.maxVertexIndex)
			{
				LOGINFO() << "[RenderScene] Failed to add the batch, cause the mesh section's vertext index range is invalid, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;
				return;
			}

			Memory::RefPtr<MaterialAsset> materialAsset = primitive->GetOverrideMaterial();
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
			batchKey.meshSection = index;

			if (GetMeshBatch(batchKey))
			{
				LOGINFO() << "[RenderScene] Failed to add the batch, cause the mesh batch with the same key already exists, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;

				continue;
			}

			Memory::RefPtr<MeshBatch> meshBatch = Memory::MakeRef<MeshBatch>();
			if (!meshBatch)
			{
				LOGINFO() << "[RenderScene] Failed to add the batch, cause failed to create the mesh batch, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;
				continue;
			}

			meshBatch->SetMesh(meshAsset, index);
			meshBatch->SetMaterial(materialAsset);
			if (!meshBatch->Init())
			{
				LOGINFO() << "[RenderScene] Failed to add the batch, cause failed to create the mesh batch, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;
				continue;
			}

			m_addable.Insert(meshBatch);
			LOGINFO() << "[RenderScene] Add the batch, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;
		}
	}
	
	void RenderScene::UpdateBatch(Memory::RefPtr<PrimitiveProxy> primitive)
	{
		if (!primitive)
		{
			return;
		}

		auto meshAsset = primitive->GetMesh();
		if (!meshAsset || meshAsset->sections.Empty())
		{
			LOGINFO() << "[RenderScene] Failed to add the batch, cause the mesh asset is invalid, ID : " << primitive->GetID().ToString();
			return;
		}

		const size_t endIndex = meshAsset->sections.Size();
		for (size_t index = 0; index < endIndex; index++)
		{
			auto& section = meshAsset->sections[index];
			if (section.minVertexIndex >= section.maxVertexIndex)
			{
				LOGINFO() << "[RenderScene] Failed to add the batch, cause the mesh section's vertext index range is invalid, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;
				return;
			}

			Memory::RefPtr<MaterialAsset> materialAsset = nullptr;
			auto itr = meshAsset->materials.Find(section.materialName);
			if (itr != meshAsset->materials.End())
			{
				materialAsset = itr->second;
			}

			MeshBatchKey batchKey;
			batchKey.meshId = meshAsset->id;
			batchKey.materialId = materialAsset ? materialAsset->id : ECS::UUID::Null();
			batchKey.meshSection = index;

			Memory::RefPtr<MeshBatch> meshBatch = GetMeshBatch(batchKey);
			if (meshBatch)
			{
				wtr::fmat4 transform = wtr::fmat4(1.0f);
				transform *= glm::translate(transform, primitive->GetPosition());

				// TODO : Make the angle by quaternion to avoid gimble lock
				meshBatch->UpdateTransform(primitive->GetID(), transform);

				m_updatable.Insert(meshBatch);
			}
		}
	}

	void RenderScene::RemoveBatch(Memory::RefPtr<PrimitiveProxy> primitive)
	{
		if (!primitive)
		{
			return;
		}
		auto meshAsset = primitive->GetMesh();
		if (!meshAsset || meshAsset->sections.Empty())
		{
			LOGINFO() << "[RenderScene] Failed to remove the batch, cause the mesh asset is invalid, ID : " << primitive->GetID().ToString();
			return;
		}
		const size_t endIndex = meshAsset->sections.Size();
		for (size_t index = 0; index < endIndex; index++)
		{
			auto& section = meshAsset->sections[index];
			if (section.minVertexIndex >= section.maxVertexIndex)
			{
				LOGINFO() << "[RenderScene] Failed to remove the batch, cause the mesh section's vertext index range is invalid, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;
				return;
			}

			Memory::RefPtr<MaterialAsset> materialAsset = nullptr;
			auto itr = meshAsset->materials.Find(section.materialName);
			if (itr != meshAsset->materials.End())
			{
				materialAsset = itr->second;
			}

			MeshBatchKey batchKey;
			batchKey.meshId = meshAsset->id;
			batchKey.materialId = materialAsset ? materialAsset->id : ECS::UUID::Null();
			batchKey.meshSection = index;

			Memory::RefPtr<MeshBatch> meshBatch = GetMeshBatch(batchKey);
			if (meshBatch)
			{
				m_meshBatches.Erase(batchKey);
				m_removable.Insert(meshBatch);

				LOGINFO() << "[RenderScene] Remove the batch, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;
			}
		}
	}
}