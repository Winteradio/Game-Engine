#include <World/WorldContext.h>

#include <World/World.h>
#include <World/WorldCommandList.h>
#include <Framework/ViewController.h>
#include <Framework/PlayerController.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	WorldContext::WorldContext()
		: world()
		, view()
		, player()
		, m_refCommandList()
	{}

	WorldContext::~WorldContext()
	{}

	bool WorldContext::Init()
	{
		world = Memory::MakePtr<World>();
		if (!world)
		{
			LOGERROR() << "[WORLDCONTEXT] Failed to create the world";
			return false;
		}

		view = Memory::MakeRef<ViewController>();
		if (!view)
		{
			LOGERROR() << "[WORLDCONTEXT] Failed to create the view controller";
			return false;
		}

		player = Memory::MakeRef<PlayerController>();
		if (!player)
		{
			LOGERROR() << "[WORLDCONTEXT] Failed to create the player controller";
			return false;
		}

		m_refCommandList = Memory::MakeRef<WorldCommandList>();
		if (!m_refCommandList)
		{
			LOGERROR() << "[WORLDCONTEXT] Failed to create the world command list";
			return false;
		}

		LOGINFO() << "[WORLDCONTEXT] Succeed to initialize the world context";

		return true;
	}

	void WorldContext::Prepare()
	{
		// TODO
	}

	void WorldContext::Update(const ECS::TimeStep& timeStep)
	{
		if (!world)
		{
			return;
		}

		world->Update(timeStep);
	}
}