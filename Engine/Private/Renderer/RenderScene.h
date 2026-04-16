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
	struct PrimitiveProxyHasher;
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
			
			using PendingPrimitive = wtr::HashSet<Memory::RefPtr<PrimitiveProxy>, PrimitiveProxyHasher>;
			using PendingBatch = wtr::HashSet<Memory::RefPtr<MeshBatch>, MeshBatchHasher>;
			
		public:
			RenderScene();
			~RenderScene();

		public :
			void RemoveAll(Memory::RefPtr<RHICommandList> cmdList);
			void Flush(Memory::RefPtr<RHICommandList> cmdList);

			void UpdateProxy(const UpdateProxyInfo& updateInfo, Memory::RefPtr<RHICommandList> cmdList);

			void AddPrimitive(Memory::RefPtr<PrimitiveProxy> primitive);
			void RemovePrimitive(const ECS::UUID& id, Memory::RefPtr<RHICommandList> cmdList);

			void AddLight(Memory::RefPtr<LightProxy> light);
			void RemoveLight(const ECS::UUID& id);

			Memory::RefPtr<MeshBatch> GetMeshBatch(const MeshBatchKey& key);
			const MeshBatchContainer& GetMeshBatches() const;

		private :
			void Clear();
			
			void FlushPending(Memory::RefPtr<RHICommandList> cmdList);
			void FlushUpdated(Memory::RefPtr<RHICommandList> cmdList);
			void FlushBatch(Memory::RefPtr<RHICommandList> cmdList);

			void AddBatch(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList);
			void UpdateBatch(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList);
			void RemoveBatch(Memory::RefPtr<PrimitiveProxy> primitive, Memory::RefPtr<RHICommandList> cmdList);

			const MeshBatchKey GetMeshBatchKey(Memory::RefPtr<PrimitiveProxy> primitive, const size_t sectionIndex);

		private :
			ProxyContainer<PrimitiveProxy> m_primitives;
			ProxyContainer<LightProxy> m_lights;

			MeshBatchContainer m_meshBatches;

			PendingPrimitive m_pendingPrimitives;
			PendingPrimitive m_updatedPrimitives;

			PendingBatch m_addedBatches;
			PendingBatch m_updatedBatches;
	};
};

#endif // __WTR_RENDERSCENE_H__