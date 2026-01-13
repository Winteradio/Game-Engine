#ifndef __WTR_RENDERCOMMANDLIST_H__
#define __WTR_RENDERCOMMANDLIST_H__

#include <Framework/CommandList.h>
#include <Renderer/RenderCommands.h>

namespace wtr
{
	class RenderCommandList : public CommandList<RenderCommandBase>
	{
	public :
		RenderCommandList();
		~RenderCommandList();

	public :
		size_t GetFrame() const;
		void SetFrame(const size_t frame);

		void SetClearCommand();
		void SetDrawCommand();
		void SetDispatchCommand();
		void SetUpdateBufferCommand();
		void SetUpdateTextureCommand();
		void SetUpdateSamplerCommand();
		void SetCopyBufferCommand();
		void SetCopyTextureCommand();
		void SetViewportCommand();

	private :
		size_t m_frame;
	};
}

#endif // __WTR_RENDERCOMMANDLIST_H__