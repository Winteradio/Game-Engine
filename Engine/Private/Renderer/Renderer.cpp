#include <Renderer/Renderer.h>

#include <Renderer/RenderCommandList.h>
#include <Renderer/RenderWorker.h>
#include <RHI/OpenGL/GLSystem.h>

#include <Log/include/Log.h>
#include <Memory/include/Core.h>

namespace wtr
{
	Renderer::Renderer()
		: m_system()
		, m_worker()
	{}

	Renderer::~Renderer()
	{}

	bool Renderer::Init(const RenderDesc& desc, void* nativeHandle)
	{
		if (eRenderType::eOpenGL == desc.Type)
		{
			m_system = Memory::MakePtr<GLSystem>();
		}

		if (!m_system)
		{
			LOGERROR() << "[RENDERER] Failed to create the rhi system";
			return false;
		}

		if (!m_system->Init(nativeHandle))
		{
			LOGERROR() << "[RENDERER] Failed to initialize the rhi system";
			return false;
		}

		return true;
	}

	void Renderer::Run()
	{
		if (m_worker)
		{
			m_worker->Start();
		}
	}

	void Renderer::Stop()
	{
		if (m_worker)
		{
			m_worker->Stop();
		}
	}

	void Renderer::Render(RenderCommandList& commandList)
	{
		static size_t frame = 0;
		LOGINFO() << "[RENDERER] Frame : " << frame++;
	}

	void Renderer::SetWorker(const Memory::ObjectPtr<RenderWorker>& worker)
	{
		if (worker)
		{
			m_worker = worker;

			RenderWorker::RenderFunc render = [this](RenderCommandList& commandList)
			{
				if (this)
				{
					this->Render(commandList);
				}
			};

			m_worker->SetFunction(render);
		}
	}
}