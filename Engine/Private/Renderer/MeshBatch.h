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
	enum class eTextureSlot : uint8_t;
	enum class eVectorSlot : uint8_t;
	enum class eScalarSlot : uint8_t;
	enum class eResourceState : uint8_t;

	struct VertexKey;
	struct MeshBatchKey;
	struct MeshDrawCommand;
	struct TransformInfo;

	class MeshAsset;
	class MaterialAsset;

	class RHIBuffer;
	class RHIVertexLayout;
	class RHITexture;
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

		void SetMesh(Memory::RefPtr<const MeshAsset> refMesh, const size_t meshSection);
		void SetMaterial(Memory::RefPtr<const MaterialAsset> refMaterial);

		void AddTransform(const ECS::UUID& id, const fmat4& transform);
		void UpdateTransform(const ECS::UUID& id, const fmat4& transform);
		void RemoveTransform(const ECS::UUID& id);

		const size_t GetInstanceCount() const;
		Memory::RefPtr<const MeshDrawCommand> GetDrawCommand() const;

		const MeshBatchKey GetKey() const;
		const std::string ToString() const;

	private :
		wtr::HashMap<ECS::UUID, TransformInfo> m_transformInfos;
		Memory::RefPtr<RHIBuffer> m_transformBuffer;
		Memory::RefPtr<RHIVertexLayout> m_vertexLayout;

		Memory::RefPtr<MeshDrawCommand> m_refDrawCommand;
		
		Memory::RefPtr<const MeshAsset> m_refMesh;
		Memory::RefPtr<const MaterialAsset> m_refMaterial;
		size_t m_sectionIndex;
	};

	struct MeshDrawCommand
	{
		// Mesh
		Memory::RefPtr<const RHIVertexLayout> vertexLayout;
		uint32_t indexOffset = 0;
		uint32_t indexCount = 0;
		uint32_t minVertexIndex = 0;
		uint32_t maxVertexIndex = 0;

		// Material
		wtr::HashMap<eTextureSlot, Memory::RefPtr<const RHITexture>> textureSlots;
		wtr::HashMap<eVectorSlot, fvec3> vectorValues;
		wtr::HashMap<eScalarSlot, float> scalarValues;

		// Instance
		Memory::RefPtr<RHIBuffer> instanceBuffer;
		uint32_t instanceCount = 0;
	};

	struct MeshBatchKey
	{
		ECS::UUID meshId;
		ECS::UUID materialId;
		size_t meshSection = 0;

		bool operator==(const MeshBatchKey& other) const
		{
			return meshId == other.meshId && materialId == other.materialId && meshSection == other.meshSection;
		}
	};

	struct MeshBatchHasher
	{
		size_t operator()(const MeshBatchKey& key) const
		{
			size_t seed = 0;

			auto combine = [&seed](const auto& value)
			{
				size_t hashValue = std::hash<std::decay_t<decltype(value)>>()(value);
				seed ^= hashValue + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			};

			combine(key.meshId);
			combine(key.materialId);
			combine(key.meshSection);

			return seed;
		}

		size_t operator()(const Memory::RefPtr<MeshBatch>& refMeshBatch) const
		{
			if (!refMeshBatch)
			{
				return 0;
			}

			return operator()(refMeshBatch->GetKey());
		}
	};

	struct TransformInfo
	{
		fmat4 transform = fmat4(1.0);
		bool dirty = true;
	};
};

#endif // __WTR_MESHBATCH_H__