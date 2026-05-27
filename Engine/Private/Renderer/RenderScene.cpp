#include <Renderer/RenderScene.h>

#include <Renderer/Proxy/PrimitiveProxy.h>
#include <Renderer/Proxy/LightProxy.h>
#include <Renderer/Proxy/MaterialProxy.h>
#include <Renderer/GlobalRenderer.h>
#include <Renderer/MeshBatch.h>
#include <RHI/RHIResources.h>
#include <RHI/RHICommandList.h>
#include <Asset/AssetTypes.h>

#include <Memory/include/Core.h>

#include <Log/include/Log.h>

//#define DEBUG_BATCH

namespace wtr
{
	RenderScene::RenderScene()
		: m_primitives()
		, m_lights()
		, m_meshBatches()
		, m_addedProxies()
		, m_removedProxies()
		, m_updatedProxies()
		, m_addedBatches()
		, m_updatedBatches()
	{}

	void RenderScene::RemoveAll(Memory::RefPtr<RHICommandList> cmdList)
	{
		m_primitives.Clear();
		m_lights.Clear();

		for (const auto& [batchKey, meshBatch] : m_meshBatches)
		{
			if (meshBatch)
			{
				meshBatch->Unload(cmdList);
			}
		}

		m_meshBatches.Clear();

		m_addedProxies.Clear();
		m_updatedProxies.Clear();
		m_removedProxies.Clear();

		m_addedBatches.Clear();
		m_updatedBatches.Clear();
	}

	void RenderScene::Flush(Memory::RefPtr<RHICommandList> cmdList)
	{
		FlushAdded(cmdList);
		FlushUpdated(cmdList);
		FlushRemoved(cmdList);
	}

	void RenderScene::AddProxy(Memory::RefPtr<SceneProxy> proxy)
	{
		if (!proxy)
		{
#ifdef DEBUG_BATCH
			LOGERROR() << "[RenderScene] Failed to add the proxy, cause the proxy is invalid";
#endif
			return;
		}

		proxy->OnAttached(this);

		m_addedProxies.Insert(proxy);
		m_removedProxies.Erase(proxy);

		if (auto primitive = Memory::Cast<PrimitiveProxy>(proxy))
		{
			m_primitives[proxy->GetID()] = primitive;
		}
		else if (auto light = Memory::Cast<LightProxy>(proxy))
		{
			m_lights[proxy->GetID()] = light;
		}
		else
		{
#ifdef DEBUG_BATCH
			LOGERROR() << "[RenderScene] Failed to add the proxy, cause the proxy type is invalid, ID : " << proxy->GetID().ToString();
#endif
		}
	}

	void RenderScene::UpdateProxy(Memory::RefPtr<SceneProxy> proxy)
	{
		if (!proxy)
		{
#ifdef DEBUG_BATCH
			LOGERROR() << "[RenderScene] Failed to update the proxy, cause the proxy is invalid";
#endif
			return;
		}

		m_updatedProxies.Insert(proxy);
	}

	void RenderScene::UpdateProxy(const ECS::UUID& id)
	{
		auto primitiveItr = m_primitives.Find(id);
		if (primitiveItr != m_primitives.End())
		{
			UpdateProxy(primitiveItr->second);
		}

		auto lightItr = m_lights.Find(id);
		if (lightItr != m_lights.End())
		{
			UpdateProxy(lightItr->second);
		}
	}

	void RenderScene::RemoveProxy(Memory::RefPtr<SceneProxy> proxy)
	{
		if (!proxy)
		{
#ifdef DEBUG_BATCH
			LOGERROR() << "[RenderScene] Failed to remove the proxy, cause the proxy is invalid";
#endif
			return;
		}

		proxy->OnDetached();

		m_removedProxies.Insert(proxy);

		m_addedProxies.Erase(proxy);
		m_updatedProxies.Erase(proxy);
		
		if (auto primitive = Memory::Cast<PrimitiveProxy>(proxy))
		{
			m_primitives.Erase(proxy->GetID());
		}
		else if (auto light = Memory::Cast<LightProxy>(proxy))
		{
			m_lights.Erase(proxy->GetID());
		}
		else
		{
#ifdef DEBUG_BATCH
			LOGERROR() << "[RenderScene] Failed to remove the proxy, cause the proxy type is invalid, ID : " << proxy->GetID().ToString();
#endif
		}
	}

	void RenderScene::RemoveProxy(const ECS::UUID& id)
	{
		auto primitiveItr = m_primitives.Find(id);
		if (primitiveItr != m_primitives.End())
		{
			RemoveProxy(primitiveItr->second);
		}

		auto lightItr = m_lights.Find(id);
		if (lightItr != m_lights.End())
		{
			RemoveProxy(lightItr->second);
		}
	}

	void RenderScene::FlushAdded(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		auto itr = m_addedProxies.begin();
		while (itr != m_addedProxies.end())
		{
			auto& proxy = *itr;
			if (!proxy)
			{
#ifdef DEBUG_BATCH
				LOGERROR() << "[RenderScene] Failed to add the proxy, cause the proxy is invalid";
#endif
				itr = m_addedProxies.Erase(itr);
				continue;
			}

			if (!proxy->IsUploadable())
			{
				itr++;
				continue;
			}

			if (auto primitiveProxy = Memory::Cast<PrimitiveProxy>(proxy))
			{
				FlushAddedPrimitive(primitiveProxy, cmdList);
			}
			else if (auto lightProxy = Memory::Cast<LightProxy>(proxy))
			{
				FlushAddedLight(lightProxy, cmdList);
			}
			else
			{
#ifdef DEBUG_BATCH
				LOGERROR() << "[RenderScene] Failed to add the proxy, cause the proxy type is invalid, ID : " << proxy->GetID().ToString();
#endif
			}

			proxy->ClearDirty();
			itr = m_addedProxies.Erase(itr);
		}

		FlushAddedBatch(cmdList);
	}

	void RenderScene::FlushUpdated(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		auto itr = m_updatedProxies.begin();
		while (itr != m_updatedProxies.end())
		{
			auto& proxy = *itr;
			if (!proxy)
			{
#ifdef DEBUG_BATCH
				LOGERROR() << "[RenderScene] Failed to update the proxy, cause the proxy is invalid";
#endif
				itr = m_updatedProxies.Erase(itr);
				continue;
			}

			if (!proxy->IsUploadable() || !proxy->IsSyncable())
			{
				itr++;
				continue;
			}

			if (auto primitiveProxy = Memory::Cast<PrimitiveProxy>(proxy))
			{
				FlushUpdatedPrimitive(primitiveProxy, cmdList);
			}
			else if (auto lightProxy = Memory::Cast<LightProxy>(proxy))
			{
				FlushUpdatedLight(lightProxy, cmdList);
			}
			else
			{
#ifdef DEBUG_BATCH
				LOGERROR() << "[RenderScene] Failed to update the proxy, cause the proxy type is invalid, ID : " << proxy->GetID().ToString();
#endif
			}

			proxy->ClearDirty();
			itr = m_updatedProxies.Erase(itr);
		}

		FlushUpdatedBatch(cmdList);
	}

	void RenderScene::FlushRemoved(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}
		auto itr = m_removedProxies.begin();
		while (itr != m_removedProxies.end())
		{
			auto& proxy = *itr;
			if (!proxy)
			{
#ifdef DEBUG_BATCH
				LOGERROR() << "[RenderScene] Failed to remove the proxy, cause the proxy is invalid";
#endif
				itr = m_removedProxies.Erase(itr);
				continue;
			}

			if (auto primitiveProxy = Memory::Cast<PrimitiveProxy>(proxy))
			{
				FlushRemovedPrimitive(primitiveProxy, cmdList);
			}
			else if (auto lightProxy = Memory::Cast<LightProxy>(proxy))
			{
				FlushRemovedLight(lightProxy, cmdList);
			}
			else
			{
#ifdef DEBUG_BATCH
				LOGERROR() << "[RenderScene] Failed to remove the proxy, cause the proxy type is invalid, ID : " << proxy->GetID().ToString();
#endif
			}

			itr = m_removedProxies.Erase(itr);
		}
	}
	
	void RenderScene::FlushAddedPrimitive(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (primitive)
		{
			auto refMeshAsset = primitive->GetMesh();
			if (!refMeshAsset)
			{
#ifdef DEBUG_BATCH
				LOGERROR() << "[RenderScene] Failed to add the batch, cause the mesh asset is invalid" << primitive->GetID().ToString();
#endif
				return;
			}

			Memory::RefPtr<const MaterialAsset> refOverrideMaterial = primitive->GetOverrideMaterial();

			const size_t endIndex = refMeshAsset->sections.Size();
			for (size_t index = 0; index < endIndex; index++)
			{
				const MeshBatchKey batchKey = GetMeshBatchKey(primitive, index);
				Memory::RefPtr<MeshBatch> meshBatch = GetMeshBatch(batchKey);
				if (meshBatch)
				{
					m_updatedBatches.Insert(meshBatch);

#ifdef DEBUG_BATCH
					LOGINFO() << "[RenderScene] Add the mesh batch's transform info, " << batchKey.ToString() << ", the primitive ID : " << primitive->GetID().ToString();
#endif
				}
				else
				{
					meshBatch = Memory::MakeRef<MeshBatch>();
					if (!meshBatch)
					{
#ifdef DEBUG_BATCH
						LOGERROR() << "[RenderScene] Failed to add the batch, cause failed to create the mesh batch, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;
#endif
						continue;
					}

					Memory::RefPtr<const MaterialProxy> refMaterialProxy = nullptr;
					Memory::RefPtr<const MaterialAsset> refMaterial;
					if (refOverrideMaterial)
					{
						refMaterial = refOverrideMaterial;
					}
					else
					{
						auto itr = refMeshAsset->materials.Find(refMeshAsset->sections[index].materialName);
						if (itr != refMeshAsset->materials.End())
						{
							refMaterial = itr->second;
						}
					}

					if (refMaterial)
					{
						Memory::RefPtr<MaterialProxy> materialProxy = Memory::MakeRef<MaterialProxy>(primitive->GetID());
						materialProxy->SetMaterialAsset(refMaterial);
						materialProxy->Upload(cmdList);

						GlobalShaderSelector::SetShader(materialProxy);

						refMaterialProxy = materialProxy;
					}
					else
					{
						refMaterialProxy = GlobalResource::GetDefaultMaterial();
					}

					meshBatch->SetMaterial(refMaterialProxy);
					meshBatch->SetMesh(refMeshAsset, index);

					m_addedBatches.Insert(meshBatch);
					m_meshBatches.Insert(std::make_pair(batchKey, meshBatch));

#ifdef DEBUG_BATCH
					LOGINFO() << "[RenderScene] Add the batch, ID : " << primitive->GetID().ToString() << ", Section Index : " << index;
#endif
				}

				meshBatch->AddTransform(primitive->GetID(), primitive->GetRawData());
				primitive->UpdateBatch(meshBatch);
			}
		}
	}

	void RenderScene::FlushUpdatedPrimitive(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (primitive)
		{
			auto meshAsset = primitive->GetMesh();
			if (!meshAsset || meshAsset->sections.Empty())
			{
#ifdef DEBUG_BATCH
				LOGWARN() << "[RenderScene] Failed to update the batch, cause the mesh asset is invalid, ID : " << primitive->GetID().ToString();
#endif
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

				primitive->UpdateBatch(meshBatch);

				m_updatedBatches.Insert(meshBatch);
			}
		}
	}

	void RenderScene::FlushRemovedPrimitive(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (primitive)
		{
			auto meshAsset = primitive->GetMesh();
			if (!meshAsset || meshAsset->sections.Empty())
			{
#ifdef DEBUG_BATCH
				LOGWARN() << "[RenderScene] Failed to remove the primitive, cause the mesh asset is invalid, ID : " << primitive->GetID().ToString();
#endif
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

				if (!meshBatch->IsEmpty())
				{
					meshBatch->Sync(cmdList);
				}
				else
				{
					RemoveBatch(meshBatch, cmdList);
				}
			}

			m_primitives.Erase(primitive->GetID());

#ifdef DEBUG_BATCH
			LOGINFO() << "[RenderScene] Remove the primitive proxy, ID : " << primitive->GetID().ToString();
#endif
		}
	}

	void RenderScene::FlushAddedLight(Memory::RefPtr<LightProxy> light, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (light)
		{
			light->Upload(cmdList);

			GlobalShaderSelector::SetShader(light);
		}
	}

	void RenderScene::FlushUpdatedLight(Memory::RefPtr<LightProxy> light, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (light)
		{
			light->Sync(cmdList);

			GlobalShaderSelector::SetShader(light);
		}
	}

	void RenderScene::FlushRemovedLight(Memory::RefPtr<LightProxy> light, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (light)
		{
			light->Unload(cmdList);
			m_lights.Erase(light->GetID());

#ifdef DEBUG_BATCH
			LOGINFO() << "[RenderScene] Remove the light proxy, ID : " << light->GetID().ToString();
#endif
		}
	}

	void RenderScene::FlushAddedBatch(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		auto itr = m_addedBatches.begin();
		while (itr != m_addedBatches.end())
		{
			auto& meshBatch = *itr;
			if (meshBatch)
			{
				meshBatch->Upload(cmdList);
			}

			itr = m_addedBatches.Erase(itr);
		}
	}

	void RenderScene::FlushUpdatedBatch(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		auto itr = m_updatedBatches.begin();
		while (itr != m_updatedBatches.end())
		{
			auto& meshBatch = *itr;
			if (meshBatch)
			{
				meshBatch->Sync(cmdList);
			}

			itr = m_updatedBatches.Erase(itr);
		}
	}

	void RenderScene::RemoveBatch(Memory::RefPtr<MeshBatch> meshBatch, Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList)
		{
			return;
		}

		if (meshBatch)
		{
			meshBatch->Unload(cmdList);
			m_meshBatches.Erase(meshBatch->GetKey());

#ifdef DEBUG_BATCH
			LOGINFO() << "[RenderScene] Remove the batch, " << meshBatch->GetKey().ToString();
#endif
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

	const RenderScene::ProxyContainer<LightProxy>& RenderScene::GetLightProxies() const
	{
		return m_lights;
	}

	const MeshBatchKey RenderScene::GetMeshBatchKey(Memory::RefPtr<PrimitiveProxy> primitive, const size_t sectionIndex)
	{
		if (!primitive)
		{
			return {};
		}

		auto refMesh = primitive->GetMesh();
		if (!refMesh)
		{
			return {};
		}

		if (refMesh->sections.Empty() || sectionIndex >= refMesh->sections.Size())
		{
			return {};
		}

		const MeshSection& section = refMesh->sections[sectionIndex];

		Memory::RefPtr<const MaterialAsset> refMaterial = primitive->GetOverrideMaterial();
		if (!refMaterial)
		{
			auto itr = refMesh->materials.Find(section.materialName);
			if (itr != refMesh->materials.End())
			{
				refMaterial = itr->second;
			}
		}

		MeshBatchKey batchKey;
		batchKey.meshId = refMesh->id;
		batchKey.materialId = refMaterial ? refMaterial->id : ECS::UUID::Null();
		batchKey.meshSection = sectionIndex;

		return batchKey;
	}
}