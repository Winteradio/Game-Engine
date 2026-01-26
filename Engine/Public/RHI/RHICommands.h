#ifndef __WTR_RHICOMMANDS_H__
#define __WTR_RHICOMMANDS_H__

#include <Reflection/include/Utils.h>

namespace wtr
{
	class RHICommandBase
	{
	public :
		using ExecuteFunc = void(*)(RHICommandBase*);

		RHICommandBase(const ExecuteFunc func)
			: m_func(func)
		{}

	public :
		virtual void Execute()
		{
			if (m_func)
			{
				m_func(this);
			}
		}

	private :
		ExecuteFunc m_func = nullptr;
	};

	template<typename T>
	class RHICommand : public RHICommandBase
	{
	public :
		RHICommand()
			: RHICommandBase(&ExecuteAndDestruct)
		{}

	public :
		static void ExecuteAndDestruct(RHICommandBase* commandBase)
		{
			static_assert(Reflection::Utils::IsBase<RHICommandBase, T> && "The RHI Command's type is invalid");

			if (nullptr != commandBase)
			{
				T* command = static_cast<T>(RHICommandBase);
				command->Execute();
				command->~T();
			}
		}
	};

	class RHICommandResource : public RHICommand<RHICommandResource>
	{
	public :
		void Execute() override
		{

		}
	};

	class RHICommandState : public RHICommand<RHICommandState>
	{
	public :
		void Execute() override
		{

		}
	};
}

#endif // __WTR_RHICOMMANDS_H__