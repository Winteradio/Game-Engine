#pragma once

#include <Framework/Task.h>
#include <Container/include/LinearArena.h>

#include <atomic>

namespace wtr
{
	class RenderScene;
	class RHICommandList;
};

namespace wtr
{
	struct RenderTask : Task
	{
		using Task::Task;

		RenderTask* next = nullptr;

		~RenderTask() = default;
	};

	class RenderCommandList
	{
	public :
		RenderCommandList() = default;
		virtual ~RenderCommandList() = default;

	public :
		void Enqueue(Task::Func&& func);
		void ExecuteAll();
		void Reset();

	public :
		template<typename... Args>
		void Enqueue(Args&&... args)
		{
			Enqueue(Task::Func(std::forward<Args>(args)...));
		}

	private :
		RenderTask* Create(Task::Func&& func);

	private :
		constexpr static size_t MAX_ALLOCATOR_COUNT = 2;

		std::atomic<RenderTask*> m_head;
		std::atomic<RenderTask*> m_tail;

		wtr::LinearArena m_allocator[MAX_ALLOCATOR_COUNT];
		std::atomic<size_t> m_writeIndex;
		size_t m_readIndex;
	};
};