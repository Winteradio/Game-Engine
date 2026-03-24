#ifndef __WTR_SCENE_H__
#define __WTR_SCENE_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Memory/include/Pointer/ObjectPtr.h>
#include <Container/include/HashMap.h>
#include <ECS/include/UUID/UUID.h>

namespace wtr
{
	class SceneNode;
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
		void Attach(Memory::ObjectPtr<SceneNode> node);
		void Detach(const ECS::UUID& nodeId);
		void Update(const ECS::UUID& nodeId);

	private :
		Memory::RefPtr<Commander> m_refCommander;
		wtr::HashMap<ECS::UUID, Memory::ObjectPtr<SceneNode>> m_nodes;
	};
};

#endif // __WTR_SCENE_H__