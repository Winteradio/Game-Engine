#ifndef __WTR_FRAMECONTEXT_H__
#define __WTR_FRAMECONTEXT_H__

#include <Memory/include/Pointer/RefPtr.h>
#include <Framework/FrameView.h>

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
		FrameView& Acquire(const eWorkerType eType);
		void Return(const eWorkerType eType, FrameView& commandList);
		void Notify(const eWorkerType eType);

	private :
		static constexpr size_t FRAME_BUFFER = 3;

		std::mutex m_mutexWorld;
		std::mutex m_mutexRender;
		std::condition_variable m_cvWorld;
		std::condition_variable m_cvRender;
		FrameView m_frameList[FRAME_BUFFER];

		bool m_isRunning;
	};
}

#endif // __WTR_FRAMECONTEXT_H__