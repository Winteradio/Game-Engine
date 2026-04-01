#include <World/Commander.h>

#include <Reflection/include/Utils.h>
#include <Renderer/RenderCommandList.h>
#include <World/Node.h>

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

	void Commander::DrawView(const RenderView& RenderView)
	{
		if (!m_refCmdList)
		{
			return;
		}

		// TODO
	}

	void Commander::AddNode(Memory::ObjectPtr<BaseNode> node)
	{
		if (!node || !m_refCmdList)
		{
			return;
		}

		if (auto meshNode = Memory::Cast<MeshNode>(node))
		{
			AddPrimitive(meshNode);			
		}
		else if (auto lightNode = Memory::Cast<LightNode>(node))
		{
			AddLight(lightNode);
		}
		else
		{
			// nothing
		}
	}

	void Commander::UpdateNode(Memory::ObjectPtr<BaseNode> node)
	{
		if (!node || !m_refCmdList)
		{
			return;
		}
	}

	void Commander::RemoveNode(Memory::ObjectPtr<BaseNode> node)
	{
		if (!node || !m_refCmdList)
		{
			return;
		}
	}

	void Commander::AddPrimitive(Memory::ObjectPtr<MeshNode> meshNode)
	{
		if (!meshNode || !m_refCmdList)
		{
			return;
		}

		// TODO : Add primitive to command list.
	}

	void Commander::AddLight(Memory::ObjectPtr<LightNode> lightNode)
	{
		if (!lightNode || !m_refCmdList)
		{
			return;
		}

		// TODO : Add light to command list.
	}
}