#pragma once

#include <Memory/include/Core.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <functional>

namespace wtr
{
	template<typename... Args>
	struct Task
	{
		using Func = std::function<void(Args...)>;
		Func func;

		Task() = default;
		Task(Func func) : func(std::move(func)) {}
		~Task() = default;

		void operator()(Args... args)
		{
			if (func)
			{
				func(std::forward<Args>(args)...);
			}
		}
	};

	using DefaultTask = Task<>;
};