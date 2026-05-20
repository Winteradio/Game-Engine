#ifndef __WTR_COMMANDER_H__
#define __WTR_COMMANDER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Memory/include/Pointer/ObjectPtr.h>
#include <ECS/include/UUID/UUID.h>

namespace wtr
{
	struct RenderTask;
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
		void SetView(const RenderView& renderView);

		void Enqueue(RenderTask&& task);
		void RemoveAll();

	private :
		Memory::RefPtr<RenderCommandList> m_refCmdList;
	};
};

#endif // __WTR_SCENEWRAPPER_H__