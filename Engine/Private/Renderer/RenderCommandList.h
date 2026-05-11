#ifndef __WTR_RENDERCOMMANDLIST_H__
#define __WTR_RENDERCOMMANDLIST_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Container/include/LinearArena.h>

#include <atomic>

namespace wtr
{
	struct RenderTask;
	class Renderer;
	class RHICommandList;
};

namespace wtr
{
	class RenderCommandList
	{
	public :
		RenderCommandList();
		virtual ~RenderCommandList();

	public :
		void Enqueue(RenderTask&& func);
		void ExecuteAll(Renderer* renderer, Memory::RefPtr<RHICommandList> cmdList);
		void Reset();

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