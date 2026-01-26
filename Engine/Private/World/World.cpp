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
		, m_worker()
	{}

	World::~World()
	{}

	void World::Run()
	{
		if (m_worker)
		{
			m_worker->Start();
		}
	}

	void World::Stop()
	{
		if (m_worker)
		{
			m_worker->Stop();
		}
	}

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

	void World::Render(FrameView& frame)
	{
		LOGINFO() << "[WORLD] Frame : " << frame.GetFrame();
	}

	void World::SetWorker(const Memory::ObjectPtr<WorldWorker>& worker)
	{
		if (worker)
		{
			m_worker = worker;

			WorldWorker::UpdateFunc update = [this](const ECS::TimeStep& timeStep)
			{
				if (this)
				{
					this->Update(timeStep);
				}
			};

			WorldWorker::RenderFunc render = [this](FrameView& frame)
			{
				if (this)
				{
					this->Render(frame);
				}
			};

			m_worker->SetFunction(update);
			m_worker->SetFunction(render);
		}
	}
}