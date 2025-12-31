#ifndef __WTR_RENDERCONTEXT_H__
#define __WTR_RENDERCONTEXT_H__

#include <Memory/include/Pointer/RefPtr.h>

#include <Renderer/RenderCommandList.h>

#include <thread>
#include <mutex>
#include <condition_variable>

namespace wtr
{
	enum class eWorkerType : uint8_t;
}

namespace wtr
{
	class RenderContext : public Memory::RefCounted
	{
	public :
		RenderContext();
		~RenderContext();

	public :
		RenderCommandList& Acquire(const eWorkerType eType);
		void Return(const eWorkerType eType, RenderCommandList& commandList);

	private :
		static constexpr size_t COMMAND_BUFFER = 3;

		std::mutex m_mutexWorld;
		std::mutex m_mutexRender;
		std::condition_variable m_cvWorld;
		std::condition_variable m_cvRender;
		RenderCommandList m_commandList[COMMAND_BUFFER];
	};
}

#endif // __WTR_RENDERCONTEXT_H__