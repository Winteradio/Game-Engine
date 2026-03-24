#pragma once

#include <Memory/include/Core.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <functional>

namespace wtr
{
	struct Task
	{
		using Func = std::function<void()>;
		Func func;

		Task() = default;
		Task(Func func) : func(std::move(func)) {}
		~Task() = default;

		template<typename Logic, typename Callback, typename... Args>
		Task(Logic&& logic, Callback&& callback, Args&&... args)
		{
			func = [taskLogic = std::forward<Logic>(logic),
				taskCallback = std::forward<Callback>(callback),
				argsTuple = std::make_tuple(std::forward<Args>(args)...)]() mutable
			{
				if constexpr (std::is_void_v<std::invoke_result_t<Logic, Args...>>)
				{
					std::apply(taskLogic, std::move(argsTuple));
					taskCallback();
				}
				else
				{
					auto result = std::apply(taskLogic, std::move(argsTuple));
					taskCallback(result);
				}
			};
		}

		void operator()()
		{
			if (func)
			{
				func();
			}
		}
	};
};