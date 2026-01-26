#ifndef __WTR_WORLD_H__
#define __WTR_WORLD_H__

#include <ECS/include/Registry/ComponentRegistry.h>
#include <ECS/include/Registry/NodeRegistry.h>
#include <ECS/include/Registry/SystemRegistry.h>
#include <ECS/include/Container/EntityContainer.h>

#include <World/Scene.h>
#include <World/WorldWorker.h>

namespace wtr
{
	class FrameView;
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
		PROPERTY(entity);
		ECS::EntityContainer entity;

		PROPERTY(node);
		ECS::NodeRegistry node;

		PROPERTY(component);
		ECS::ComponentRegistry component;

		PROPERTY(system);
		ECS::SystemRegistry system;

		PROPERTY(scene);
		SceneContainer scene;

	public :
		void Run();
		void Stop();

		void Update(const ECS::TimeStep& timeStep);
		void Render(FrameView& frame);

		void SetWorker(const Memory::ObjectPtr<WorldWorker>& worker);

	private :
		Memory::RootPtr<WorldWorker> m_worker;
	};
};

#endif // __WTR_WORLD_H__