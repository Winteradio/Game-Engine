#ifndef __WTR_COMMANDER_H__
#define __WTR_COMMANDER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Memory/include/Pointer/ObjectPtr.h>

namespace wtr
{
	struct SceneNode;
	struct MeshNode;
	struct LightNode;

	struct FrameView;
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

		void DrawView(const FrameView& frameView);

		void AddNode(Memory::ObjectPtr<SceneNode> node);
		void UpdateNode(Memory::ObjectPtr<SceneNode> node);
		void RemoveNode(Memory::ObjectPtr<SceneNode> node);

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