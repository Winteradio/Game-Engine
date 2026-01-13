#include <Framework/FrameContext.h>

#include <Framework/Worker.h>

namespace wtr
{
	FrameContext::FrameContext()
		: m_mutexWorld()
		, m_mutexRender()
		, m_cvWorld()
		, m_cvRender()
		, m_commandList()
		, m_isRunning(true)
	{}

	FrameContext::~FrameContext()
	{}

	RenderCommandList& FrameContext::Acquire(const eWorkerType eType)
	{
		if (eWorkerType::eProceduer == eType)
		{
			std::unique_lock<std::mutex> lock(m_mutexWorld);

			auto checkFree = [this]()
			{
				for (auto& commandList : this->m_commandList)
				{
					if (eCommandState::eFree == commandList.GetState())
					{
						return true;
					}
				}

				return !this->m_isRunning;
			};

			m_cvWorld.wait(lock, checkFree);

			for (auto& commandList : m_commandList)
			{
				if (eCommandState::eFree == commandList.GetState())
				{
					commandList.SetState(eCommandState::eWriting);
					return commandList;
				}
			}
		}
		else
		{
			std::unique_lock<std::mutex> lock(m_mutexRender);

			auto checkReady = [this]()
				{
					for (auto& commandList : this->m_commandList)
					{
						if (eCommandState::eReady == commandList.GetState())
						{
							return true;
						}
					}

					return !this->m_isRunning;
				};

			m_cvRender.wait(lock, checkReady);

			size_t minFrame = size_t(0) - 1;
			size_t minIndex = size_t(0) - 1;

			for (size_t index = 0; index < COMMAND_BUFFER; index++)
			{
				auto& commandList = m_commandList[index];

				if (eCommandState::eReady == commandList.GetState() && minFrame >= commandList.GetFrame())
				{
					minFrame = commandList.GetFrame();
					minIndex = index;
				}
			}

			return m_commandList[minIndex];
		}

		static RenderCommandList nullList;

		return nullList;
	}

	void FrameContext::Return(const eWorkerType eType, RenderCommandList& commandList)
	{
		if (eWorkerType::eProceduer == eType)
		{
			commandList.SetState(eCommandState::eReady);

			m_cvRender.notify_one();
		}
		else
		{
			commandList.SetState(eCommandState::eFree);

			m_cvWorld.notify_one();
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