#ifndef __WTR_RHICOMMANDEXECUTOR_H__
#define __WTR_RHICOMMANDEXECUTOR_H__

namespace wtr
{
	class RHICommandExecutor : public Memory::RefCounted
	{
	public :
		RHICommandExecutor();
		~RHICommandExecutor();

	public :
		// Methods to execute RHI commands would go here

	private :
		// Private members for command execution
	};
};

#endif // __WTR_RHICOMMANDEXECUTOR_H__