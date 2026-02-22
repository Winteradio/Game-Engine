#include <Framework/FrameContext.h>

#include <Framework/Worker.h>
#include <Log/include/Log.h>

namespace wtr
{
	FrameContext::FrameContext()
		: m_mutexWorld()
		, m_mutexRender()
		, m_cvWorld()
		, m_cvRender()
		, m_frameList()
		, m_isRunning(true)
	{}

	FrameContext::~FrameContext()
	{}

	FrameView& FrameContext::Acquire(const eWorkerType eType)
	{
		if (eWorkerType::eProceduer == eType)
		{
			std::unique_lock<std::mutex> lock(m_mutexWorld);

			auto checkFree = [this]()
			{
				for (auto& frame : this->m_frameList)
				{
					if (eFrameState::eFree == frame.GetState())
					{
						return true;
					}
				}

				return !this->m_isRunning;
			};

			m_cvWorld.wait(lock, checkFree);

			for (auto& frame : m_frameList)
			{
				if (eFrameState::eFree == frame.GetState())
				{
					frame.SetState(eFrameState::eWriting);
					return frame;
				}
			}
		}
		else
		{
			std::unique_lock<std::mutex> lock(m_mutexRender);

			auto checkReady = [this]()
			{
				for (auto& frame : this->m_frameList)
				{
					if (eFrameState::eReady == frame.GetState())
					{
						return true;
					}
				}

				return !this->m_isRunning;
			};

			m_cvRender.wait(lock, checkReady);

			size_t minFrame = size_t(0) - 1;
			size_t minIndex = size_t(0) - 1;

			for (size_t index = 0; index < FRAME_BUFFER; index++)
			{
				auto& frame = m_frameList[index];

				if (eFrameState::eReady == frame.GetState() && minFrame >= frame.GetFrame())
				{
					minFrame = frame.GetFrame();
					minIndex = index;
				}
			}

			if (minIndex < FRAME_BUFFER)
			{
				return m_frameList[minIndex];
			}
		}

		static FrameView nullList;
		return nullList;
	}

	void FrameContext::Return(const eWorkerType eType, FrameView& frame)
	{
		if (eWorkerType::eProceduer == eType)
		{
			{
				std::lock_guard<std::mutex> lock(m_mutexRender);
				frame.SetState(eFrameState::eReady);
			}

			m_cvRender.notify_all();
		}
		else
		{
			{
				std::lock_guard<std::mutex> lock(m_mutexWorld);
				frame.SetState(eFrameState::eFree);
			}

			m_cvWorld.notify_all();
		}
	}

	void FrameContext::Notify(const eWorkerType eType)
	{
		if (eWorkerType::eProceduer == eType)
		{
			m_cvWorld.notify_all();
		}
		else
		{
			m_cvRender.notify_all();
		}

		m_isRunning = false;
	}
}