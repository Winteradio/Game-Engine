#ifndef __WTR_RENDERCOMMANDLIST_H__
#define __WTR_RENDERCOMMANDLIST_H__

#include <Framework/Task.h>
#include <Container/include/LinearArena.h>

#include <atomic>

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

	class RenderCommandList
	{
	public :
		RenderCommandList();
		virtual ~RenderCommandList();

	public :
		void Enqueue(RenderTask::Func&& func);
		void ExecuteAll(Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList);
		void Reset();

	private :
		RenderTask* Create(RenderTask::Func&& func);

	private :
		constexpr static size_t MAX_ALLOCATOR_COUNT = 2;

		std::atomic<RenderTask*> m_head;
		std::atomic<RenderTask*> m_tail;

		wtr::LinearArena m_allocator[MAX_ALLOCATOR_COUNT];
		std::atomic<size_t> m_writeIndex;
		size_t m_readIndex;
	};
};

#endif // __WTR_RENDERCOMMANDLIST_H__