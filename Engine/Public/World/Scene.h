#ifndef __WTR_SCENE_H__
#define __WTR_SCENE_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Memory/include/Pointer/ObjectPtr.h>
#include <Container/include/HashMap.h>
#include <Container/include/HashSet.h>
#include <ECS/include/UUID/UUID.h>

namespace wtr
{
	class BaseNode;
	class SceneComponent;
	class Commander;
};

namespace wtr
{
	class Scene
	{
	private :
		struct ScenePair
		{
			Memory::ObjectPtr<SceneComponent> transform;
			wtr::HashSet<size_t> nodeTypes;
		};

	public :
		Scene();
		~Scene();

	public :
		void SetCommander(Memory::RefPtr<Commander> refCommander);

		void Attach(Memory::ObjectPtr<BaseNode> node);
		void Detach(Memory::ObjectPtr<BaseNode> node);
		void Detach(const ECS::UUID& entityId);
		void DetachAll();
		
		void Update(const ECS::UUID& entityId);

	private :
		void AttachNode(Memory::ObjectPtr<BaseNode> node, Memory::ObjectPtr<SceneComponent> transform);
		void DetachNode(Memory::ObjectPtr<BaseNode> node);

	private :
		Memory::RefPtr<Commander> m_refCommander;

		wtr::HashMap<ECS::UUID, ScenePair> m_sceneDatas;
	};
};

#endif // __WTR_SCENE_H__