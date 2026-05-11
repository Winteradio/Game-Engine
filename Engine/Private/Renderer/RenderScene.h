#ifndef __WTR_RENDERSCENE_H__
#define __WTR_RENDERSCENE_H__

#include <ECS/include/UUID/UUID.h>
#include <Container/include/HashSet.h>
#include <Container/include/HashMap.h>
#include <Container/include/DynamicArray.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
	class SceneProxy;
	class PrimitiveProxy;
	class LightProxy;

	struct SceneProxyHasher;
	struct UpdateProxyInfo;

	class MeshBatch;
	
	struct MeshBatchKey;
	struct MeshBatchHasher;

	class RHICommandList;
};

namespace wtr
{
	class RenderScene
	{
		public :
			template<typename T>
			using ProxyContainer = wtr::HashMap<ECS::UUID, Memory::RefPtr<T>>;

			using MeshBatchContainer = wtr::HashMap<MeshBatchKey, Memory::RefPtr<MeshBatch>, MeshBatchHasher>;

			using PendingProxy = wtr::HashSet<Memory::RefPtr<SceneProxy>, SceneProxyHasher>;
			using PendingBatch = wtr::HashSet<Memory::RefPtr<MeshBatch>, MeshBatchHasher>;
			
		public:
			RenderScene();
			~RenderScene() = default;

		public :
			void RemoveAll(Memory::RefPtr<RHICommandList> cmdList);
			void Flush(Memory::RefPtr<RHICommandList> cmdList);

			void AddProxy(Memory::RefPtr<SceneProxy> proxy);

			void UpdateProxy(Memory::RefPtr<SceneProxy> proxy);
			void UpdateProxy(const ECS::UUID& id);

			void RemoveProxy(Memory::RefPtr<SceneProxy> proxy);
			void RemoveProxy(const ECS::UUID& id);

			Memory::RefPtr<MeshBatch> GetMeshBatch(const MeshBatchKey& key);
			const MeshBatchContainer& GetMeshBatches() const;

		public :
			template<typename T>
			Memory::RefPtr<T> GetPrimitiveProxy(const ECS::UUID& id)
			{
				auto itr = m_primitives.Find(id);
				if (itr == m_primitives.End())
				{
					return nullptr;
				}

				Memory::RefPtr<T> proxy = Memory::Cast<T>(itr->second);
				if (proxy)
				{
					return proxy;
				}
				else
				{
					return nullptr;
				}
			}

			template<typename T>
			Memory::RefPtr<T> GetLightProxy(const ECS::UUID& id)
			{
				auto itr = m_lights.Find(id);
				if (itr == m_lights.End())
				{
					return nullptr;
				}

				Memory::RefPtr<T> proxy = Memory::Cast<T>(itr->second);
				if (proxy)
				{
					return proxy;
				}
				else
				{
					return nullptr;
				}
			}

		private :
			void FlushAdded(Memory::RefPtr<RHICommandList> cmdList);
			void FlushUpdated(Memory::RefPtr<RHICommandList> cmdList);
			void FlushRemoved(Memory::RefPtr<RHICommandList> cmdList);

			void FlushAddedPrimitive(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList);
			void FlushUpdatedPrimitive(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList);
			void FlushRemovedPrimitive(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList);

			void FlushAddedLight(Memory::RefPtr<LightProxy> light, Memory::RefPtr<RHICommandList> cmdList);
			void FlushUpdatedLight(Memory::RefPtr<LightProxy> light, Memory::RefPtr<RHICommandList> cmdList);
			void FlushRemovedLight(Memory::RefPtr<LightProxy> light, Memory::RefPtr<RHICommandList> cmdList);

			void FlushAddedBatch(Memory::RefPtr<RHICommandList> cmdList);
			void FlushUpdatedBatch(Memory::RefPtr<RHICommandList> cmdList);
			void RemoveBatch(Memory::RefPtr<MeshBatch> meshBatch, Memory::RefPtr<RHICommandList> cmdList);

			const MeshBatchKey GetMeshBatchKey(Memory::RefPtr<PrimitiveProxy> primitive, const size_t sectionIndex);

		private :
			ProxyContainer<PrimitiveProxy> m_primitives;
			ProxyContainer<LightProxy> m_lights;

			MeshBatchContainer m_meshBatches;

			PendingProxy m_addedProxies;
			PendingProxy m_updatedProxies;
			PendingProxy m_removedProxies;

			PendingBatch m_addedBatches;
			PendingBatch m_updatedBatches;
	};
};

#endif // __WTR_RENDERSCENE_H__