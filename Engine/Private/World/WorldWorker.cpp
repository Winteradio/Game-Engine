#include <World/WorldWorker.h>

#include <Framework/Input/InputStorage.h>
#include <Framework/Player.h>
#include <Framework/PlayerController.h>
#include <Framework/ViewController.h>
#include <Framework/FrameGate.h>
#include <Renderer/RenderView.h>
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
		, m_refProducer(nullptr)
		, m_renderViews()
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

	void WorldWorker::SetProducer(const Memory::RefPtr<FrameProducer> producer)
	{
		if (producer)
		{
			m_refProducer = producer;
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

		for (const auto& renderView : m_renderViews)
		{
			commander->SetView(renderView);
		}

		if (m_refProducer)
		{
			m_refProducer->Submit();
		}
	}

	void WorldWorker::onNotify()
	{
		if (m_refProducer)
		{
			m_refProducer->NotifyAll();
		}
	}

	void WorldWorker::UpdateView(Memory::RefPtr<PlayerController> playerController, Memory::RefPtr<ViewController> viewController)
	{
		if (!playerController || !viewController)
		{
			return;
		}

		m_renderViews.Clear();

		const auto& actives = playerController->GetActives();
		for (const auto& player : actives)
		{
			if (!player)
			{
				continue;
			}

			const auto& viewList = player->GetViews();

			for (const auto& viewId : viewList)
			{
				const auto view = viewController->Get(viewId);
				if (!view)
				{
					continue;
				}

				RenderView renderView = MakeView(player, view);

				m_renderViews.PushBack(renderView);
			}
		}
	}

	RenderView WorldWorker::MakeView(Memory::RefPtr<Player> player, Memory::RefPtr<ViewInfo> view)
	{
		if (!player || !view)
		{
			return {};
		}

		Memory::ObjectPtr<const CameraComponent> camera = player->GetCamera();
		Memory::ObjectPtr<const SceneComponent> transform = player->GetTransform();

		if (!camera || !transform)
		{
			return {};
		}

		RenderView RenderView;
		RenderView.viewport.width = view->GetWidth();
		RenderView.viewport.height = view->GetHeight();
		RenderView.viewport.posX = view->GetPosX();
		RenderView.viewport.posY = view->GetPosY();

		RenderView.camera.fov = camera->fov;
		RenderView.camera.nearPlane = camera->nearPlane;
		RenderView.camera.farPlane = camera->farPlane;
		RenderView.camera.position = transform->GetPosition();
		RenderView.camera.rotation = transform->GetRotation();

		return RenderView;
	}
}