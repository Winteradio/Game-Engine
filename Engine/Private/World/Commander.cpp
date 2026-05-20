#include <World/Commander.h>

#include <Renderer/RenderView.h>
#include <Renderer/RenderTask.h>
#include <Renderer/RenderCommandList.h>
#include <Renderer/RenderScene.h>
#include <Renderer/Renderer.h>

#include <World/Node.h>
#include <Log/include/Log.h>

namespace wtr
{
	Commander::Commander()
		: m_refCmdList()
	{}

	Commander::~Commander()
	{}

	void Commander::SetCommand(Memory::RefPtr<RenderCommandList> refCmdList)
	{
		m_refCmdList = refCmdList;
	}

	void Commander::SetView(const RenderView& renderView)
	{
		if (!m_refCmdList)
		{
			return;
		}

		RenderTask task;
		task.func = [renderView](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				if (!renderer)
				{
					return;
				}

				renderer->SetView(renderView);
			};

		Enqueue(std::move(task));
	}

	void Commander::Enqueue(RenderTask&& task)
	{
		if (!m_refCmdList)
		{
			return;
		}

		m_refCmdList->Enqueue(std::forward<RenderTask>(task));
	}

	void Commander::RemoveAll()
	{
		if (!m_refCmdList)
		{
			return;
		}

		RenderTask task;
		task.func = [](Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList)
			{
				LOGINFO() << "[Renderer] Remove all primitive and light";
				if (nullptr == renderer)
				{
					return;
				}

				auto renderScene = renderer->GetScene();
				if (!renderScene)
				{
					return;
				}

				renderScene->RemoveAll(cmdList);
			};

		m_refCmdList->Enqueue(std::move(task));
	}
}