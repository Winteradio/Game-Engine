#ifndef __WTR_RENDERTASK_H__
#define __WTR_RENDERTASK_H__

#include <Framework/Task.h>

namespace wtr
{
	class Renderer;
	class RHICommandList;
};

namespace wtr
{
	struct RenderTask : Task<Renderer*, Memory::RefPtr<RHICommandList>>
	{
		using Task::Task;

		RenderTask* next = nullptr;

		~RenderTask() = default;
	};
};

#endif // __WTR_RENDERTASK_H__