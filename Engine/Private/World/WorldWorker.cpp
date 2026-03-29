#include <World/WorldWorker.h>

#include <Framework/Input/InputStorage.h>
#include <Framework/Player.h>
#include <Framework/PlayerController.h>
#include <Framework/ViewController.h>
#include <Framework/FrameView.h>
#include <World/WorldContext.h>
#include <World/World.h>
#include <World/Commander.h>
#include <World/Component.h>

namespace wtr
{
	WorldWorker::WorldWorker()
		: m_timeStep()
		, m_refInputStorage(nullptr)
		, m_refWorldContext(nullptr)
		, m_frameViews()
	{}

	WorldWorker::~WorldWorker()
	{}

	void WorldWorker::SetInputStorage(const Memory::RefPtr<InputStorage> inputStorage)
	{
		if (inputStorage)
		{
			m_refInputStorage = inputStorage;
		}
	}

	void WorldWorker::SetWorldContext(const Memory::RefPtr<WorldContext> worldContext)
	{
		if (worldContext)
		{
			m_refWorldContext = worldContext;
		}
	}

	void WorldWorker::onStart()
	{}

	void WorldWorker::onUpdate()
	{
		m_timeStep.Tick();

		if (!m_refInputStorage || !m_refWorldContext)
		{
			return;
		}

		m_refInputStorage->SwapInput();
		m_refWorldContext->Update(m_timeStep);

		auto& commander = m_refWorldContext->commander;
		auto& views = m_refWorldContext->views;
		auto& players = m_refWorldContext->players;

		if (!commander || !players || !views)
		{
			return;
		}

		UpdateView(players, views);

		for (const auto& frameView : m_frameViews)
		{
			commander->DrawView(frameView);
		}
	}

	void WorldWorker::onDestroy()
	{
		if (!m_refWorldContext)
		{
			return;
		}

		auto& world = m_refWorldContext->world;
		if (!world)
		{
			return;
		}

		world->Clear();
	}

	void WorldWorker::UpdateView(Memory::RefPtr<PlayerController> playerController, Memory::RefPtr<ViewController> viewController)
	{
		if (!playerController || !viewController)
		{
			return;
		}

		m_frameViews.Clear();
	}

	FrameView WorldWorker::MakeView(Memory::RefPtr<Player> player, Memory::RefPtr<ViewInfo> view)
	{
		if (!player || !view)
		{
			return {};
		}

		Memory::ObjectPtr<const CameraComponent> camera = player->GetCamera();
		Memory::ObjectPtr<const TransformComponent> transform = player->GetTransform();

		if (!camera || !transform)
		{
			return {};
		}

		FrameView frameView;
		frameView.viewport.width = view->GetWidth();
		frameView.viewport.height = view->GetHeight();
		frameView.viewport.posX = view->GetPosX();
		frameView.viewport.posY = view->GetPosY();

		frameView.camera.fov = camera->fov;
		frameView.camera.nearPlane = camera->near;
		frameView.camera.farPlane = camera->far;
		frameView.camera.position = transform->position;
		frameView.camera.rotation = transform->rotation;

		return frameView;
	}
}