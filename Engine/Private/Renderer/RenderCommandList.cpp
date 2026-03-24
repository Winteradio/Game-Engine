#include <Renderer/RenderCommandList.h>

namespace wtr
{
	RenderCommandList::RenderCommandList()
		: m_head{}
		, m_tail{}
		, m_allocator()
		, m_writeIndex{0}
		, m_readIndex(0)
	{}

	RenderCommandList::~RenderCommandList()
	{}

	void RenderCommandList::Enqueue(Task::Func&& func)
	{
		RenderTask* task = Create(std::forward<Task::Func>(func));
		if (nullptr == task)
		{
			return;
		}

		RenderTask* oldTail = m_tail.load(std::memory_order_relaxed);

		if (oldTail)
		{
			oldTail->next = task;
		}
		else
		{
			m_head.store(task, std::memory_order_release);
		}

		m_tail.store(task, std::memory_order_release);
	}

	void RenderCommandList::ExecuteAll()
	{
		const size_t writeIndex = m_writeIndex.load(std::memory_order_acquire);
		const size_t readIndex = m_readIndex;
		
		m_writeIndex.store(readIndex, std::memory_order_release);
		m_readIndex = writeIndex;
		
		RenderTask* execute = m_head.exchange(nullptr, std::memory_order_acquire);
		m_tail.store(nullptr, std::memory_order_release);

		while (nullptr != execute)
		{
			(*execute)();
			execute->~RenderTask();
			execute = execute->next;
		}
	}

	void RenderCommandList::Reset()
	{
		m_allocator[m_readIndex].Reset();
	}

	RenderTask* RenderCommandList::Create(Task::Func&& func)
	{
		const size_t index = m_writeIndex.load(std::memory_order_relaxed);

		auto& allocator = m_allocator[index];

		void* memory = allocator.Allocate<RenderTask>();
		if (nullptr == memory)
		{
			return nullptr;
		}

		RenderTask* task = new (memory) RenderTask(std::forward<Task::Func>(func));
		return task;
	}
}