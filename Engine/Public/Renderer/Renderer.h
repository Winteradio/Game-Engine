#ifndef __WTR_RENDERER_H__
#define __WTR_RENDERER_H__

#include <Container/include/DynamicArray.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Renderer/RenderTypes.h>

namespace wtr
{
	struct RenderView;

	class RenderScene;
	class RenderCommandList;
	class RenderGraph;
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
		
		void SetView(const RenderView& view);

		void PreDraw(Memory::RefPtr<RHICommandList> cmdList);
		void Draw(Memory::RefPtr<RHICommandList> cmdList);
		void PostDraw(Memory::RefPtr<RHICommandList> cmdList);

		Memory::RefPtr<RenderScene> GetScene();
		Memory::RefPtr<RenderGraph> GetGraph();
		Memory::RefPtr<RenderCommandList> GetCommandList();

	private :
		Memory::RefPtr<RenderScene> m_refScene;
		Memory::RefPtr<RenderGraph> m_refGraph;
		Memory::RefPtr<RenderCommandList> m_refCommandList;

		wtr::DynamicArray<RenderView> m_renderViews;
	};
};

#endif // __WTR_RENDERER_H__