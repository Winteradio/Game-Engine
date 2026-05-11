#ifndef __WTR_MESHBATCH_H__
#define __WTR_MESHBATCH_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>
#include <Container/include/HashMap.h>
#include <ECS/include/UUID/UUID.h>

#include <Framework/Math/MathTypes.h>
#include <Renderer/RenderResource.h>

#include <atomic>

namespace wtr
{
	enum class eResourceState : uint8_t;
	enum class eRenderDirty : uint8_t;

	struct DataInfo;
	struct MeshDrawCommand;
	struct MeshBatchKey;
	struct ProxyInfo;

	class MeshAsset;
	class MaterialProxy;
	class RawData;
};

namespace wtr
{
	class MeshBatch : public RenderResource
	{
	public :
		MeshBatch();
		virtual ~MeshBatch() = default;

	public :
		void Upload(Memory::RefPtr<RHICommandList> cmdList) override;
		void Unload(Memory::RefPtr<RHICommandList> cmdList) override;
		void Sync(Memory::RefPtr<RHICommandList> cmdList) override;

		eResourceState GetResourceState() const override;

	public :
		void Clear();

		void SetMaterial(Memory::RefPtr<const MaterialProxy> refMaterial);
		void SetMesh(Memory::RefPtr<const MeshAsset> refMesh, const size_t meshSection);
		void UpdateMesh();

		void AddTransform(const ECS::UUID& id, Memory::RefPtr<const RawData> transform);
		void UpdateTransform(const ECS::UUID& id);
		void RemoveTransform(const ECS::UUID& id);

		bool HasTransform(const ECS::UUID& id) const;

		Memory::RefPtr<const MeshDrawCommand> GetDrawCommand() const;

		const MeshBatchKey GetKey() const;
		bool IsEmpty() const;

	private :
		void SyncTransforms(Memory::RefPtr<RHICommandList> cmdList);
		void SyncMesh(Memory::RefPtr<RHICommandList> cmdList);
		void SyncMaterial();

		void SetDirty(const eRenderDirty dirty);
		void ClearDirty();

	private :
		wtr::HashMap<ECS::UUID, size_t> m_proxyIndices;
		wtr::DynamicArray<ProxyInfo> m_proxyInfos;
		Memory::RefPtr<MeshDrawCommand> m_drawCommand;

		Memory::RefPtr<const MeshAsset> m_refMesh;
		Memory::RefPtr<const MaterialProxy> m_refMaterial;
		size_t m_sectionIndex;

		eRenderDirty m_dirty;
	};

	struct ProxyInfo
	{
		ECS::UUID id = ECS::UUID::Null();
		bool dirty = false;
		size_t prevCount = 0;
		Memory::RefPtr<const RawData> transform;
	};

	struct MeshBatchKey
	{
		ECS::UUID meshId = ECS::UUID::Null();
		ECS::UUID materialId = ECS::UUID::Null();
		size_t meshSection = 0;

		bool operator==(const MeshBatchKey& other) const;
		const std::string ToString() const;
	};

	struct MeshBatchHasher
	{
		size_t operator()(const MeshBatchKey& key) const;
		size_t operator()(const Memory::RefPtr<MeshBatch>& refMeshBatch) const;
	};
};

#endif // __WTR_MESHBATCH_H__