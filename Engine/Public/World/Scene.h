#ifndef __WTR_SCENE_H__
#define __WTR_SCENE_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Memory/include/Pointer/ObjectPtr.h>
#include <Container/include/HashMap.h>
#include <Container/include/HashSet.h>
#include <ECS/include/UUID/UUID.h>

namespace wtr
{
	enum class eDirtyProxy : uint8_t;

	class BaseComponent;
	class ProxyNode;
	class Commander;
};

namespace wtr
{
	class Scene
	{
	public :
		struct NodeID
		{
			ECS::UUID id;
			const Reflection::TypeInfo* typeInfo;

			bool operator==(const NodeID& other) const;
			bool operator!=(const NodeID& other) const;
		};

		Scene();
		~Scene();

	public :
		void SetCommander(Memory::RefPtr<Commander> refCommander);

		void Flush();

		void Attach(Memory::ObjectPtr<ProxyNode> node);
		void Detach(Memory::ObjectPtr<ProxyNode> node);
		void Detach(const ECS::UUID& entityId);
		void DetachAll();
		
		void Update(BaseComponent* component);

	private :
		void FlushAdded();
		void FlushRemoved();
		void FlushUpdated();

		Memory::ObjectPtr<ProxyNode> GetProxyNode(const NodeID& id) const;

	private :
		Memory::RefPtr<Commander> m_refCommander;

		wtr::HashMap<NodeID, Memory::ObjectPtr<ProxyNode>> m_proxies;
		wtr::HashMap<ECS::UUID, wtr::HashSet<NodeID>> m_proxyIDs;

		wtr::HashSet<NodeID> m_addedProxies;
		wtr::HashSet<NodeID> m_removedProxies;
		wtr::HashSet<NodeID> m_updatedProxies;
	};
};

namespace std
{
	template<>
	struct hash<wtr::Scene::NodeID>
	{
		size_t operator()(const wtr::Scene::NodeID& id) const
		{
			size_t seed = std::hash<ECS::UUID>()(id.id);
			seed ^= id.typeInfo->GetTypeHash()	+ 0x9e3779b9 + (seed << 6) + (seed >> 2);

			return seed;
		}
	};
};

#endif // __WTR_SCENE_H__