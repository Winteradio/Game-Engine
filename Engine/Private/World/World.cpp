#include <World/World.h>

#include <World/WorldWorker.h>
#include <Framework/FrameView.h>

namespace wtr
{
	World::World()
		: entity()
		, node()
		, component()
		, system()
		, scene()
	{}

	World::~World()
	{}

	void World::Update(const ECS::TimeStep& timeStep)
	{
		const ECS::SystemRegistry::GraphType graph = system.BuildGraph();

		for (const auto& scenePair : scene.GetScene())
		{
			const auto& scene = scenePair.second;
			const wtr::DynamicArray<ECS::UUID> partialID = graph.GetPartialSorted(scene.GetSystemID());

			for (const auto& systemID : partialID)
			{
				auto sSystem = system.Get(systemID);
				if (!sSystem)
				{
					continue;
				}

				const auto nodeType = sSystem->GetNodeType();
				auto nodeContainer = node.GetContainer(nodeType);

				sSystem->Update(timeStep, nodeContainer);
			}
		}
	}

	void World::Render(const FrameView& frame)
	{
		//LOGINFO() << "[WORLD] Frame : " << frame.GetFrame();
	}
}