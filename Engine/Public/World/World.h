#ifndef __WTR_WORLD_H__
#define __WTR_WORLD_H__

#include <ECS/include/Container/DataRegistry.h>
#include <ECS/include/Container/SystemRegistry.h>

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

	public :
		World();
		virtual ~World();

	public :
		void SetCommander(Memory::RefPtr<Commander> refCommander);
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
		Memory::ObjectPtr<ECS::Component> GetComponent(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo);
		Memory::ObjectPtr<ECS::Node> GetNode(const ECS::UUID& uuid, const Reflection::TypeInfo* typeinfo);

	public :
		template<typename T, typename... Args>
		Memory::ObjectPtr<T> CreateComponent(Args&&... args)
		{
			return m_componentContainer.Create<T>(std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		Memory::ObjectPtr<T> CreateNode(Args&&... args)
		{
			return m_nodeContainer.Create<T>(std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		Memory::ObjectPtr<T> CreateSystem()
		{
			return m_systemRegistry.Create<T>();
		}

	private :
		Memory::RefPtr<Commander> m_refCommander;

		PROPERTY(m_entityContainer);
		ECS::Container<Entity> m_entityContainer;

		PROPERTY(m_nodeContainer);
		ECS::NodeRegistry m_nodeContainer;

		PROPERTY(m_componentContainer);
		ECS::ComponentRegistry m_componentContainer;

		PROPERTY(m_systemRegistry);
		ECS::SystemRegistry m_systemRegistry;
	};
};

#endif // __WTR_WORLD_H__