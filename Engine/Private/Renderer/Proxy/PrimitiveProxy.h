#ifndef __WTR_PRIMTIVEPROXY_H__
#define __WTR_PRIMTIVEPROXY_H__

#include <Renderer/Proxy/SceneProxy.h>
#include <Framework/Math/MathTypes.h>

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/DynamicArray.h>
#include <Container/include/HashSet.h>

namespace wtr
{
	template<typename T>
	class ArrayData; 

	template<typename T>
	class ScalarData;

	enum class eRenderDirty : uint8_t;

	class RawData;
	class MeshBatch;
	class MeshAsset;
	class MaterialAsset;
};

namespace wtr
{
	class PrimitiveProxy : public SceneProxy
	{
		GENERATE(PrimitiveProxy);
	public :
		PrimitiveProxy(const ECS::UUID& id);
		virtual ~PrimitiveProxy() = default;

	public :
		virtual Memory::RefPtr<const RawData> GetRawData() const = 0;
		virtual void UpdateBatch(Memory::RefPtr<MeshBatch> meshBatch) = 0;
		bool IsUploadable() const override;

	public :
		void SetMesh(Memory::RefPtr<const MeshAsset> refMesh);
		void SetOverrideMaterial(Memory::RefPtr<const MaterialAsset> refOverrideMaterial);

		Memory::RefPtr<const MeshAsset> GetMesh() const;
		Memory::RefPtr<const MaterialAsset> GetOverrideMaterial() const;

	protected :
		Memory::RefPtr<const MeshAsset> m_refMesh;
		Memory::RefPtr<const MaterialAsset> m_refOverrideMaterial;
	};

	class SinglePrimitiveProxy : public PrimitiveProxy
	{
		GENERATE(SinglePrimitiveProxy);
	public:
		SinglePrimitiveProxy(const ECS::UUID& id);
		virtual ~SinglePrimitiveProxy() = default;

	public:
		Memory::RefPtr<const RawData> GetRawData() const override;
		bool IsSyncable() const override;

	protected :
		Memory::RefPtr<ScalarData<ftransform>> m_transformData;
	};

	class StaticPrimitiveProxy : public SinglePrimitiveProxy
	{
		GENERATE(StaticPrimitiveProxy);
	public :
		StaticPrimitiveProxy(const ECS::UUID& id);
		virtual ~StaticPrimitiveProxy() = default;

	public :
		void UpdateBatch(Memory::RefPtr<MeshBatch> meshBatch) override;
	};

	// TODO : Implement DynamicPrimitiveProxy for dynamic objects that can be animated or have changing geometry. 
	// This may involve using a different approach for storing and updating transform data, 
	// such as using a structured buffer or a more flexible data structure to accommodate changes in the number of instances or their transforms.
	class DynamicPrimitiveProxy : public SinglePrimitiveProxy
	{
		GENERATE(DynamicPrimitiveProxy);
	public:
		DynamicPrimitiveProxy(const ECS::UUID& id);
		virtual ~DynamicPrimitiveProxy() = default;

	public:
		void UpdateBatch(Memory::RefPtr<MeshBatch> meshBatch) override;

	public :
		void UpdateMesh(Memory::RefPtr<const MeshAsset> refMesh);
	};

	class InstancedStaticPrimitiveProxy : public PrimitiveProxy
	{
		GENERATE(InstancedStaticPrimitiveProxy);
	public :
		using SceneProxy::GetPosition;
		using SceneProxy::GetRotation;
		using SceneProxy::GetScale;

		InstancedStaticPrimitiveProxy(const ECS::UUID& id);
		virtual ~InstancedStaticPrimitiveProxy() = default;

	public :
		Memory::RefPtr<const RawData> GetRawData() const override;
		bool IsSyncable() const override;
		void UpdateBatch(Memory::RefPtr<MeshBatch> meshBatch) override;

	public :
		void SetInstanceCount(const size_t instanceCount);
		void UpdateTransform(const size_t instanceIndex, const ftransform& transform);

		const size_t GetInstanceCount() const;

	private :
		wtr::DynamicArray<ftransform> m_instanceTransforms;
		wtr::HashSet<size_t> m_dirtyInstances;

		Memory::RefPtr<ArrayData<ftransform>> m_transformData;
	};

	struct PrimitiveProxyHasher
	{
		size_t operator()(const PrimitiveProxy& proxy) const
		{
			return std::hash<ECS::UUID>()(proxy.GetID());
		}

		size_t operator()(const Memory::RefPtr<PrimitiveProxy>& proxy) const
		{
			return std::hash<ECS::UUID>()(proxy ? proxy->GetID() : ECS::UUID::Null());
		}
	};
}

#endif // __WTR_PRIMTIVEPROXY_H__