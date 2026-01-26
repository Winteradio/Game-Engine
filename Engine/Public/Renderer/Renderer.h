#ifndef __WTR_RENDERER_H__
#define __WTR_RENDERER_H__

#include <Renderer/RenderTypes.h>
#include <Renderer/RenderWorker.h>
#include <RHI/RHISystem.h>

#include <Memory/include/Pointer/ObjectPtr.h>
#include <Memory/include/Pointer/RootPtr.h>

namespace wtr
{
	class FrameView;
};

namespace wtr
{
	struct RenderDesc
	{
		eRenderType	Type;

		RenderDesc()
			: Type(eRenderType::eNone)
		{}
	};

	class Renderer
	{
	public :
		Renderer();
		~Renderer();

	public :
		bool Init(const RenderDesc& desc, void* nativeHandle);

		void Run();
		void Stop();

		void Render(FrameView& frame);
		void SetWorker(const Memory::ObjectPtr<RenderWorker>& worker);

	private :
		Memory::RootPtr<RHISystem> m_system;
		Memory::RootPtr<RenderWorker> m_worker;
	};
};

#endif // __WTR_RENDERER_H__