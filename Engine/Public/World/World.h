#ifndef __WTR_WORLD_H__
#define __WTR_WORLD_H__

#include <ECS/include/Container/Registry.h>
#include <ECS/include/Container/SystemRegistry.h>
#include <World/Scene.h>
#include <World/Node.h>
#include <World/Component.h>

namespace wtr
{
	class Entity;
	class Commander;
};

namespace wtr
{
	class World : public ECS::Object
	{
		GENERATE(World);

	public  :
		PROPERTY(scene);
		Scene scene;

	public :
		World();
		virtual ~World();

	public :
		bool Init(Memory::RefPtr<Commander> refCommander);
		void Clear();

		void Update(const ECS::TimeStep& timeStep);

		Memory::ObjectPtr<Entity> CreateEntity();
		void RegisterEntity(Memory::ObjectPtr<Entity> entity);

		void RemoveEntity(const ECS::UUID& uuid);
		void RemoveSystem(const ECS::UUID& uuid);
		void RemoveComponent(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo);
		void RemoveNode(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo);

		Memory::ObjectPtr<Entity> GetEntity(const ECS::UUID& uuid);
		Memory::ObjectPtr<ECS::System> GetSystem(const ECS::UUID& uuid);
		Memory::ObjectPtr<BaseComponent> GetComponent(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo);
		Memory::ObjectPtr<BaseNode> GetNode(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo);

	public :
		template<typename T, typename... Args>
		Memory::ObjectPtr<T> CreateComponent(const ECS::UUID& id, Args&&... args)
		{
			return m_componentContainer.Create<T>(id, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		Memory::ObjectPtr<T> CreateNode(const ECS::UUID& id, Args&&... args)
		{
			return m_nodeContainer.Create<T>(id, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		Memory::ObjectPtr<T> CreateSystem()
		{
			return m_systemRegistry.Create<T>();
		}

	private :
		PROPERTY(m_entityStorage);
		ECS::ObjectStorage<Entity> m_entityStorage;

		PROPERTY(m_nodeContainer);
		ECS::Registry<BaseNode> m_nodeContainer;

		PROPERTY(m_componentContainer);
		ECS::Registry<BaseComponent> m_componentContainer;

		PROPERTY(m_systemRegistry);
		ECS::SystemRegistry m_systemRegistry;
	};
};

#endif // __WTR_WORLD_H__