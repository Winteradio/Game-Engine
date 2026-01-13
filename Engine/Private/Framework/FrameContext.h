#ifndef __WTR_FRAMECONTEXT_H__
#define __WTR_FRAMECONTEXT_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Renderer/RenderCommandList.h>

#include <thread>
#include <mutex>
#include <condition_variable>

namespace wtr
{
	enum class eWorkerType : uint8_t;
};

namespace wtr
{
	class FrameContext : public Memory::RefCounted
	{
	public :
		FrameContext();
		~FrameContext();

	public :
		RenderCommandList& Acquire(const eWorkerType eType);
		void Return(const eWorkerType eType, RenderCommandList& commandList);
		void Notify(const eWorkerType eType);

	private :
		static constexpr size_t COMMAND_BUFFER = 3;

		std::mutex m_mutexWorld;
		std::mutex m_mutexRender;
		std::condition_variable m_cvWorld;
		std::condition_variable m_cvRender;
		RenderCommandList m_commandList[COMMAND_BUFFER];

		bool m_isRunning;
	};
}

#endif // __WTR_FRAMECONTEXT_H__