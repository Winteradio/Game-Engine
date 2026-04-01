#include <World/WorldContext.h>

#include <World/World.h>
#include <World/WorldCommandList.h>
#include <World/Commander.h>
#include <Framework/ViewController.h>
#include <Framework/PlayerController.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>

namespace wtr
{
	WorldContext::WorldContext()
		: world()
		, views()
		, players()
		, m_refCommandList()
	{}

	WorldContext::~WorldContext()
	{}

	bool WorldContext::Init()
	{
		views = Memory::MakeRef<ViewController>();
		if (!views)
		{
			LOGERROR() << "[WorldContext] Failed to create the view controller";
			return false;
		}

		players = Memory::MakeRef<PlayerController>();
		if (!players)
		{
			LOGERROR() << "[WorldContext] Failed to create the player controller";
			return false;
		}

		commander = Memory::MakeRef<Commander>();
		if (!commander)
		{
			LOGERROR() << "[WorldContext] Failed to create the commander";
			return false;
		}

		world = Memory::MakePtr<World>();
		if (!world)
		{
			LOGERROR() << "[WorldContext] Failed to create the world";
			return false;
		}

		if (!world->Init(commander))
		{
			LOGERROR() << "[WorldContext] Failed to initialize the world";
			return false;
		}

		m_refCommandList = Memory::MakeRef<WorldCommandList>();
		if (!m_refCommandList)
		{
			LOGERROR() << "[WorldContext] Failed to create the world command list";
			return false;
		}

		LOGINFO() << "WorldContext] Succeed to initialize the world context";

		return true;
	}

	void WorldContext::Clear()
	{
		if (world)
		{
			world->Clear();
		}
	}

	void WorldContext::Prepare()
	{
		if (!m_refCommandList)
		{
			return;
		}

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