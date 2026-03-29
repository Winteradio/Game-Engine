#include <Renderer/Renderer.h>

#include <Renderer/RenderScene.h>
#include <Renderer/RenderCommandList.h>
#include <Renderer/RenderGraph.h>
#include <Renderer/RenderView.h>

#include <RHI/RHICommandList.h>

#include <Memory/include/Core.h>
#include <Log/include/Log.h>
namespace wtr
{
	Renderer::Renderer()
		: m_refScene()
		, m_refGraph()
		, m_refCommandList()
	{}

	Renderer::~Renderer()
	{}

	bool Renderer::Init()
	{
		LOGINFO() << "[RENDERER] Initialize the renderer";

		m_refScene = Memory::MakeRef<RenderScene>();
		if (!m_refScene)
		{
			LOGERROR() << "[RENDERER] Failed to create the render scene";
			return false;
		}

		m_refGraph = Memory::MakeRef<RenderGraph>();
		if (!m_refGraph)
		{
			LOGERROR() << "[RENDERER] Failed to create the render graph";
			return false;
		}

		if (!m_refGraph->Init())
		{
			LOGERROR() << "[RENDERER] Failed to initialize the render graph";
			return false;
		}

		m_refCommandList = Memory::MakeRef<RenderCommandList>();
		if (!m_refCommandList)
		{
			LOGERROR() << "[RENDERER] Failed to create the render command list";
			return false;
		}

		LOGINFO() << "[RENDERER] Succeed to initialize the renderer";

		return true;
	}

	void Renderer::Execute(Memory::RefPtr<RHICommandList> cmdList)
	{
		if (!cmdList || m_refCommandList)
		{
			return;
		}

		
	}
}