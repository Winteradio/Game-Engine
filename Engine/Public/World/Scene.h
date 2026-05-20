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

	class ProxyNode;
	class Commander;
};

namespace wtr
{
	class Scene
	{
	public :
		Scene();
		~Scene();

	public :
		void SetCommander(Memory::RefPtr<Commander> refCommander);

		void Flush();

		void Attach(Memory::ObjectPtr<ProxyNode> node);
		void Detach(Memory::ObjectPtr<ProxyNode> node);
		void Detach(const ECS::UUID& entityId);
		void DetachAll();
		
		void Update(const ECS::UUID& entityId);

	private :
		void FlushAdded();
		void FlushRemoved();
		void FlushUpdated();

		Memory::ObjectPtr<ProxyNode> GetProxyNode(const ECS::UUID& id) const;

	private :
		Memory::RefPtr<Commander> m_refCommander;

		wtr::HashMap<ECS::UUID, Memory::ObjectPtr<ProxyNode>> m_proxies;

		wtr::HashSet<ECS::UUID> m_addedProxies;
		wtr::HashSet<ECS::UUID> m_removedProxies;
		wtr::HashSet<ECS::UUID> m_updatedProxies;
	};
};

#endif // __WTR_SCENE_H__