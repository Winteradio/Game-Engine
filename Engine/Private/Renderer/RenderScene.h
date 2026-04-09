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
			
			using PendingPrimitive = wtr::DynamicArray<Memory::RefPtr<PrimitiveProxy>>;
			using PendingBatch = wtr::HashSet<Memory::RefPtr<MeshBatch>, MeshBatchHasher>;
			
		public:
			RenderScene();
			~RenderScene();

		public :
			void RemoveAll();
			void Flush(Memory::RefPtr<RHICommandList> cmdList);

			void UpdateProxy(const ECS::UUID& id, const fvec3 position, const fvec3 rotation, const fvec3 scale);

			void AddPrimitive(Memory::RefPtr<PrimitiveProxy> primitive);
			void RemovePrimitive(const ECS::UUID& id);
			Memory::RefPtr<PrimitiveProxy> GetPrimitive(const ECS::UUID& id);

			void AddLight(Memory::RefPtr<LightProxy> light);
			void RemoveLight(const ECS::UUID& id);
			Memory::RefPtr<LightProxy> GetLight(const ECS::UUID& id);

			Memory::RefPtr<MeshBatch> GetMeshBatch(const MeshBatchKey& key);
			const MeshBatchContainer& GetMeshBatches() const;

		private :
			void Clear();

			void FlushPrimitive();
			void FlushAddable(Memory::RefPtr<RHICommandList> cmdList);
			void FlushUpdatable(Memory::RefPtr<RHICommandList> cmdList);
			void FlushRemovable(Memory::RefPtr<RHICommandList> cmdList);
			
			void AddBatch(Memory::RefPtr<PrimitiveProxy> primitive);
			void UpdateBatch(Memory::RefPtr<PrimitiveProxy> primitive);
			void RemoveBatch(Memory::RefPtr<PrimitiveProxy> primitive);

		private :
			ProxyContainer<PrimitiveProxy> m_primitives;
			ProxyContainer<LightProxy> m_lights;

			MeshBatchContainer m_meshBatches;

			PendingPrimitive m_pendingPrimitives;
			PendingBatch m_addable;
			PendingBatch m_removable;
			PendingBatch m_updatable;
	};
};

#endif // __WTR_RENDERSCENE_H__