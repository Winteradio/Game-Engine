#ifndef __WTR_COMMANDER_H__
#define __WTR_COMMANDER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Memory/include/Pointer/ObjectPtr.h>

namespace wtr
{
	class BaseNode;
	class MeshNode;
	class LightNode;

	struct RenderView;
	class RenderCommandList;
};

namespace wtr
{
	class Commander
	{
	public :
		Commander();
		~Commander();

	public :
		void SetCommand(Memory::RefPtr<RenderCommandList> refCmdList);

		void DrawView(const RenderView& RenderView);

		void AddNode(Memory::ObjectPtr<BaseNode> node);
		void UpdateNode(Memory::ObjectPtr<BaseNode> node);
		void RemoveNode(Memory::ObjectPtr<BaseNode> node);

	private :
		void AddPrimitive(Memory::ObjectPtr<MeshNode> meshNode);
		void RemovePrimitive(Memory::ObjectPtr<MeshNode> meshNode);

		void AddLight(Memory::ObjectPtr<LightNode> lightNode);
		void RemoveLight(Memory::ObjectPtr<LightNode> lightNode);

	private :
		Memory::RefPtr<RenderCommandList> m_refCmdList;
	};
};

#endif // __WTR_SCENEWRAPPER_H__