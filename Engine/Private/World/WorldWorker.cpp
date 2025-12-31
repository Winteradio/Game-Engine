#include <World/WorldWorker.h>

#include <Framework/Input/InputStorage.h>
#include <Renderer/RenderContext.h>

namespace wtr
{
	WorldWorker::WorldWorker()
		: m_refInputStorage()
	{}

	WorldWorker::~WorldWorker()
	{}

	bool WorldWorker::Init(const Memory::RefPtr<InputStorage> refInputStorage, const Memory::RefPtr<RenderContext> refRenderContext)
	{
		if (!refInputStorage || !refRenderContext)
		{
			return false;
		}

		m_refInputStorage = refInputStorage;
		m_refRenderContext = refRenderContext;

		return true;
	}

	void WorldWorker::onStart()
	{}

#include <Windows.h>

	void WorldWorker::onUpdate()
	{
		Sleep(500);

		if (m_refInputStorage)
		{
			m_refInputStorage->SwapInput();
		}
	}

	void WorldWorker::onDestroy()
	{
		m_refInputStorage.Reset();
	}
}