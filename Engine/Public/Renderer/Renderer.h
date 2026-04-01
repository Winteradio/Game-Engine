#ifndef __WTR_RENDERER_H__
#define __WTR_RENDERER_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Renderer/RenderTypes.h>

namespace wtr
{
	class RenderScene;
	class RenderCommandList;
	class RenderGraph;
	class RenderView;
	class RHICommandList;
};

namespace wtr
{
	struct RenderDesc
	{
		eRenderType Type = eRenderType::eNone;
		size_t FrameCount = 3;
	};

	class Renderer
	{
	public :
		Renderer();
		~Renderer();

	public :
		bool Init();

		void Execute(Memory::RefPtr<RHICommandList> cmdList);
		
		void PreDraw(Memory::RefPtr<RHICommandList> cmdList);
		void Draw(Memory::RefPtr<RHICommandList> cmdList);
		void PostDraw(Memory::RefPtr<RHICommandList> cmdList);

		Memory::RefPtr<RenderCommandList> GetCommandList();

	private :
		Memory::RefPtr<RenderScene> m_refScene;
		Memory::RefPtr<RenderGraph> m_refGraph;
		Memory::RefPtr<RenderCommandList> m_refCommandList;
	};
};

#endif // __WTR_RENDERER_H__